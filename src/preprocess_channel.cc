#include "preprocess_channel.h"
#include "utils.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {
Napi::FunctionReference PreProcessChannel::constructor;

PreProcessChannel::PreProcessChannel(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PreProcessChannel>(info) {}

void PreProcessChannel::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  auto func =
      DefineClass(env, "PreProcessChannel",
                  {InstanceAccessor("meanValue", &PreProcessChannel::GetMean,
                                    &PreProcessChannel::SetMean),
                   InstanceAccessor("stdScale", &PreProcessChannel::GetSTDScale,
                                    &PreProcessChannel::SetSTDScale),
                   InstanceAccessor("meanData", &PreProcessChannel::GetMeanData,
                                    &PreProcessChannel::SetMeanData)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Napi::Value PreProcessChannel::GetMean(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->_actual->meanValue);
}

void PreProcessChannel::SetMean(const Napi::CallbackInfo& info,
                                const Napi::Value& value) {
  auto meanValue = value.As<Napi::Number>();
  this->_actual->meanValue = meanValue.FloatValue();
}

Napi::Value PreProcessChannel::GetSTDScale(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), this->_actual->stdScale);
}

void PreProcessChannel::SetSTDScale(const Napi::CallbackInfo& info,
                                    const Napi::Value& value) {
  auto stdScale = value.As<Napi::Number>();
  this->_actual->stdScale = stdScale.FloatValue();
}

Napi::Value PreProcessChannel::GetMeanData(const Napi::CallbackInfo& info) {
  auto env = info.Env();

  auto meanData = this->_actual->meanData;
  auto memoryMeanData = ie::as<ie::MemoryBlob>(meanData);
  auto tensorDesc = memoryMeanData->getTensorDesc();

  auto result = Napi::Object::New(env);
  auto tensorDescObj = Napi::Object::New(env);

  tensorDescObj.Set("precision",
                    utils::GetNameOfPrecision(tensorDesc.getPrecision()));
  tensorDescObj.Set("layout", utils::GetNameOfLayout(tensorDesc.getLayout()));

  auto dims = tensorDesc.getDims();
  auto dimsObj = Napi::Array::New(env, dims.size());

  for (size_t i = 0; i < dims.size(); i++) {
    dimsObj[i] = Napi::Number::New(env, dims[i]);
  }
  tensorDescObj.Set("dims", dimsObj);
  result.Set("tensorDesc", tensorDescObj);

  auto localMemory = memoryMeanData->rmap();
  auto byteSize = meanData->byteSize();
  auto buffer = Napi::ArrayBuffer::New(env, byteSize);
  auto meanDataBuffer = buffer.Data();
  switch (tensorDesc.getPrecision()) {
    case ie::Precision::FP32:
      memcpy(
          meanDataBuffer,
          localMemory
              .as<const ie::PrecisionTrait<ie::Precision::FP32>::value_type*>(),
          byteSize);
      break;
    case ie::Precision::FP16:
    case ie::Precision::I16:
      memcpy(
          meanDataBuffer,
          localMemory
              .as<const ie::PrecisionTrait<ie::Precision::FP16>::value_type*>(),
          byteSize);
      break;
    case ie::Precision::U8:
      memcpy(
          meanDataBuffer,
          localMemory
              .as<const ie::PrecisionTrait<ie::Precision::U8>::value_type*>(),
          byteSize);
      break;
    default:
      Napi::TypeError::New(env,
                           "Unsupported network precision ! Supported "
                           "precisions are: FP32, FP16, I16, U8")
          .ThrowAsJavaScriptException();
      return env.Null();
  }
  result.Set("data", buffer);
  return result;
}

