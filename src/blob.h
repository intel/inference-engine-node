#ifndef IE_BLOB_H
#define IE_BLOB_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class Blob : public Napi::ObjectWrap<Blob> {
 public:
  static void Init(const Napi::Env& env);
  static Napi::Value NewInstance(const Napi::Env& env,
                                 const InferenceEngine::Blob::Ptr& actual);
  Blob(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value ByteSize(const Napi::CallbackInfo& info);
  Napi::Value Buffer(const Napi::CallbackInfo& info);
  Napi::Value Size(const Napi::CallbackInfo& info);

  InferenceEngine::Blob::Ptr actual_;
};

}  // namespace ienodejs

#endif  // IE_BLOB_H