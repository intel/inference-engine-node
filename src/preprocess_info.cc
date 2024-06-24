#include "preprocess_info.h"
#include "preprocess_channel.h"
#include "utils.h"

#include <core.h>
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
          InstanceAccessor(Napi::Symbol::WellKnown(env, "toStringTag"),
                           &PreProcessInfo::toStringTag, nullptr,
                           napi_enumerable),
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
  auto env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  size_t index = info[0].ToNumber().Int32Value();
  ie::PreProcessInfo& pre_info = _input_info->getPreProcess();
  return PreProcessChannel::NewInstance(env, pre_info, index);
}

Napi::Value PreProcessInfo::toStringTag(const Napi::CallbackInfo& info) {
  return Napi::String::From(info.Env(), "PreProcessInfo");
}

}  // namespace ienodejs
