#include "preprocess_info.h"
#include "utils.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference PreProcessInfo::constructor;

void PreProcessInfo::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "PreProcessInfo",
      {
          InstanceMethod("init", &PreProcessInfo::Init),
          InstanceMethod("getColorFormat", &PreProcessInfo::GetColorFormat),
          InstanceMethod("setColorFormat", &PreProcessInfo::SetColorFormat),
          InstanceMethod("getResizeAlgorithm",
                         &PreProcessInfo::GetResizeAlgorithm),
          InstanceMethod("setResizeAlgorithm",
                         &PreProcessInfo::SetResizeAlgorithm),
          InstanceMethod("getMeanVariant", &PreProcessInfo::GetMeanVariant),
          InstanceMethod("setVariant", &PreProcessInfo::SetVariant),
          InstanceMethod("getNumberOfChannels",
                         &PreProcessInfo::GetNumberOfChannels),
          InstanceMethod("getPreProcessChannel",
                         &PreProcessInfo::GetPreProcessChannel),
          InstanceMethod("setPreProcessChannel",
                         &PreProcessInfo::SetPreProcessChannel),
      });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

PreProcessInfo::PreProcessInfo(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PreProcessInfo>(info) {}

Napi::Object PreProcessInfo::NewInstance(const Napi::Env& env,
                                         ie::InputInfo::Ptr actual) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  PreProcessInfo* preInfo = Napi::ObjectWrap<PreProcessInfo>::Unwrap(obj);
  preInfo->_input_info = actual;

  return scope.Escape(napi_value(obj)).ToObject();
}

void PreProcessInfo::Init(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  size_t numOfChannels = info[0].ToNumber().Int32Value();

  _input_info->getPreProcess().init(numOfChannels);

  return;
}

void PreProcessInfo::SetColorFormat(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string colorFormatName = info[0].ToString().Utf8Value();

  if (!utils::IsColorFormatName(colorFormatName)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  _input_info->getPreProcess().setColorFormat(
      utils::GetColorFormatByName(colorFormatName));
}

Napi::Value PreProcessInfo::GetColorFormat(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(env,
                           utils::GetNameOfColorFormat(
                               _input_info->getPreProcess().getColorFormat()));
}

Napi::Value PreProcessInfo::GetNumberOfChannels(
    const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::Number::New(env,
                           _input_info->getPreProcess().getNumberOfChannels());
}

void PreProcessInfo::SetResizeAlgorithm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string resizeAlgorithm = info[0].ToString().Utf8Value();

  if (!utils::IsResizeAlgorithmName(resizeAlgorithm)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  _input_info->getPreProcess().setResizeAlgorithm(
      utils::GetResizeAlgorithmByName(resizeAlgorithm));
}

Napi::Value PreProcessInfo::GetResizeAlgorithm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfResizeAlgorithm(
               _input_info->getPreProcess().getResizeAlgorithm()));
}

void PreProcessInfo::SetVariant(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string meanvariant = info[0].ToString().Utf8Value();

  if (!utils::IsMeanVariantName(meanvariant)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  _input_info->getPreProcess().setVariant(
      utils::GetMeanVariantByName(meanvariant));
}

Napi::Value PreProcessInfo::GetMeanVariant(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(env,
                           utils::GetNameOfMeanVariant(
                               _input_info->getPreProcess().getMeanVariant()));
}

Napi::Value PreProcessInfo::GetPreProcessChannel(
    const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);
    ;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);
    ;
  }

  size_t index = info[0].ToNumber().Int32Value();
  ie::PreProcessInfo& preInfo = _input_info->getPreProcess();

  size_t numberOfChannels = preInfo.getNumberOfChannels();

  if (numberOfChannels == 0) {
    Napi::Error::New(env, "accessing pre-process when nothing was set.")
        .ThrowAsJavaScriptException();
    return env.Null();
  };

  if (index >= numberOfChannels || index < 0) {
    std::string errorLog =
        "pre process index " + std::to_string(index) + " is out of bounds.";
    Napi::Error::New(env, errorLog).ThrowAsJavaScriptException();
    return env.Null();
  };

  const ie::PreProcessChannel::Ptr& preProcessChannel = preInfo[index];
  Napi::Object preprocess_channel = Napi::Object::New(env);
  preprocess_channel.Set("stdScale", preProcessChannel->stdScale);
  preprocess_channel.Set("meanValue", preProcessChannel->meanValue);

  ie::Blob::Ptr meandata = preProcessChannel->meanData;
  std::unique_ptr<ie::LockedMemory<void>> locked_memory_;
  ie::MemoryBlob::Ptr memory_meandata = ie::as<ie::MemoryBlob>(meandata);
  if (!memory_meandata) {
    preprocess_channel.Set("meanData", env.Null());
    Napi::Value channelInfo = Napi::Value::From(env, preprocess_channel);
    return channelInfo;
  }

  locked_memory_.reset(reinterpret_cast<ie::LockedMemory<void>*>(
      new ie::LockedMemory<const void>(memory_meandata->rmap())));
  Napi::ArrayBuffer buffer = Napi::ArrayBuffer::New(
      env,
      locked_memory_->as<ie::PrecisionTrait<ie::Precision::I8>::value_type*>(),
      meandata->byteSize());

  preprocess_channel.Set("meanData", buffer);
  Napi::Value channelInfo = Napi::Value::From(env, preprocess_channel);

  return channelInfo;
}

