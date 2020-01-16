#ifndef IE_NODE_NETWORK_H
#define IE_NODE_NETWORK_H

#include <napi.h>
#include "core.h"

#include "inference_engine.hpp"

namespace ienodejs {

class Network : public Napi::ObjectWrap<Network> {
 public:
  static void Init(const Napi::Env& env);
  static void NewInstanceAsync(Napi::Env env,
                               const Napi::Value& model,
                               const Napi::Value& weights,
                               Napi::Promise::Deferred& deferred);
  Network(const Napi::CallbackInfo& info);

 private:
  friend class Core;
  friend class NetworkAsyncWorker;
  friend class ExecnetworkAsyncWorker;

  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value GetName(const Napi::CallbackInfo& info);
  Napi::Value GetInputsInfo(const Napi::CallbackInfo& info);
  Napi::Value GetOutputsInfo(const Napi::CallbackInfo& info);

  std::string model_;
  std::string weights_;
  InferenceEngine::CNNNetwork actual_;
};

}  // namespace ienodejs

#endif  // IE_NODE_NETWORK_H