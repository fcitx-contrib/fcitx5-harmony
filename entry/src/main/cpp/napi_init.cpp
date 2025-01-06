#include "napi/native_api.h"
#include "fcitx.h"
#include <string>

#define GET_ARGS(n)                                                                                                    \
    size_t argc = n;                                                                                                   \
    napi_value args[n] = {nullptr};                                                                                    \
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

#define GET_STRING(name, i)                                                                                            \
    size_t len##i;                                                                                                     \
    napi_get_value_string_utf8(env, args[i], nullptr, 0, &len##i);                                                     \
    std::string name(len##i, '\0');                                                                                    \
    napi_get_value_string_utf8(env, args[i], name.data(), len##i + 1, &len##i);

static napi_value init(napi_env env, napi_callback_info info) {
    GET_ARGS(2)
    GET_STRING(bundle, 0)
    GET_STRING(resfile, 1)
    fcitx::init(bundle, resfile);
    return {};
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {{"init", nullptr, init, nullptr, nullptr, nullptr, napi_default, nullptr}};
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void *)0),
    .reserved = {0},
};

extern "C" __attribute__((constructor)) void RegisterEntryModule(void) { napi_module_register(&demoModule); }
