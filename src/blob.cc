#include "blob.h"
#include "infer_request.h"

#include <napi.h>
#include <uv.h>

#include "inference_engine.hpp"

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference Blob::constructor;

void Blob::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env, "Blob",
                  {InstanceMethod("buffer", &Blob::Buffer),
                   InstanceMethod("byteSize", &Blob::ByteSize),
                   InstanceMethod("size", &Blob::Size)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Blob::Blob(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Blob>(info) {}

Napi::Value Blob::NewInstance(const Napi::Env& env,
                              const ie::Blob::Ptr& actual) {
  Napi::EscapableHandleScope scope(env);

  Napi::Object obj = constructor.New({});
  Blob* blob = Napi::ObjectWrap<Blob>::Unwrap(obj);
  blob->actual_ = actual;

  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value Blob::Buffer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::ArrayBuffer::New(env, actual_->buffer(), actual_->byteSize());
}

Napi::Value Blob::ByteSize(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::Number::New(env, actual_->byteSize());
}

Napi::Value Blob::Size(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::Number::New(env, actual_->size());
}

}  // namespace ienodejs