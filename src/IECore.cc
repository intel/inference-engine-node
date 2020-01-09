#include "napi.h"
#include "IECore.h"

#include "inference_engine.hpp"

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference Core::constructor;

Napi::Function Core::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope(env);

    Napi::Function func = DefineClass(env, "Core", {
        InstanceMethod("getVersions", &Core::getVersions)
    })

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    return func
}

Core::Core(cons Napi::CallbackInfo &info) 
    : Napi::ObjectWrap<Core>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    this->actual = ie::Core();
}

Napi::Object getVersions(const Napi::CallbackInfo& info, Napi::String device_name_value) {
    Napi::Env env = info.Env();
    
    std::string device_name_string = device_name_value.ToString();
    const std::map<std::string, InferenceEngine::Version> versions = actual::GetVersions();

    Napi::Object version = Napi::Object::New(env);

    Napi::Object api_version = Napi::Object::New(env);
    api_version.Set("major", ie_version->apiVersion.major);
    api_version.Set("minor", ie_version->apiVersion.minor);
    version.Set("apiVersion", api_version);
  
    if (ie_version->buildNumber) {
        version.Set("buildNumber", ie_version->buildNumber);
    }

    if (ie_version->description) {
        version.Set("description", ie_version->description);
    }

    return version; 
}


}