void PreProcessChannel::SetMeanData(const Napi::CallbackInfo& info,
                                    const Napi::Value& value) {
  auto env = info.Env();

  if (!value.IsObject()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  Napi::Object meanData = value.ToObject();

  auto tensorDescFieldName = "tensorDesc";
  auto dataFieldName = "data";
  if (!meanData.Has(tensorDescFieldName) || !meanData.Has(dataFieldName)) {
    Napi::TypeError::New(env, "Wrong meanData").ThrowAsJavaScriptException();
    return;
  }
  auto tensorDesc = meanData.Get(tensorDescFieldName);
  if (!tensorDesc.IsObject()) {
    Napi::TypeError::New(env, "Wrong tensorDesc field")
        .ThrowAsJavaScriptException();
    return;
  }
  auto tensorDescObj = tensorDesc.ToObject();

  if (!utils::checkTensorDesc(tensorDescObj)) {
    Napi::TypeError::New(env, "Wrong desc").ThrowAsJavaScriptException();
    return;
  }
  auto data = meanData.Get(dataFieldName);
  if (!data.IsArrayBuffer()) {
    Napi::TypeError::New(env, "Wrong data").ThrowAsJavaScriptException();
    return;
  }

  auto meanDataArray = Napi::ArrayBuffer(env, data);
  std::string precisionString = tensorDescObj.Get("precision").ToString();
  std::string layoutString = tensorDescObj.Get("layout").ToString();
  auto dims = Napi::Array(env, tensorDescObj.Get("dims"));

  auto meanDataLength = meanDataArray.ByteLength();
  auto dimsLength = dims.Length();
  auto meanDataBuffer = meanDataArray.Data();

  try {
    ie::SizeVector dimsVector = {};
    size_t dimsProduct = 1;
    for (size_t i = 0; i < dimsLength; i++) {
      Napi::Value dimsValue = dims[i];
      dimsVector.push_back(dimsValue.ToNumber().Uint32Value());
      dimsProduct = dimsProduct * dimsValue.ToNumber().Uint32Value();
    }

    ie::Precision precision = utils::GetPrecisionByName(precisionString);
    ie::Layout layout = utils::GetLayoutByName(layoutString);
    ie::TensorDesc tensor(precision, dimsVector, layout);
    ie::Blob::Ptr meanDataBlob;

    switch (precision) {
      case ie::Precision::FP32:
        if ((meanDataLength / sizeof(float)) != dimsProduct) {
          Napi::TypeError::New(env, "Dims does not match the data")
              .ThrowAsJavaScriptException();
          return;
        }
        meanDataBlob = ie::make_shared_blob<float>(
            tensor, reinterpret_cast<float*>(malloc(meanDataLength)),
            meanDataLength);
        memcpy(ie::as<ie::MemoryBlob>(meanDataBlob)->wmap(), meanDataBuffer,
               meanDataLength);
        break;
      case ie::Precision::FP16:
      case ie::Precision::I16:
        if ((meanDataLength / sizeof(int16_t)) != dimsProduct) {
          Napi::TypeError::New(env, "Dims does not match the data")
              .ThrowAsJavaScriptException();
          return;
        }
        meanDataBlob = ie::make_shared_blob<int16_t>(
            tensor, reinterpret_cast<int16_t*>(malloc(meanDataLength)),
            meanDataLength);
        memcpy(ie::as<ie::MemoryBlob>(meanDataBlob)->wmap(), meanDataBuffer,
               meanDataLength);
        break;
      case ie::Precision::U8:
        if ((meanDataLength / sizeof(uint8_t)) != dimsProduct) {
          Napi::TypeError::New(env, "Dims does not match the data")
              .ThrowAsJavaScriptException();
          return;
        }
        meanDataBlob = ie::make_shared_blob<uint8_t>(
            tensor, reinterpret_cast<uint8_t*>(malloc(meanDataLength)),
            meanDataLength);
        memcpy(ie::as<ie::MemoryBlob>(meanDataBlob)->wmap(), meanDataBuffer,
               meanDataLength);
        break;
      default:
        Napi::TypeError::New(env,
                             "Unsupported network precision ! Supported "
                             "precisions are: FP32, FP16, I16, U8")
            .ThrowAsJavaScriptException();
        return;
    }
    meanDataBlob->allocate();
    this->_actual->meanData = meanDataBlob;
  } catch (const std::exception& error) {
    Napi::TypeError::New(env, error.what()).ThrowAsJavaScriptException();
    return;
  } catch (...) {
    Napi::Error::New(env, "Unknown/internal exception happened.")
        .ThrowAsJavaScriptException();
    return;
  }
}

Napi::Object PreProcessChannel::NewInstance(
    const Napi::Env& env,
    const InferenceEngine::PreProcessInfo& preProcessInfo,
    const size_t& index) {
  Napi::EscapableHandleScope scope(env);
  auto obj = constructor.New({});
  auto preProcessChannel = Napi::ObjectWrap<PreProcessChannel>::Unwrap(obj);

  auto numberOfChannels = preProcessInfo.getNumberOfChannels();

  if (index >= numberOfChannels) {
    auto errorMessage = "PreprocessChannel index " + std::to_string(index) +
                        " is out of bounds.";
    Napi::Error::New(env, errorMessage).ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  preProcessChannel->_actual = preProcessInfo[index];
  return scope.Escape(napi_value(obj)).ToObject();
}

}  // namespace ienodejs
