#ifndef IE_BACKEND_H_
#define IE_BACKEND_H_

#include <node_api.h>
#include <map>
#include <memory>
#include <string>

namespace ienodejs {

class IEBackend {
 public:
  static IEBackend* Create(napi_env env);

  napi_value GetVersion(napi_env env);

  napi_value IENetworkCreate(napi_env env, napi_value model, napi_value weigths);
  
 private:
  IEBackend(napi_env env) {};
  ~IEBackend() {};
};

}  // namespace ienodejs

#endif  // #ifndef IE_BACKEND_H_
