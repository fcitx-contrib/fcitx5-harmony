#pragma once

#include <string>
#include <napi/native_api.h>

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

#define GET_U32(name, i)                                                                                               \
    uint32_t name;                                                                                                     \
    napi_get_value_uint32(env, args[i], &name);

#define GET_I32(name, i)                                                                                               \
    int32_t name;                                                                                                      \
    napi_get_value_int32(env, args[i], &name);

#define GET_BOOL(name, i)                                                                                              \
    bool name;                                                                                                         \
    napi_get_value_bool(env, args[i], &name);

#define OBJECT(name)                                                                                                   \
    napi_value name;                                                                                                   \
    napi_create_object(env, &name);

#define STRING(name, value)                                                                                            \
    napi_value name;                                                                                                   \
    napi_create_string_utf8(env, (value).c_str(), (value).size(), &name);

#define BOOL(name, value)                                                                                              \
    napi_value name;                                                                                                   \
    napi_get_boolean(env, value, &name);

#define INT32(name, value)                                                                                             \
    napi_value name;                                                                                                   \
    napi_create_int32(env, value, &name);

inline void set(napi_env env, napi_value object, const std::string key, const std::string &value) {
    STRING(key_, key);
    STRING(value_, value);
    napi_set_property(env, object, key_, value_);
}

inline void set(napi_env env, napi_value object, const std::string key, bool value) {
    STRING(key_, key);
    BOOL(value_, value);
    napi_set_property(env, object, key_, value_);
}

inline void set(napi_env env, napi_value object, const std::string key, int32_t value) {
    STRING(key_, key);
    INT32(value_, value);
    napi_set_property(env, object, key_, value_);
}

#define SET(object, key, value) set(env, object, key, value);
