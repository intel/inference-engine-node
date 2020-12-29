#include "blob.h"

#include <napi.h>
#include <inference_engine.hpp>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference Blob::constructor;

void Blob::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "Blob",
      {InstanceMethod("byteSize", &Blob::ByteSize),
       InstanceMethod("size", &Blob::Size), InstanceMethod("rmap", &Blob::Rmap),
       InstanceMethod("rwmap", &Blob::Rwmap),
       InstanceMethod("wmap", &Blob::Wmap),
       InstanceMethod("unmap", &Blob::Unmap)});

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

Napi::Value Blob::Memmap(const Napi::CallbackInfo& info, const int mode) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  ie::MemoryBlob::Ptr memory_blob = ie::as<ie::MemoryBlob>(actual_);
  if (!memory_blob) {
    Napi::TypeError::New(env, "Not able to cast Blob to MemoryBlob")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (locked_memory_ != nullptr) {
    Napi::TypeError::New(env, "Already mapped").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (mode == READ)
    locked_memory_.reset(reinterpret_cast<ie::LockedMemory<void>*>(
        new ie::LockedMemory<const void>(memory_blob->rmap())));
  else if (mode == WRITE)
    locked_memory_.reset(new ie::LockedMemory<void>(memory_blob->wmap()));
  else if (mode == READ_WRITE)
    locked_memory_.reset(new ie::LockedMemory<void>(memory_blob->rwmap()));
  else {
    Napi::TypeError::New(env, "Map mode is not supported")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  return Napi::ArrayBuffer::New(
      env,
      locked_memory_->as<ie::PrecisionTrait<ie::Precision::I8>::value_type*>(),
      actual_->byteSize());
}

Napi::Value Blob::Unmap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  locked_memory_.reset();
  return env.Null();
}

Napi::Value Blob::ByteSize(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  return Napi::Number::New(env, actual_->byteSize());
}

Napi::Value Blob::Size(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  return Napi::Number::New(env, actual_->size());
}

}  // namespace ienodejs