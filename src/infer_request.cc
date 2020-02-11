#include "infer_request.h"
#include "blob.h"

#include <napi.h>
#include <uv.h>

#include "inference_engine.hpp"

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference InferRequest::constructor;

void InferRequest::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env, "InferRequest",
                  {InstanceMethod("getBlob", &InferRequest::GetBlob),
                   InstanceMethod("startAsync", &InferRequest::StartAsync)});

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

Napi::Value InferRequest::StartAsync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

  deferred.Reject(Napi::TypeError::New(env, "Not implemented").Value());

  return deferred.Promise();
}

}  // namespace ienodejs