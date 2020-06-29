#include "preprocess_info.h"
#include "blob.h"
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
      {InstanceMethod("init", &PreProcessInfo::Init),
       InstanceMethod("getColorFormat", &PreProcessInfo::GetColorFormat),
       InstanceMethod("setColorFormat", &PreProcessInfo::SetColorFormat),
       InstanceMethod("getResizeAlgorithm", &PreProcessInfo::GetResizeAlgorithm),
       InstanceMethod("setResizeAlgorithm", &PreProcessInfo::SetResizeAlgorithm),
       InstanceMethod("getMeanVariant", &PreProcessInfo::GetMeanVariant),
       InstanceMethod("setVariant", &PreProcessInfo::SetVariant),
       InstanceMethod("getNumberOfChannels", &PreProcessInfo::GetNumberOfChannels),
       InstanceMethod("getPreProcessChannel", &PreProcessInfo::GetPreProcessChannel),
       InstanceMethod("setPreProcessChannel", &PreProcessInfo::SetPreProcessChannel),
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

  _input_info->getPreProcess().setColorFormat(utils::GetColorFormatByName(colorFormatName));
}

Napi::Value PreProcessInfo::GetColorFormat(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfColorFormat(_input_info->getPreProcess().getColorFormat()));
}

Napi::Value PreProcessInfo::GetNumberOfChannels(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::Number::New(
      env, _input_info->getPreProcess().getNumberOfChannels());
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

  _input_info->getPreProcess().setResizeAlgorithm(utils::GetResizeAlgorithmByName(resizeAlgorithm));
}

Napi::Value PreProcessInfo::GetResizeAlgorithm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfResizeAlgorithm(_input_info->getPreProcess().getResizeAlgorithm()));
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

  _input_info->getPreProcess().setVariant(utils::GetMeanVariantByName(meanvariant));
}

Napi::Value PreProcessInfo::GetMeanVariant(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfMeanVariant(_input_info->getPreProcess().getMeanVariant()));
}

Napi::Value PreProcessInfo::GetPreProcessChannel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);;
  }

  size_t index = info[0].ToNumber().Int32Value();
  ie::PreProcessInfo& preInfo = _input_info->getPreProcess();
  
  size_t numberOfChannels = preInfo.getNumberOfChannels();

  if (numberOfChannels == 0) {
    Napi::Error::New(env, "accessing pre-process when nothing was set.")
        .ThrowAsJavaScriptException();
    return env.Null();
  };

  if (index >= numberOfChannels) {
    std::string errorLog = "pre process index " + std::to_string(index) + " is out of bounds.";
    Napi::Error::New(env, errorLog)
        .ThrowAsJavaScriptException();
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
    preprocess_channel.Set("meanData", Napi::ArrayBuffer::ArrayBuffer(env, env.Null()));
    Napi::Value channelInfo = Napi::Value::From(env, preprocess_channel);
  
    return channelInfo;
  }

  locked_memory_.reset(reinterpret_cast<ie::LockedMemory<void>*>(new ie::LockedMemory<const void>(memory_meandata->rmap())));
  Napi::ArrayBuffer buffer = Napi::ArrayBuffer::New(env, locked_memory_->as<ie::PrecisionTrait<ie::Precision::I8>::value_type*>(),meandata->byteSize());

  preprocess_channel.Set("meanData", buffer);
  Napi::Value channelInfo = Napi::Value::From(env, preprocess_channel);
  
  return channelInfo;
}

void PreProcessInfo::SetPreProcessChannel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsNumber()) {
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

  if (index >= numberOfChannels) {
    std::string errorLog = "pre process index " + std::to_string(index) + " is out of bounds.";
    Napi::Error::New(env, errorLog)
        .ThrowAsJavaScriptException();
    return;
  };

  ie::PreProcessChannel::Ptr channel = preInfo[index];
  if (newChannel.Has("stdScale")) {
    channel->stdScale = newChannel.Get("stdScale").ToNumber().FloatValue();
    std::cout<<channel->stdScale<<std::endl;
  }

  if (newChannel.Has("meanValue")) {
    channel->meanValue = newChannel.Get("meanValue").ToNumber().FloatValue();
    std::cout<<channel->meanValue<<std::endl;
  }

  if (newChannel.Has("meanData")) {
    Napi::ArrayBuffer meanDataArray = Napi::ArrayBuffer::ArrayBuffer(env, newChannel.Get("meanData"));
    size_t bytelength = meanDataArray.ByteLength();
    void* buffer = meanDataArray.Data();
    std::cout<<buffer<<std::endl;

    // ie::Blob::Ptr meandata = preProcessChannel->meanData;
    // std::unique_ptr<ie::LockedMemory<void>> locked_memory_;
    // ie::MemoryBlob::Ptr memory_meandata = ie::as<ie::MemoryBlob>(meandata);




    // channel->meanValue = newChannel.Get("meanData");
  }

  return;
}

}  // namespace ienodejs