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
                                    &PreProcessChannel::SetSTDScale)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Napi::Value PreProcessChannel::GetSTDScale(const Napi::CallbackInfo& info) {
  auto stdScale = this->_actual->stdScale;
  return Napi::Number::New(info.Env(), stdScale);
}

void PreProcessChannel::SetSTDScale(const Napi::CallbackInfo& info,
                                    const Napi::Value& value) {
  auto arg = value.As<Napi::Number>();
  this->_actual->stdScale = arg.FloatValue();
}

Napi::Value PreProcessChannel::GetMean(const Napi::CallbackInfo& info) {
  auto meanValue = this->_actual->meanValue;
  return Napi::Number::New(info.Env(), meanValue);
}

void PreProcessChannel::SetMean(const Napi::CallbackInfo& info,
                                const Napi::Value& value) {
  auto arg = value.As<Napi::Number>();
  this->_actual->meanValue = arg.FloatValue();
}

Napi::Object PreProcessChannel::NewInstanceAsync(
    const Napi::Env& env,
    const InferenceEngine::PreProcessInfo& preProcessInfo,
    const size_t& index,
    Napi::Promise::Deferred& deferred) {
  Napi::EscapableHandleScope scope(env);
  auto obj = constructor.New({});
  auto preProcessChannel = Napi::ObjectWrap<PreProcessChannel>::Unwrap(obj);
  preProcessChannel->_actual = preProcessInfo[index];
  return scope.Escape(napi_value(obj)).ToObject();
}

}  // namespace ienodejs