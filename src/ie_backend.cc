#include "ie_backend.h"
#include "utils.h"

#include "inference_engine.hpp"

#include <algorithm>
#include <cstring>
#include <memory>
#include <set>
#include <string>

namespace ie = InferenceEngine;

namespace ienodejs {

IEBackend *IEBackend::Create(napi_env env) { return new IEBackend(env); }

napi_value IEBackend::GetVersion(napi_env env) {
  napi_status nstatus;

  const ie::Version* version = ie::GetInferenceEngineVersion();

  napi_value result;
  nstatus = napi_create_object(env, &result);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);

  // apiVersion
  napi_value api_version;
  nstatus = napi_create_object(env, &api_version);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  napi_value major;
  nstatus = napi_create_int32(env, version->apiVersion.major, &major);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  nstatus = napi_set_named_property(env, api_version, "major", major);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  napi_value minor;
  nstatus = napi_create_int32(env, version->apiVersion.minor, &minor);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  nstatus = napi_set_named_property(env, api_version, "minor", minor);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  nstatus = napi_set_named_property(env, result, "apiVersion", api_version);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);

  // buildNumber
  if (version->buildNumber) {
    napi_value build_number;
    nstatus = napi_create_string_utf8(env, version->buildNumber, strlen(version->buildNumber), &build_number);
    ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
    nstatus = napi_set_named_property(env, result, "buildNumber", build_number);
    ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  }

  // description
  if (version->description) {
    napi_value description;
    nstatus = napi_create_string_utf8(env, version->description, strlen(version->description), &description);
    ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
    nstatus = napi_set_named_property(env, result, "description", description);
    ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);
  }

  return result;
}

napi_value IENetworkCreate(napi_env env, napi_value model, napi_value weigths) {
  napi_value network_id;
  return network_id;
}

}  // namespace ienodejs