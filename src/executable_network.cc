#include "executable_network.h"

#include "infer_request.h"
#include "network.h"

#include <napi.h>

using namespace Napi;
namespace ie = InferenceEngine;

namespace ienodejs {

class LoadNetworkAsyncWorker : public Napi::AsyncWorker {
 public:
  LoadNetworkAsyncWorker(Napi::Env& env,
                         const Napi::Value& network,
                         const Napi::Value& device_name,
                         const ie::Core& core,
                         Napi::Promise::Deferred& deferred)
      : Napi::AsyncWorker(env),
        core_(core),
        device_name_(device_name.As<Napi::String>()),
        env_(env),
        deferred_(deferred) {
    network_ = Napi::ObjectWrap<Network>::Unwrap(network.ToObject())->actual_;
  }

  ~LoadNetworkAsyncWorker() override = default;

  void Execute() override {
    try {
      executable_network_ = core_.LoadNetwork(network_, device_name_);
    } catch (const std::exception& error) {
      Napi::AsyncWorker::SetError(error.what());
      return;
    } catch (...) {
      Napi::AsyncWorker::SetError("Unknown/internal exception happened.");
      return;
    }
  }

  void OnOK() override {
    Napi::EscapableHandleScope scope(env_);
    Napi::Object obj = ExecutableNetwork::constructor.New({});
    ExecutableNetwork* exec_network =
        Napi::ObjectWrap<ExecutableNetwork>::Unwrap(obj);
    exec_network->actual_ = executable_network_;
    deferred_.Resolve(scope.Escape(napi_value(obj)).ToObject());
  }

  void OnError(Napi::Error const& error) override {
    deferred_.Reject(error.Value());
  }

 private:
  ie::CNNNetwork network_;
  ie::Core core_;
  ie::ExecutableNetwork executable_network_;
  std::string device_name_;
  Napi::Env env_;
  Napi::Promise::Deferred deferred_;
};

Napi::FunctionReference ExecutableNetwork::constructor;

void ExecutableNetwork::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "ExecutableNetwork",
      {
          InstanceMethod("createInferRequest",
                         &ExecutableNetwork::CreateInferRequest),
          InstanceAccessor(Napi::Symbol::WellKnown(env, "toStringTag"),
                           &ExecutableNetwork::toStringTag, nullptr,
                           napi_enumerable),
      });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

ExecutableNetwork::ExecutableNetwork(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<ExecutableNetwork>(info) {}

void ExecutableNetwork::NewInstanceAsync(Napi::Env& env,
                                         const Napi::Value& network,
                                         const Napi::Value& dev_name,
                                         const ie::Core& core,
                                         Napi::Promise::Deferred& deferred) {
  auto load_network_worker =
      new LoadNetworkAsyncWorker(env, network, dev_name, core, deferred);
  load_network_worker->Queue();
}

Napi::Value ExecutableNetwork::CreateInferRequest(
    const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  try {
    ie::InferRequest infer_request = actual_.CreateInferRequest();
    return InferRequest::NewInstance(env, infer_request);
  } catch (const std::exception& error) {
    Napi::RangeError::New(env, error.what()).ThrowAsJavaScriptException();
    return env.Null();
  } catch (...) {
    Napi::Error::New(env, "Unknown/internal exception happened.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value ExecutableNetwork::toStringTag(const Napi::CallbackInfo& info) {
  return Napi::String::From(info.Env(), "ExecutableNetwork");
}

}  // namespace ienodejs