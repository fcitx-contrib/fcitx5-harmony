#include <string>

#include "fcitx.h"
#include "napihelper.h"

API(init) {
    GET_ARGS(2)
    GET_STRING(bundle, 0)
    GET_STRING(resfile, 1)
    fcitx::init(bundle, resfile);
    return {};
}

API(focusIn) {
    GET_ARGS(1)
    GET_BOOL(clientPreedit, 0)
    fcitx::focusIn(clientPreedit);
    return {};
}

API(focusOut) {
    fcitx::focusOut();
    return {};
}

API(reset) {
    auto state = fcitx::reset();
    OBJECT(ret)
    SET(ret, "commit", state.commit)
    SET(ret, "preedit", state.preedit)
    SET(ret, "cursorPos", state.cursorPos)
    SET(ret, "accepted", state.accepted)
    return ret;
}

API(toggle) {
    fcitx::toggle();
    return {};
}

API(processKey) {
    GET_ARGS(3)
    GET_U32(unicode, 0)
    GET_I32(keyCode, 1)
    GET_BOOL(isRelease, 2)
    auto state = fcitx::processKey(unicode, keyCode, isRelease);
    OBJECT(ret)
    SET(ret, "commit", state.commit)
    SET(ret, "preedit", state.preedit)
    SET(ret, "cursorPos", state.cursorPos)
    SET(ret, "accepted", state.accepted)
    return ret;
}

API(selectCandidate) {
    GET_ARGS(1)
    GET_I32(index, 0)
    fcitx::selectCandidate(index);
    return {};
}

API(askCandidateActions) {
    GET_ARGS(1)
    GET_I32(index, 0)
    fcitx::askCandidateAction(index);
    return {};
}

API(activateCandidateAction) {
    GET_ARGS(2)
    GET_I32(index, 0)
    GET_I32(id, 1)
    fcitx::activateCandidateAction(index, id);
    return {};
}

API(activateStatusAreaAction) {
    GET_ARGS(1)
    GET_I32(id, 0)
    fcitx::activateStatusAreaAction(id);
    return {};
}

API(setCurrentInputMethod) {
    GET_ARGS(1)
    GET_STRING(inputMethod, 0)
    fcitx::setCurrentInputMethod(inputMethod);
    return {};
}

static void CallJs(napi_env env, napi_value jsCb, void *context, void *data) {
    if (env == nullptr) {
        return;
    }
    auto pStr = (std::string *)data;
    napi_value undefined = nullptr;
    napi_get_undefined(env, &undefined);
    STRING(str, *pStr);
    napi_value argv[1] = {str};
    napi_call_function(env, undefined, jsCb, 1, argv, nullptr);
    delete pStr;
}

API(setCallback) {
    GET_ARGS(1)
    std::string name = "callback";
    STRING(name_, name)
    // Bind args[0] and CallJs to tsfn, so that by calling tsfn on fcitx thread,
    // CallJs will be called with args[0] on main thread.
    napi_create_threadsafe_function(env, args[0], nullptr, name_, 0, 1, nullptr, nullptr, nullptr, CallJs, &tsfn);
    return nullptr;
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {
        {"init", nullptr, init, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setCallback", nullptr, setCallback, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"focusIn", nullptr, focusIn, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"focusOut", nullptr, focusOut, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"reset", nullptr, reset, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"toggle", nullptr, toggle, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"processKey", nullptr, processKey, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"selectCandidate", nullptr, selectCandidate, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"askCandidateActions", nullptr, askCandidateActions, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"activateCandidateAction", nullptr, activateCandidateAction, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setCurrentInputMethod", nullptr, setCurrentInputMethod, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"activateStatusAreaAction", nullptr, activateStatusAreaAction, nullptr, nullptr, nullptr, napi_default,
         nullptr}};
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
