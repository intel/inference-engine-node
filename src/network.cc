#include "network.h"
#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

class NetworkAsyncWorker : public Napi::AsyncWorker {
 public:
  NetworkAsyncWorker(Napi::Env& env, const Napi::Value& model, const Napi::Value& weights, Napi::Promise::Deferred& deferred)
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
    } catch (const std::exception & error) {
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

  void OnError(Napi::Error const& error) {
    deferred_.Reject(error.Value());
  }

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
      {InstanceMethod("getName", &Network::getName)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Network::Network(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<Network>(info) {}

void Network::NewInstanceAsync(Napi::Env env, const Napi::Value& model, const Napi::Value& weights, Napi::Promise::Deferred& deferred) {
  NetworkAsyncWorker* worker = new NetworkAsyncWorker(env, model, weights, deferred);
  worker->Queue();
}

Napi::Value Network::getName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, actual_.getName());
}

}  // ienodejs