#include "input_info.h"
#include "preprocess_info.h"
#include "utils.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference InputInfo::constructor;

void InputInfo::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env, "InputInfo",
                  {
                      InstanceMethod("name", &InputInfo::Name),
                      InstanceMethod("getPrecision", &InputInfo::GetPrecision),
                      InstanceMethod("setPrecision", &InputInfo::SetPrecision),
                      InstanceMethod("getLayout", &InputInfo::GetLayout),
                      InstanceMethod("setLayout", &InputInfo::SetLayout),
                      InstanceMethod("getDims", &InputInfo::GetDims),
                      InstanceMethod("getPreProcess", &InputInfo::GetPreProcess)
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

InputInfo::InputInfo(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<InputInfo>(info) {}

Napi::Object InputInfo::NewInstance(const Napi::Env& env,
                                    const ie::InputInfo::Ptr& actual) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  InputInfo* info = Napi::ObjectWrap<InputInfo>::Unwrap(obj);
  info->actual_ = actual;

  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value InputInfo::Name(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  return Napi::String::New(env, actual_->name());
}

Napi::Value InputInfo::GetPrecision(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  return Napi::String::New(env,
                           utils::GetNameOfPrecision(actual_->getPrecision()));
}

void InputInfo::SetPrecision(const Napi::CallbackInfo& info) {
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

  std::string precision_name = info[0].ToString().Utf8Value();

  if (!utils::IsValidPrecisionName(precision_name)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  actual_->setPrecision(utils::GetPrecisionByName(precision_name));
}

Napi::Value InputInfo::GetLayout(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  return Napi::String::New(env, utils::GetNameOfLayout(actual_->getLayout()));
}

void InputInfo::SetLayout(const Napi::CallbackInfo& info) {
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

  std::string layout_name = info[0].ToString().Utf8Value();

  if (!utils::IsValidLayoutName(layout_name)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  actual_->setLayout(utils::GetLayoutByName(layout_name));
}

Napi::Value InputInfo::GetDims(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  ie::SizeVector ie_dims = actual_->getTensorDesc().getDims();
  Napi::Array js_dims = Napi::Array::New(env, ie_dims.size());
  for (size_t i = 0; i < ie_dims.size(); ++i) {
    js_dims[i] = ie_dims[i];
  }
  return js_dims;
}

Napi::Value InputInfo::GetPreProcess(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return PreProcessInfo::NewInstance(env, actual_->getPreProcess());
}

}  // namespace ienodejs
