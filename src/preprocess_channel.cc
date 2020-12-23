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