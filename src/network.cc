#include "../include/network.h"
#include "../include/input_info.h"
#include "../include/output_info.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

class ReadNetworkAsyncWorker : public Napi::AsyncWorker {
 public:
  ReadNetworkAsyncWorker(Napi::Env& env,
                         const Napi::Value& model,
                         const Napi::Value& weights,
                         const InferenceEngine::Core& core,
                         Napi::Promise::Deferred& deferred)
      : Napi::AsyncWorker(env),
        core_(core),
        model_(model.As<Napi::String>()),
        read_from_data_(false),
        env_(env),
        deferred_(deferred) {
    if (weights.IsString()) {
      weights_path_ = weights.As<Napi::String>();
    } else {
      read_from_data_ = true;
      ArrayBuffer buffer = weights.As<Napi::ArrayBuffer>();
      ie::TensorDesc desc(ie::Precision::U8, {buffer.ByteLength()},
                          ie::Layout::C);
      weights_blob_ =
          ie::make_shared_blob<uint8_t>(desc, (uint8_t*)buffer.Data());
    }
  }

  ~ReadNetworkAsyncWorker() {}

  void Execute() {
    try {
      if (read_from_data_) {
        actual_ = core_.ReadNetwork(model_, weights_blob_);
      } else {
        actual_ = core_.ReadNetwork(model_, weights_path_);
      }
    } catch (const std::exception& error) {
      Napi::AsyncWorker::SetError(error.what());
      return;
    } catch (...) {
      Napi::AsyncWorker::SetError("Unknown/internal exception happened.");
      return;
    }
  }

  void OnOK() {
    Napi::EscapableHandleScope scope(env_);
    Napi::Object obj = Network::constructor.New({});
    Network* network = Napi::ObjectWrap<Network>::Unwrap(obj);
    network->actual_ = actual_;
    deferred_.Resolve(scope.Escape(napi_value(obj)).ToObject());
  }

  void OnError(Napi::Error const& error) { deferred_.Reject(error.Value()); }

 private:
  ie::CNNNetwork actual_;
  ie::Core core_;
  std::string model_;
  std::string weights_path_;
  ie::Blob::CPtr weights_blob_;
  bool read_from_data_;
  Napi::Env env_;
  Napi::Promise::Deferred deferred_;
};

Napi::FunctionReference Network::constructor;

void Network::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "Network",
      {
          InstanceMethod("getName", &Network::GetName),
          InstanceMethod("getInputsInfo", &Network::GetInputsInfo),
          InstanceMethod("getOutputsInfo", &Network::GetOutputsInfo),
      });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Network::Network(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<Network>(info) {}

void Network::NewInstanceAsync(Napi::Env env,
                               const Napi::Value& model,
                               const Napi::Value& weights,
                               const InferenceEngine::Core& core,
                               Napi::Promise::Deferred& deferred) {
  ReadNetworkAsyncWorker* read_network_worker =
      new ReadNetworkAsyncWorker(env, model, weights, core, deferred);
  read_network_worker->Queue();
}

Napi::Value Network::GetName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  return Napi::String::New(env, actual_.getName());
}

Napi::Value Network::GetInputsInfo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  const ie::InputsDataMap ie_inputs_info = actual_.getInputsInfo();
  Napi::Array js_inputs_info = Napi::Array::New(env, ie_inputs_info.size());
  size_t i = 0;
  for (auto& in : ie_inputs_info) {
    js_inputs_info[i++] = InputInfo::NewInstance(env, in.second);
  }
  return js_inputs_info;
}

Napi::Value Network::GetOutputsInfo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  const ie::OutputsDataMap ie_outputputs_info = actual_.getOutputsInfo();
  Napi::Array js_outputs_info =
      Napi::Array::New(env, ie_outputputs_info.size());
  size_t i = 0;
  for (auto& out : ie_outputputs_info) {
    js_outputs_info[i++] = OutputInfo::NewInstance(env, out.second);
  }
  return js_outputs_info;
}

}  // namespace ienodejs