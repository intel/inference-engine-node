#include "input_info.h"
#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

std::map<ie::Precision, std::string> precision_name_map = { {ie::Precision::FP32, "fp32"},
                                                            {ie::Precision::FP16, "fp16"},
                                                            {ie::Precision::Q78,  "q87"},
                                                            {ie::Precision::I32,  "i32"},
                                                            {ie::Precision::I16,  "i16"},
                                                            {ie::Precision::I8,   "i8"},
                                                            {ie::Precision::U16,  "u16"},
                                                            {ie::Precision::U8,   "u8"} };

std::map<std::string, ie::Precision> precision_type_map = { {"fp32", ie::Precision::FP32},
                                                            {"fp16", ie::Precision::FP16},
                                                            {"q78",  ie::Precision::Q78},
                                                            {"i32",  ie::Precision::I32},
                                                            {"i16",  ie::Precision::I16},
                                                            {"i8",   ie::Precision::I8},
                                                            {"i16",  ie::Precision::U16},
                                                            {"u8",   ie::Precision::U8} };

std::map<ie::Layout, std::string> layout_name_map = { {ie::Layout::ANY,     "any"},
                                                      {ie::Layout::NCHW,    "nchw"},
                                                      {ie::Layout::NHWC,    "nhwc"},
                                                      {ie::Layout::OIHW,    "oihw"},
                                                      {ie::Layout::C,       "c"},
                                                      {ie::Layout::CHW,     "chw"},
                                                      {ie::Layout::HW,      "hw"},
                                                      {ie::Layout::NC,      "nc"},
                                                      {ie::Layout::CN,      "cn"},
                                                      {ie::Layout::NCDHW,   "ncdhw"},
                                                      {ie::Layout::BLOCKED, "blocked"} };


std::map<std::string, ie::Layout> layout_type_map = { {"any",     ie::Layout::ANY},
                                                      {"nchw",    ie::Layout::NCHW},
                                                      {"nhwc",    ie::Layout::NHWC},
                                                      {"oihw",    ie::Layout::OIHW},
                                                      {"c",       ie::Layout::C},
                                                      {"chw",     ie::Layout::CHW},
                                                      {"hw",      ie::Layout::HW},
                                                      {"nc",      ie::Layout::NC},
                                                      {"cn",      ie::Layout::CN},
                                                      {"ncdhw",   ie::Layout::NCDHW},
                                                      {"blocked", ie::Layout::BLOCKED} };

Napi::FunctionReference InputInfo::constructor;

void InputInfo::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "InputInfo",
      {InstanceMethod("name", &InputInfo::Name),
       InstanceMethod("getPrecision", &InputInfo::GetPrecision),
       InstanceMethod("setPrecision", &InputInfo::GetPrecision),
       InstanceMethod("getLayout", &InputInfo::GetLayout),
       InstanceMethod("setLayout", &InputInfo::SetLayout),
       InstanceMethod("getDims", &InputInfo::GetDims),
      });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

InputInfo::InputInfo(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<InputInfo>(info) {}

Napi::Object InputInfo::NewInstance(const Napi::Env& env, const ie::InputInfo::Ptr& actual) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  InputInfo* info = Napi::ObjectWrap<InputInfo>::Unwrap(obj);
  info->actual_ = actual;
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value InputInfo::Name(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, actual_->name());
}

Napi::Value InputInfo::GetPrecision(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, precision_name_map[actual_->getPrecision()]);
}

Napi::Value InputInfo::SetPrecision(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string precision = info[0].ToString().Utf8Value();

  actual_->setPrecision(precision_type_map[precision]);

  return env.Null();
}

Napi::Value InputInfo::GetLayout(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, layout_name_map[actual_->getLayout()]);
}

Napi::Value InputInfo::SetLayout(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string layout = info[0].ToString().Utf8Value();

  actual_->setLayout(layout_type_map[layout]);

  return env.Null();
}

Napi::Value InputInfo::GetDims(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  ie::SizeVector ie_dims = actual_->getTensorDesc().getDims();
  Napi::Array js_dims = Napi::Array::New(env, ie_dims.size());
  for (size_t i = 0; i < ie_dims.size(); ++i) {
    js_dims[i] = ie_dims[i];
  }
  return js_dims;
}

}
