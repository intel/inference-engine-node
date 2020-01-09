#ifndef CORE_H
#define CORE_H

#include <napi.h>

#include "inference_engine.hpp"

namespace ienodejs {

class Core : public Napi::ObjectWrap<Core> {
    public:
        static void Init(const Napi::Env& env);
        static Napi::Object NewInstance(const Napi::Env& env);
        Core(const Napi::CallbackInfo& info);

    private:
        static Napi::FunctionReference constructor;

        Napi::Value test(const Napi::CallbackInfo& info);

        // Napi::Object getVersions(const Napi::CallbackInfo& info, Napi::String device_name);
        
       // Napi::Object loadNetwork(const Napi::CallbackInfo& info, Napi::String device_name);

        InferenceEngine::Core actual_;
};

}



#endif