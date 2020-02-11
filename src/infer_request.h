#ifndef IE_INFER_REQUEST_H
#define IE_INFER_REQUEST_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class InferRequest : public Napi::ObjectWrap<InferRequest> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Value NewInstance(const Napi::Env& env,
                                 const InferenceEngine::InferRequest& actual);
  InferRequest(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value GetBlob(const Napi::CallbackInfo& info);
  Napi::Value StartAsync(const Napi::CallbackInfo& info);

  InferenceEngine::InferRequest actual_;
};

}  // namespace ienodejs

#endif  // IE_INFER_REQUEST_H