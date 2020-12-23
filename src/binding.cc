#include "napi.h"

#include "blob.h"
#include "core.h"
#include "executable_network.h"
#include "infer_request.h"
#include "input_info.h"
#include "network.h"
#include "output_info.h"
#include "preprocess_info.h"
#include "preprocess_channel.h"

#include "inference_engine.hpp"

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::Object GetVersion(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

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
  Blob::Init(env);
  Core::Init(env, exports);
  Network::Init(env);
  ExecutableNetwork::Init(env);
  InferRequest::Init(env);
  PreProcessInfo::Init(env);
  PreProcessChannel::Init(env);
  InputInfo::Init(env);
  OutputInfo::Init(env);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace ienodejs