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
  explicit Blob(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  // APIs
  Napi::Value ByteSize(const Napi::CallbackInfo& info);
  Napi::Value Size(const Napi::CallbackInfo& info);
  Napi::Value Rwmap(const Napi::CallbackInfo& info) {
    return Memmap(info, READ_WRITE);
  }
  Napi::Value Rmap(const Napi::CallbackInfo& info) {
    return Memmap(info, READ);
  }
  Napi::Value Wmap(const Napi::CallbackInfo& info) {
    return Memmap(info, WRITE);
  }
  Napi::Value Unmap(const Napi::CallbackInfo& info);

  // Helpers
  const static int READ = 0;
  const static int WRITE = 1;
  const static int READ_WRITE = 2;
  Napi::Value Memmap(const Napi::CallbackInfo& info, const int mode);

  InferenceEngine::Blob::Ptr actual_;
  std::unique_ptr<InferenceEngine::LockedMemory<void>> locked_memory_;
};

}  // namespace ienodejs

#endif  // IE_BLOB_H