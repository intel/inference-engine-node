#ifndef IE_EXECNETWORK_H
#define IE_EXECNETWORK_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class ExecutableNetwork : public Napi::ObjectWrap<ExecutableNetwork> {
 public:
  static void Init(const Napi::Env& env);
  static void NewInstanceAsync(Napi::Env& env,
                               const Napi::Value& network,
                               const Napi::Value& dev_name,
                               InferenceEngine::Core core,
                               Napi::Promise::Deferred& deferred);
  ExecutableNetwork(const Napi::CallbackInfo& info);

 private:
  friend class ExecnetworkAsyncWorker;

  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value GetName(const Napi::CallbackInfo& info);

  InferenceEngine::ExecutableNetwork actual_;

  std::string name;
};

}  // namespace ienodejs

#endif  // IE_EXECNETWORK_H