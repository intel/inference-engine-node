#include "infer_request.h"
#include "blob.h"

#include <napi.h>
#include <uv.h>

#include "inference_engine.hpp"

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

class InferAsyncWorker : public Napi::AsyncWorker {
 public:
  InferAsyncWorker(Napi::Env& env,
                   const ie::InferRequest& infer_request,
                   Napi::Promise::Deferred& deferred)
      : Napi::AsyncWorker(env),
        infer_request_(infer_request),
        env_(env),
        deferred_(deferred) {}

  ~InferAsyncWorker() {}

  void Execute() {
    try {
      infer_request_.Infer();
    } catch (const std::exception& error) {
      Napi::AsyncWorker::SetError(error.what());
      return;
    } catch (...) {
      Napi::AsyncWorker::SetError("Unknown/internal exception happened.");
      return;
    }
  }

  void OnOK() { deferred_.Resolve(Env().Null()); }

  void OnError(Napi::Error const& error) { deferred_.Reject(error.Value()); }

 private:
  InferenceEngine::InferRequest infer_request_;
  Napi::Env env_;
  Napi::Promise::Deferred deferred_;
};

Napi::FunctionReference InferRequest::constructor;

void InferRequest::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "InferRequest",
      {InstanceMethod("getBlob", &InferRequest::GetBlob),
       InstanceMethod("infer", &InferRequest::Infer),
       InstanceMethod("startAsync", &InferRequest::StartAsync),
       InstanceMethod("setCompletionCallback",
                      &InferRequest::SetCompletionCallback),
       InstanceMethod("nativeStartAsync", &InferRequest::NativeStartAsync)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

InferRequest::InferRequest(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<InferRequest>(info) {}

Napi::Value InferRequest::NewInstance(const Napi::Env& env,
                                      const ie::InferRequest& actual) {
  Napi::EscapableHandleScope scope(env);

  Napi::Object obj = constructor.New({});
  InferRequest* infer_Request = Napi::ObjectWrap<InferRequest>::Unwrap(obj);
  infer_Request->actual_ = actual;

  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value InferRequest::GetBlob(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string name = info[0].ToString();
  try {
    ie::Blob::Ptr blob = actual_.GetBlob(name);
    return Blob::NewInstance(env, blob);
  } catch (const std::exception& error) {
    Napi::RangeError::New(env, error.what()).ThrowAsJavaScriptException();
    return env.Null();
  } catch (...) {
    Napi::Error::New(env, "Unknown/internal exception happened.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value InferRequest::Infer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  try {
    actual_.Infer();
  } catch (const std::exception& error) {
    Napi::Error::New(env, error.what()).ThrowAsJavaScriptException();
    return env.Null();
  } catch (...) {
    Napi::Error::New(env, "Unknown/internal exception happened.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  return env.Null();
}

Napi::Value InferRequest::StartAsync(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto deferred = Napi::Promise::Deferred::New(env);
  if (info.Length() > 0) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong number of arguments").Value());
    return deferred.Promise();
  }

  auto infer_worker = new InferAsyncWorker(env, actual_, deferred);
  infer_worker->Queue();

  return deferred.Promise();
}

void InferRequest::SetCompletionCallback(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  this->_threadSafeFunction = Napi::ThreadSafeFunction::New(
      env, info[0].As<Napi::Function>(), "CompletionCallback", 0, 1,
      [](Napi::Env) {});

  actual_.SetCompletionCallback([this]() {
    auto callback = [](Napi::Env env, Napi::Function jsCallback) {
      jsCallback.Call({});
    };
    this->_threadSafeFunction.BlockingCall(callback);
    this->_threadSafeFunction.Release();
  });
}

void InferRequest::NativeStartAsync(const Napi::CallbackInfo& info) {
  actual_.StartAsync();
}
}  // namespace ienodejs