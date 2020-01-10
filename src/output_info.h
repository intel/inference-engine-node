#ifndef IE_NODE_OutputInfo_H
#define IE_NODE_OutputInfo_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class OutputInfo : public Napi::ObjectWrap<OutputInfo> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Object NewInstance(const Napi::Env& env, const InferenceEngine::DataPtr& actual);
  OutputInfo(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value Name(const Napi::CallbackInfo& info);
  Napi::Value GetPrecision(const Napi::CallbackInfo& info);
  void SetPrecision(const Napi::CallbackInfo& info);
  Napi::Value GetLayout(const Napi::CallbackInfo& info);
  Napi::Value GetDims(const Napi::CallbackInfo& info);

  InferenceEngine::DataPtr actual_;
};

}  // namespace ienodejs

#endif  // IE_NODE_OUTPUT_INFO_H