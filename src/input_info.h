#ifndef IE_NODE_INPUT_INFO_H
#define IE_NODE_INPUT_INFO_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class InputInfo : public Napi::ObjectWrap<InputInfo> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Object NewInstance(
      const Napi::Env& env,
      const InferenceEngine::InputInfo::Ptr& actual);
  InputInfo(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value Name(const Napi::CallbackInfo& info);
  Napi::Value GetPrecision(const Napi::CallbackInfo& info);
  void SetPrecision(const Napi::CallbackInfo& info);
  Napi::Value GetLayout(const Napi::CallbackInfo& info);
  void SetLayout(const Napi::CallbackInfo& info);
  Napi::Value GetDims(const Napi::CallbackInfo& info);
  Napi::Value GetPreProcess(const Napi::CallbackInfo& info);

  InferenceEngine::InputInfo::Ptr actual_;
};

}  // namespace ienodejs

#endif  // InputInfo_H