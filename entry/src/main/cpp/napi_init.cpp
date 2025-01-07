#include "fcitx.cpp"
#include "napi/native_api.h"
#include "fcitx.h"
#include <string>

#define API(func) static napi_value func(napi_env env, napi_callback_info info)

#define GET_ARGS(n)                                                                                                    \
    size_t argc = n;                                                                                                   \
    napi_value args[n] = {nullptr};                                                                                    \
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

#define GET_STRING(name, i)                                                                                            \
    size_t len##i;                                                                                                     \
    napi_get_value_string_utf8(env, args[i], nullptr, 0, &len##i);                                                     \
    std::string name(len##i, '\0');                                                                                    \
    napi_get_value_string_utf8(env, args[i], name.data(), len##i + 1, &len##i);

#define GET_I32(name, i)                                                                                               \
    int32_t name;                                                                                                      \
    napi_get_value_int32(env, args[i], &name);

#define GET_BOOL(name, i)                                                                                              \
    bool name;                                                                                                         \
    napi_get_value_bool(env, args[i], &name);

API(init) {
    GET_ARGS(2)
    GET_STRING(bundle, 0)
    GET_STRING(resfile, 1)
    fcitx::init(bundle, resfile);
    return {};
}

API(focusIn) {
    fcitx::focusIn();
    return {};
}

API(focusOut) {
    fcitx::focusOut();
    return {};
}

API(processKeyCode) {
    GET_ARGS(2)
    GET_I32(keyCode, 0)
    GET_BOOL(isRelease, 1)
    fcitx::processKeyCode(keyCode, isRelease);
    return {};
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {
        {"init", nullptr, init, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"focusIn", nullptr, focusIn, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"focusOut", nullptr, focusOut, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"processKeyCode", nullptr, processKeyCode, nullptr, nullptr, nullptr, napi_default, nullptr}};
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
