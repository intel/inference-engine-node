#ifndef IE_NODE_PREPROCESS_INFO_H
#define IE_NODE_PREPROCESS_INFO_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class PreProcessInfo : public Napi::ObjectWrap<PreProcessInfo> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Object NewInstance(
      const Napi::Env& envs,
      const InferenceEngine::PreProcessInfo& actual);
  PreProcessInfo(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  void SetColorFormat(const Napi::CallbackInfo& info);
  Napi::Value GetColorFormat(const Napi::CallbackInfo& info);
  Napi::Value GetNumberOfChannels(const Napi::CallbackInfo& info);
  void SetResizeAlgorithm(const Napi::CallbackInfo& info);
  Napi::Value GetResizeAlgorithm(const Napi::CallbackInfo& info);
  void SetVariant(const Napi::CallbackInfo& info);
  Napi::Value GetMeanVariant(const Napi::CallbackInfo& info);

  InferenceEngine::PreProcessInfo actual_;
  // std::shared_ptr<InferenceEngine::PreProcessInfo> actual_;
};

}  // namespace ienodejs

#endif  // IE_NODE_PREPROCESS_INFO_H