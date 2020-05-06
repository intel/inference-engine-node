#include "core.h"
#include "executable_network.h"
#include "network.h"

#include <napi.h>
#include <uv.h>

#include "inference_engine.hpp"

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference Core::constructor;

void Core::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "Core",
      {InstanceMethod("getVersions", &Core::GetVersions),
       InstanceMethod("readNetwork", &Core::ReadNetwork),
       InstanceMethod("readNetworkFromData", &Core::ReadNetworkFromData),
       InstanceMethod("loadNetwork", &Core::LoadNetwork),
       InstanceMethod("getAvailableDevices", &Core::GetAvailableDevices)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Core::Core(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Core>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->actual_ = ie::Core();
}

Napi::Object Core::NewInstance(const Napi::Env& env) {
  Napi::EscapableHandleScope scope(env);

  Napi::Object obj = constructor.New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value Core::GetVersions(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string device_name_string = info[0].ToString().Utf8Value();

  std::map<std::string, ie::Version> versions_map;
  try {
    versions_map = actual_.GetVersions(device_name_string);
  } catch (const std::exception& error) {
    Napi::TypeError::New(env, error.what()).ThrowAsJavaScriptException();
    return env.Null();
  } catch (...) {
    Napi::Error::New(env, "Unknown/internal exception happened.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  std::map<std::string, ie::Version>::iterator iter;

  // FIXME: should create a Map once node-addon-api supports it.
  Napi::Object versions = Napi::Object::New(env);

  for (iter = versions_map.begin(); iter != versions_map.end(); iter++) {
    Napi::Object device = Napi::Object::New(env);
    ie::Version* ie_version = &iter->second;

    Napi::Object api_version = Napi::Object::New(env);
    api_version.Set("major", ie_version->apiVersion.major);
    api_version.Set("minor", ie_version->apiVersion.minor);
    device.Set("apiVersion", api_version);

    if (ie_version->buildNumber) {
      device.Set("buildNumber", ie_version->buildNumber);
    }

    if (ie_version->description) {
      device.Set("description", ie_version->description);
    }

    versions.Set(iter->first, device);
  }

  Napi::Value versions_napi_value = Napi::Value::From(env, versions);
  return versions_napi_value;
}

Napi::Value Core::ReadNetwork(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

  if (info.Length() < 2) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong number of arguments").Value());
    return deferred.Promise();
  }

  if (!info[0].IsString() || !info[1].IsString()) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong type of arguments").Value());
    return deferred.Promise();
  }

  Network::NewInstanceAsync(env, info[0], info[1], actual_, deferred);
  return deferred.Promise();
}

Napi::Value Core::ReadNetworkFromData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

  if (info.Length() < 2) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong number of arguments").Value());
    return deferred.Promise();
  }

  if (!info[0].IsString() || !info[1].IsArrayBuffer()) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong type of arguments").Value());
    return deferred.Promise();
  }

  Network::NewInstanceAsync(env, info[0], info[1], actual_, deferred);
  return deferred.Promise();
}

Napi::Value Core::LoadNetwork(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

  if (info.Length() < 2) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong number of arguments").Value());
    return deferred.Promise();
  }

  if (!info[0].IsObject() || !info[1].IsString()) {
    deferred.Reject(
        Napi::TypeError::New(env, "Wrong type of arguments").Value());
    return deferred.Promise();
  }

  if (!info[0].ToObject().InstanceOf(Network::constructor.Value())) {
    deferred.Reject(Napi::TypeError::New(
                        env, "The first argument should be a Network object")
                        .Value());
    return deferred.Promise();
  }

  ExecutableNetwork::NewInstanceAsync(env, info[0], info[1], actual_, deferred);

  return deferred.Promise();
}

Napi::Value Core::GetAvailableDevices(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  std::vector<std::string> availableDevices = actual_.GetAvailableDevices();

  std::vector<std::string>::iterator iter;

  Napi::Array devices = Napi::Array::New(env);
  size_t i = 0;
  for (iter = availableDevices.begin(); iter != availableDevices.end();
       iter++) {
    devices[i++] = *iter;
  }

  Napi::Value available_devices = Napi::Value::From(env, devices);
  return available_devices;
}

}  // namespace ienodejs