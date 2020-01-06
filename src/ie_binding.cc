#include <node_api.h>

#include "utils.h"
#include "ie_backend.h"

namespace ienodejs {

IEBackend* gBackend = nullptr;

napi_value IENetworkCreate(napi_env env, napi_callback_info info) {
  napi_status nstatus;

  // IENetworkCreate takes 2 params: model name and weights name;
  // 
  size_t argc = 2;
  napi_value args[2];
  napi_value js_this;
  nstatus = napi_get_cb_info(env, info, &argc, args, &js_this, nullptr);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, nullptr);

  if (argc < 2) {
    NAPI_THROW_ERROR(env, "Invalid number of args passed to IENetworkCreate()");
    return nullptr;
  }

  ENSURE_VALUE_IS_STRING_RETVAL(env, args[0], nullptr);
  ENSURE_VALUE_IS_STRING_RETVAL(env, args[1], nullptr);

  return gBackend->IENetworkCreate(env, args[0], args[1]);
}

napi_value GetVersion(napi_env env, napi_callback_info info) {
  return gBackend->GetVersion(env);
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status nstatus;

  gBackend = IEBackend::Create(env);
  ENSURE_VALUE_IS_NOT_NULL_RETVAL(env, gBackend, nullptr);

  // Set all export values list here.
  napi_property_descriptor exports_properties[] = {
      {"IENetworkCreate", nullptr, IENetworkCreate, nullptr, nullptr, nullptr,
       napi_default, nullptr},
      {"getVersion", nullptr, GetVersion, nullptr, nullptr, nullptr,
       napi_default, nullptr},
  };
  nstatus = napi_define_properties(env, exports, ARRAY_SIZE(exports_properties),
                                   exports_properties);
  ENSURE_NAPI_OK_RETVAL(env, nstatus, exports);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace ienodejs