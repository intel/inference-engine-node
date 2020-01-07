#include "napi.h"

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

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("getVersion", Napi::Function::New(env, GetVersion));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace ienodejs