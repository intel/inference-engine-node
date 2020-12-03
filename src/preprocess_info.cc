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
  PreProcessInfo* pre_info = Napi::ObjectWrap<PreProcessInfo>::Unwrap(obj);
  pre_info->_input_info = actual;

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
  ie::PreProcessInfo& pre_info = _input_info->getPreProcess();

  size_t number_of_channels = pre_info.getNumberOfChannels();

  if (number_of_channels == 0) {
    Napi::Error::New(env, "accessing pre-process when nothing was set.")
        .ThrowAsJavaScriptException();
    return env.Null();
  };

  if (index >= number_of_channels || index < 0) {
    std::string errorLog =
        "pre process index " + std::to_string(index) + " is out of bounds.";
    Napi::Error::New(env, errorLog).ThrowAsJavaScriptException();
    return env.Null();
  };

  const ie::PreProcessChannel::Ptr& preProcessChannel = pre_info[index];
  Napi::Object preprocess_channel = Napi::Object::New(env);
  preprocess_channel.Set("stdScale", preProcessChannel->stdScale);
  preprocess_channel.Set("meanValue", preProcessChannel->meanValue);

  ie::Blob::Ptr mean_data = preProcessChannel->meanData;
  std::unique_ptr<ie::LockedMemory<void>> locked_memory_;
  ie::MemoryBlob::Ptr memory_mean_data = ie::as<ie::MemoryBlob>(mean_data);
  if (!memory_mean_data) {
    preprocess_channel.Set("meanData", env.Null());
    Napi::Value channelInfo = Napi::Value::From(env, preprocess_channel);
    return channelInfo;
  }
  ie::LockedMemory<const void> local_memory = memory_mean_data->rmap();
  Napi::ArrayBuffer buffer = Napi::ArrayBuffer::New(env, mean_data->byteSize());
  void* data = buffer.Data();
  memcpy(data, local_memory, mean_data->byteSize());
  preprocess_channel.Set("meanData", buffer);
  Napi::Value channelInfo = Napi::Value::From(env, preprocess_channel);

  return channelInfo;
}

bool checkDesc(Napi::Object desc) {
  if (desc.Has("precision") && desc.Has("dims") && desc.Has("layout")) {
    if (desc.Get("precision").IsString() && desc.Get("layout").IsString() &&
        desc.Get("dims").IsArray()) {
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
  Napi::Object new_channel = info[1].ToObject();

  ie::PreProcessInfo& pre_info = _input_info->getPreProcess();

  size_t number_of_channels = pre_info.getNumberOfChannels();

  if (number_of_channels == 0) {
    Napi::Error::New(env, "accessing pre-process when nothing was set.")
        .ThrowAsJavaScriptException();
    return;
  };

  if (index >= number_of_channels || index < 0) {
    std::string errorLog =
        "pre process index " + std::to_string(index) + " is out of bounds.";
    Napi::Error::New(env, errorLog).ThrowAsJavaScriptException();
    return;
  };

  ie::PreProcessChannel::Ptr channel = pre_info[index];
  if (new_channel.Has("stdScale")) {
    if (new_channel.Get("stdScale").IsNumber()) {
      channel->stdScale = new_channel.Get("stdScale").ToNumber().FloatValue();
    } else {
      Napi::TypeError::New(env, "Wrong stdScale").ThrowAsJavaScriptException();
    }
  }

  if (new_channel.Has("meanValue")) {
    if (new_channel.Get("meanValue").IsNumber()) {
      channel->meanValue = new_channel.Get("meanValue").ToNumber().FloatValue();
    } else {
      Napi::TypeError::New(env, "Wrong meanValue").ThrowAsJavaScriptException();
    }
  }

  if (new_channel.Has("meanData")) {
    if (!new_channel.Get("meanData").IsObject()) {
      Napi::TypeError::New(env, "Wrong meanData").ThrowAsJavaScriptException();
      return;
    }

    Napi::Object mean_data = new_channel.Get("meanData").ToObject();

    if (!mean_data.Has("desc") || !mean_data.Has("data")) {
      Napi::TypeError::New(env, "Wrong meanData").ThrowAsJavaScriptException();
      return;
    }

    if (!mean_data.Get("desc").IsObject()) {
      Napi::TypeError::New(env, "Wrong desc").ThrowAsJavaScriptException();
      return;
    }

    if (!mean_data.Get("data").IsArrayBuffer()) {
      Napi::TypeError::New(env, "Wrong data").ThrowAsJavaScriptException();
      return;
    }

    Napi::Object desc = mean_data.Get("desc").ToObject();
    if (!checkDesc(desc)) {
      Napi::TypeError::New(env, "Wrong desc").ThrowAsJavaScriptException();
      return;
    }

    Napi::ArrayBuffer data =
        Napi::ArrayBuffer(env, mean_data.Get("data"));
    std::string precision_string = desc.Get("precision").ToString();
    std::string layout_string = desc.Get("layout").ToString();
    Napi::Array dims = Napi::Array(env, desc.Get("dims"));

    size_t data_length = data.ByteLength();
    size_t dims_length = dims.Length();
    void* buffer = data.Data();

    try {
      ie::SizeVector dims_vector = {};
      size_t dims_product = 1;
      for (size_t i = 0; i < dims_length; i++) {
        Napi::Value dims_value = dims[i];
        dims_vector.push_back(dims_value.ToNumber().Uint32Value());
        dims_product = dims_product * dims_value.ToNumber().Uint32Value();
      }
      ie::Precision precision = utils::GetPrecisionByName(precision_string);
      ie::Layout layout = utils::GetLayoutByName(layout_string);
      ie::TensorDesc tensor(precision, dims_vector, layout);
      ie::Blob::Ptr mean_data_blob;
      switch (precision) {
        case ie::Precision::FP32:
          if ((data_length / sizeof(float)) != dims_product) {
            Napi::TypeError::New(env, "Dims does not match the data")
                .ThrowAsJavaScriptException();
            return;
          }
          mean_data_blob = ie::make_shared_blob<float>(
              tensor, reinterpret_cast<float*>(malloc(data_length)),
              data_length);
          memcpy(ie::as<ie::MemoryBlob>(mean_data_blob)->wmap(), buffer,
                 data_length);
          break;
        case ie::Precision::FP16:
        case ie::Precision::I16:
          if ((data_length / sizeof(int16_t)) != dims_product) {
            Napi::TypeError::New(env, "Dims does not match the data")
                .ThrowAsJavaScriptException();
            return;
          }
          mean_data_blob = ie::make_shared_blob<int16_t>(
              tensor, reinterpret_cast<int16_t*>(malloc(data_length)),
              data_length);
          memcpy(ie::as<ie::MemoryBlob>(mean_data_blob)->wmap(), buffer,
                 data_length);
          break;
        case ie::Precision::U8:
          if ((data_length / sizeof(uint8_t)) != dims_product) {
            Napi::TypeError::New(env, "Dims does not match the data")
                .ThrowAsJavaScriptException();
            return;
          }
          mean_data_blob = ie::make_shared_blob<uint8_t>(
              tensor, reinterpret_cast<uint8_t*>(malloc(data_length)),
              data_length);
          memcpy(ie::as<ie::MemoryBlob>(mean_data_blob)->wmap(), buffer,
                 data_length);
          break;
        default:
          Napi::TypeError::New(env,
                               "Unsupported network precision ! Supported "
                               "precisions are: FP32, FP16, I16, U8")
              .ThrowAsJavaScriptException();
          return;
      }
      mean_data_blob->allocate();
      channel->meanData = mean_data_blob;
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