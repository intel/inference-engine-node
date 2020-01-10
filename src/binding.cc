#include "napi.h"

#include "network.h"

#include "core.h"

#include "inference_engine.hpp"

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::Object GetVersion(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  const ie::Version* ie_version = ie::GetInferenceEngineVersion();

  Napi::Object version = Napi::Object::New(env);

  Napi::Object api_version = Napi::Object::New(env);
  api_version.Set("major", ie_version->apiVersion.major);
  api_version.Set("minor", ie_version->apiVersion.minor);
  version.Set("apiVersion", api_version);
  
  if (ie_version->buildNumber) {
    version.Set("buildNumber", ie_version->buildNumber);
  }

  if (ie_version->description) {
    version.Set("description", ie_version->description);
  }
  return version;
}

Napi::Value CreateNetwork(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
  Network::NewInstanceAsync(env, info[0], info[1], deferred);
  return deferred.Promise();
}

Napi::Value CreateCore(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Core::NewInstance(env);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("getVersion", Napi::Function::New(env, GetVersion));
  exports.Set("createNetwork", Napi::Function::New(env, CreateNetwork));
  exports.Set("createCore", Napi::Function::New(env, CreateCore));
  Network::Init(env);
  Core::Init(env);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace ienodejs