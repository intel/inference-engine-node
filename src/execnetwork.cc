#include "execnetwork.h"
#include "network.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;
namespace ie = InferenceEngine;

namespace ienodejs {

class ExecnetworkAsyncWorker : public Napi::AsyncWorker {
 public:
  ExecnetworkAsyncWorker(Napi::Env& env,
                         const Napi::Value& network,
                         const Napi::Value& device_name,
                         ie::Core& core,
                         Napi::Promise::Deferred& deferred)
      : env_(env),
        Napi::AsyncWorker(env),
        device_name_(device_name.As<Napi::String>()),
        core_(core),
        deferred_(deferred) {
    network_ = Napi::ObjectWrap<Network>::Unwrap(network.ToObject())->actual_;
  }

  ~ExecnetworkAsyncWorker() {}

  void Execute() {
    try {
      name_ = network_.getName();
      actual_ = core_.LoadNetwork(network_, device_name_);
    } catch (const std::exception& error) {
      Napi::AsyncWorker::SetError("The first step error.");
      return;
    } catch (...) {
      Napi::AsyncWorker::SetError("The second step error.");
      return;
    }
  }

  void OnOK() {
    Napi::EscapableHandleScope scope(env_);
    Napi::Object obj = ExecutableNetwork::constructor.New({});
    ExecutableNetwork* execnetwork =
        Napi::ObjectWrap<ExecutableNetwork>::Unwrap(obj);
    execnetwork->actual_ = actual_;
    execnetwork->name = name_;
    deferred_.Resolve(scope.Escape(napi_value(obj)).ToObject());
  }

  void OnError(Napi::Error const& error) { deferred_.Reject(error.Value()); }

 private:
  ie::CNNNetwork network_;
  ie::Core core_;
  ie::ExecutableNetwork actual_;
  std::string device_name_;
  std::string name_;
  Napi::Env env_;
  Napi::Promise::Deferred deferred_;
};

Napi::FunctionReference ExecutableNetwork::constructor;

void ExecutableNetwork::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env, "ExecutableNetwork",
                  {InstanceMethod("getName", &ExecutableNetwork::GetName)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

ExecutableNetwork::ExecutableNetwork(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<ExecutableNetwork>(info) {}

void ExecutableNetwork::NewInstanceAsync(Napi::Env& env,
                                         const Napi::Value& network,
                                         const Napi::Value& dev_name,
                                         ie::Core core,
                                         Napi::Promise::Deferred& deferred) {
  ExecnetworkAsyncWorker* execworker =
      new ExecnetworkAsyncWorker(env, network, dev_name, core, deferred);
  execworker->Queue();
}

Napi::Value ExecutableNetwork::GetName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, name);
}

}  // namespace ienodejs