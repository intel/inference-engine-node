#ifndef IE_NODE_CORE_H
#define IE_NODE_CORE_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class Core : public Napi::ObjectWrap<Core> {
 public:
  static void Init(const Napi::Env& env, Napi::Object exports);
  static Napi::Object NewInstance(const Napi::Env& env);
  Core(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value GetVersions(const Napi::CallbackInfo& info);
  Napi::Value ReadNetwork(const Napi::CallbackInfo& info);
  Napi::Value ReadNetworkFromData(const Napi::CallbackInfo& info);
  Napi::Value LoadNetwork(const Napi::CallbackInfo& info);
  Napi::Value GetAvailableDevices(const Napi::CallbackInfo& info);
  Napi::Value toStringTag(const Napi::CallbackInfo& info);

  InferenceEngine::Core actual_;
};

}  // namespace ienodejs

#endif  // IE_NODE_CORE_H