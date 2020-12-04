#ifndef IE_NODE_PREPROCESS_INFO_H
#define IE_NODE_PREPROCESS_INFO_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class PreProcessInfo : public Napi::ObjectWrap<PreProcessInfo> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Object NewInstance(const Napi::Env& envs,
                                  InferenceEngine::InputInfo::Ptr actual);
  explicit PreProcessInfo(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs

  void Init(const Napi::CallbackInfo& info);
  void SetColorFormat(const Napi::CallbackInfo& info);
  Napi::Value GetColorFormat(const Napi::CallbackInfo& info);
  void SetResizeAlgorithm(const Napi::CallbackInfo& info);
  Napi::Value GetResizeAlgorithm(const Napi::CallbackInfo& info);
  void SetVariant(const Napi::CallbackInfo& info);
  Napi::Value GetMeanVariant(const Napi::CallbackInfo& info);

  Napi::Value GetPreProcessChannel(const Napi::CallbackInfo& info);
  void SetPreProcessChannel(const Napi::CallbackInfo& info);
  Napi::Value GetNumberOfChannels(const Napi::CallbackInfo& info);

  InferenceEngine::InputInfo::Ptr _input_info;
};

}  // namespace ienodejs

#endif  // IE_NODE_PREPROCESS_INFO_H