bool checkDesc(Napi::Object desc) {
  if (desc.Has("precision") && desc.Has("dims") && desc.Has("layout")) {
    if (desc.Get("precision").IsString() && desc.Get("layout").IsString() &&
        desc.Get("dims").IsArrayBuffer()) {
      std::string precision = desc.Get("precision").ToString();
      std::string layout = desc.Get("layout").ToString();
      if (utils::IsValidPrecisionName(precision) &&
          utils::IsValidLayoutName(layout)) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void PreProcessInfo::SetPreProcessChannel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  size_t index = info[0].ToNumber().Int32Value();
  Napi::Object newChannel = info[1].ToObject();

  ie::PreProcessInfo& preInfo = _input_info->getPreProcess();

  size_t numberOfChannels = preInfo.getNumberOfChannels();

  if (numberOfChannels == 0) {
    Napi::Error::New(env, "accessing pre-process when nothing was set.")
        .ThrowAsJavaScriptException();
    return;
  };

  if (index >= numberOfChannels || index < 0) {
    std::string errorLog =
        "pre process index " + std::to_string(index) + " is out of bounds.";
    Napi::Error::New(env, errorLog).ThrowAsJavaScriptException();
    return;
  };

  ie::PreProcessChannel::Ptr channel = preInfo[index];
  if (newChannel.Has("stdScale")) {
    if (newChannel.Get("stdScale").IsNumber()) {
      channel->stdScale = newChannel.Get("stdScale").ToNumber().FloatValue();
    } else {
      Napi::TypeError::New(env, "Wrong stdScale").ThrowAsJavaScriptException();
    }
  }

  if (newChannel.Has("meanValue")) {
    if (newChannel.Get("meanValue").IsNumber()) {
      channel->meanValue = newChannel.Get("meanValue").ToNumber().FloatValue();
    } else {
      Napi::TypeError::New(env, "Wrong meanValue").ThrowAsJavaScriptException();
    }
  }

  if (newChannel.Has("meanData")) {
    if (!newChannel.Get("meanData").IsObject()) {
      Napi::TypeError::New(env, "Wrong meanData").ThrowAsJavaScriptException();
      return;
    }

    Napi::Object meanData = newChannel.Get("meanData").ToObject();

    if (!meanData.Has("desc") || !meanData.Has("data")) {
      Napi::TypeError::New(env, "Wrong meanData").ThrowAsJavaScriptException();
      return;
    }

    if (!meanData.Get("desc").IsObject()) {
      Napi::TypeError::New(env, "Wrong desc").ThrowAsJavaScriptException();
      return;
    }

    if (!meanData.Get("data").IsArrayBuffer()) {
      Napi::TypeError::New(env, "Wrong data").ThrowAsJavaScriptException();
      return;
    }

    Napi::Object desc = meanData.Get("desc").ToObject();
    if (!checkDesc(desc)) {
      Napi::TypeError::New(env, "Wrong desc").ThrowAsJavaScriptException();
      return;
    }

    Napi::ArrayBuffer data =
        Napi::ArrayBuffer::ArrayBuffer(env, meanData.Get("data"));
    std::string precision_string = desc.Get("precision").ToString();
    std::string layout_string = desc.Get("layout").ToString();
    Napi::ArrayBuffer dims =
        Napi::ArrayBuffer::ArrayBuffer(env, desc.Get("dims"));

    size_t datalength = data.ByteLength();
    size_t dimslength = dims.ByteLength() / sizeof(uint32_t);
    const uint32_t* dimArray = reinterpret_cast<uint32_t*>(dims.Data());
    void* buffer = data.Data();

    try {
      ie::SizeVector dims_vector = {};
      for (size_t i = 0; i < dimslength; i++) {
        dims_vector.push_back(dimArray[i]);
      }
      ie::Precision precision = utils::GetPrecisionByName(precision_string);
      ie::Layout layout = utils::GetLayoutByName(layout_string);
      ie::TensorDesc tensor(precision, dims_vector, layout);
      ie::Blob::Ptr meanDataBlob;
      switch (precision) {
        case ie::Precision::FP32:
          meanDataBlob = ie::make_shared_blob<float>(
              tensor, reinterpret_cast<float_t*>(buffer), datalength);
          break;
        case ie::Precision::FP16:
        case ie::Precision::I16:
          meanDataBlob = ie::make_shared_blob<int16_t>(
              tensor, reinterpret_cast<int16_t*>(buffer), datalength);
          break;
        case ie::Precision::U8:
          meanDataBlob = ie::make_shared_blob<uint8_t>(
              tensor, reinterpret_cast<uint8_t*>(buffer), datalength);
          break;
        default:
          Napi::TypeError::New(env,
                               "Unsupported network precision ! Supported "
                               "precisions are: FP32, FP16, I16, U8")
              .ThrowAsJavaScriptException();
          return;
      }
      meanDataBlob->allocate();
      channel->meanData = meanDataBlob;
    } catch (const std::exception& error) {
      Napi::TypeError::New(env, error.what()).ThrowAsJavaScriptException();
      return;
    } catch (...) {
      Napi::Error::New(env, "Unknown/internal exception happened.")
          .ThrowAsJavaScriptException();
      return;
    }
  }

  return;
}

}  // namespace ienodejs