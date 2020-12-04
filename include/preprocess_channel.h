#ifndef IE_NODE_PREPROCESS_CHANNEL_H
#define IE_NODE_PREPROCESS_CHANNEL_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class PreProcessChannel : public Napi::ObjectWrap<PreProcessChannel> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Object NewInstanceAsync(
      const Napi::Env& env,
      const InferenceEngine::PreProcessInfo& preProcessInfo,
      const size_t& index,
      Napi::Promise::Deferred& deferred);
  explicit PreProcessChannel(const Napi::CallbackInfo& info);

 private:

  static Napi::FunctionReference constructor;
  // APIs

  Napi::Value GetSTDScale(const Napi::CallbackInfo& info);
  void SetSTDScale(const Napi::CallbackInfo& info, const Napi::Value& value);
  Napi::Value GetMean(const Napi::CallbackInfo& info);
  void SetMean(const Napi::CallbackInfo& info, const Napi::Value& value);

  InferenceEngine::PreProcessChannel::Ptr _actual;
};

}  // namespace ienodejs

#endif  // IE_NODE_PREPROCESS_CHANNEL_H