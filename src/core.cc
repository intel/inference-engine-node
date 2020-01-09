#include <napi.h>
#include <uv.h>
#include "core.h"
#include "network.h"

#include "inference_engine.hpp"

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference Core::constructor;

void Core::Init(const Napi::Env& env) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(
      env, "Core",
      {InstanceMethod("test", &Core::test)});

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
}

// Core::Core(const Napi::CallbackInfo &info) 
//     : Napi::ObjectWrap<Core>(info) {
//         Napi::Env env = info.Env();
//         Napi::HandleScope scope(env);

//         this->actual_ = ie::Core();
// }

Core::Core(const Napi::CallbackInfo &info) 
    : Napi::ObjectWrap<Core>(info) { }

Napi::Object Core::NewInstance(const Napi::Env& env) {
  Napi::EscapableHandleScope scope(env);

  Napi::Object obj = constructor.New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

// Napi::Object Core::getVersions(const Napi::CallbackInfo& info, Napi::String device_name_value) {}

Napi::Value Core::test(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    // obj.Set("info", "Core.test()");
    // obj.Set("status", "Task 1");

    return Napi::String::New(env, actual_.GetVersions("CPU")["CPU"].buildNumber);
}


}