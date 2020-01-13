#include "network.h"
#include "input_info.h"
#include "output_info.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

class NetworkAsyncWorker : public Napi::AsyncWorker {
 public:
  NetworkAsyncWorker(Napi::Env& env,
                     const Napi::Value& model,
                     const Napi::Value& weights,
                     Napi::Promise::Deferred& deferred)
      : env_(env),
        Napi::AsyncWorker(env),
        model_(model.As<Napi::String>()),
        weights_(weights.As<Napi::String>()),
        deferred_(deferred) {}

  ~NetworkAsyncWorker() {}

  void Execute() {
    try {
      ie::CNNNetReader net_reader;
      net_reader.ReadNetwork(model_);
      net_reader.ReadWeights(weights_);
      actual_ = net_reader.getNetwork();
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
  InferenceEngine::CNNNetwork actual_;
  std::string model_;
  std::string weights_;
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
                               Napi::Promise::Deferred& deferred) {
  NetworkAsyncWorker* worker =
      new NetworkAsyncWorker(env, model, weights, deferred);
  worker->Queue();
}

Napi::Value Network::GetName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, actual_.getName());
}

Napi::Value Network::GetInputsInfo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
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