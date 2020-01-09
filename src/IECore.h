#ifndef CORE_H
#define CORE_H

#include <napi.h>

#include "inference_engine.hpp"

class Core : public Napi::ObjectWrap<Core> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        Core(const Napi::CallbackInfo& info);

    private:
        static Napi::FunctionReference constructor;

        Napi::Object getVersions(const Napi::CallbackInfo& info, Napi::String device_name);
        //Napi::Object loadNetwork(const Napi::CallbackInfo& info, Napi::String device_name);

        InferenceEngine::Core actual;
};



#endif