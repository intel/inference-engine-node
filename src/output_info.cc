#include "output_info.h"
#include "utils.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference OutputInfo::constructor;

void OutputInfo::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "OutputInfo",
      {InstanceMethod("name", &OutputInfo::Name),
       InstanceMethod("getPrecision", &OutputInfo::GetPrecision),
       InstanceMethod("setPrecision", &OutputInfo::GetPrecision),
       InstanceMethod("getLayout", &OutputInfo::GetLayout),
       InstanceMethod("getDims", &OutputInfo::GetDims),
      });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

OutputInfo::OutputInfo(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<OutputInfo>(info) {}

Napi::Object OutputInfo::NewInstance(const Napi::Env& env, const ie::DataPtr& actual) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  OutputInfo* info = Napi::ObjectWrap<OutputInfo>::Unwrap(obj);
  info->actual_ = actual;
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value OutputInfo::Name(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, actual_->getName());
}

Napi::Value OutputInfo::GetPrecision(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, utils::GetNameOfPrecision(actual_->getPrecision()));
}

void OutputInfo::SetPrecision(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments").ThrowAsJavaScriptException();
    return;
  }

  std::string precision_name = info[0].ToString().Utf8Value();

  if (!utils::IsValidPrecisionName(precision_name)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  actual_->setPrecision(utils::GetPrecisionByName(precision_name));
}

Napi::Value OutputInfo::GetLayout(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, utils::GetNameOfLayout(actual_->getLayout()));
}

Napi::Value OutputInfo::GetDims(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  ie::SizeVector ie_dims = actual_->getTensorDesc().getDims();
  Napi::Array js_dims = Napi::Array::New(env, ie_dims.size());
  for (size_t i = 0; i < ie_dims.size(); ++i) {
    js_dims[i] = ie_dims[i];
  }
  return js_dims;
}

}
