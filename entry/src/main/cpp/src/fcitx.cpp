#include "fcitx.h"
#include <fcitx-utils/event.h>
#include <fcitx-utils/eventdispatcher.h>
#include <fcitx-utils/standardpath.h>
#include <fcitx/action.h>
#include <fcitx/addonmanager.h>
#include <fcitx/instance.h>
#include <fcitx/userinterfacemanager.h>
#include <thread>
#include <sys/stat.h>
#include <filesystem>
#include "nativestreambuf.h"
#include "keycode.h"
#include "util.h"
#include "../harmonyfrontend/harmonyfrontend.h"
#include "inputmethod.h"

#ifdef __x86_64__
#define ARCH "x86_64"
#elif defined(__aarch64__)
#define ARCH "arm64-v8a"
#endif

namespace fs = std::filesystem;
using json = nlohmann::json;

FCITX_DEFINE_STATIC_ADDON_REGISTRY(getStaticAddon)

napi_threadsafe_function tsfn;

void notify_main_async(std::string str) {
    auto pStr = new std::string(std::move(str));
    napi_call_threadsafe_function_with_priority(tsfn, pStr, napi_priority_high, true);
}

namespace fcitx {
std::unique_ptr<Instance> instance;
std::unique_ptr<fcitx::EventDispatcher> dispatcher;
HarmonyFrontend *frontend;

static native_streambuf log_streambuf;
static std::ostream stream(&log_streambuf);

static std::thread fcitx_thread;

void setupLog() {
    fcitx::Log::setLogStream(stream);
    fcitx::Log::setLogRule("*=5,notimedate");
}

void setupEnv(const std::string &bundle, const std::string &resfile) {
    // resfile is /data/storage/el1/bundle/entry/resources/resfile
    ::fs::path bundlePath = bundle;
    ::fs::path resfilePath = resfile;
    std::string fcitx_addon_dirs = bundlePath / "libs" / ARCH;
    setenv("FCITX_ADDON_DIRS", fcitx_addon_dirs.c_str(), 1);
    ::fs::path xdg_data_dirs = resfilePath / "usr" / "share";
    std::string fcitx_data_dirs = xdg_data_dirs / "fcitx5";
    std::string libime_model_dirs = resfilePath / "usr" / "lib" / "libime";
    setenv("XDG_DATA_DIRS", xdg_data_dirs.c_str(), 1);
    setenv("FCITX_DATA_DIRS", fcitx_data_dirs.c_str(), 1);
    setenv("LIBIME_MODEL_DIRS", libime_model_dirs.c_str(), 1);
}

void init(const std::string &bundle, const std::string &resfile) {
    setupLog();
    setupEnv(bundle, resfile);
    // f5a and f5j need it. Not sure if f5h needs it but just do it.
    umask(007);
    StandardPath::global().syncUmask(); // Must happen after setupEnv.

    instance = std::make_unique<Instance>(0, nullptr);
    instance->setInputMethodMode(fcitx::InputMethodMode::OnScreenKeyboard);
    auto &addonMgr = instance->addonManager();
    addonMgr.registerDefaultLoader(&getStaticAddon());
    instance->initialize();
    dispatcher = std::make_unique<fcitx::EventDispatcher>();
    dispatcher->attach(&instance->eventLoop());
    fcitx_thread = std::thread([] { instance->eventLoop().exec(); });
    frontend = dynamic_cast<HarmonyFrontend *>(addonMgr.addon("harmonyfrontend"));
    setInputMethods({"pinyin"}); // XXX: for test only.
}

void focusIn(bool clientPreedit) {
    with_fcitx([clientPreedit] { frontend->focusIn(clientPreedit); });
}

void focusOut() {
    with_fcitx([] { frontend->focusOut(); });
}

InputContextState reset() {
    return with_fcitx([] { return frontend->reset(); });
}

InputContextState processKey(uint32_t unicode, int32_t keyCode, bool isRelease) {
    return with_fcitx([unicode, keyCode, isRelease] {
        auto key = ohKeyToFcitxKey(unicode, keyCode);
        return frontend->keyEvent(key, isRelease);
    });
}

void selectCandidate(int index) {
    with_fcitx([index] {
        auto ic = instance->mostRecentInputContext();
        const auto &list = ic->inputPanel().candidateList();
        if (!list)
            return;
        try {
            // Engine is responsible for updating UI
            list->candidate(index).select(ic);
        } catch (const std::invalid_argument &e) {
            FCITX_ERROR() << "select candidate index out of range";
        }
    });
}

void askCandidateAction(int index) {
    with_fcitx([index] {
        auto ic = instance->mostRecentInputContext();
        const auto &list = ic->inputPanel().candidateList();
        if (!list)
            return;
        auto *actionableList = list->toActionable();
        if (!actionableList) {
            return;
        }
        try {
            auto &candidate = list->candidate(index);
            if (actionableList->hasAction(candidate)) {
                json actions = json::array();
                for (const auto &action : actionableList->candidateActions(candidate)) {
                    actions.push_back({{"id", action.id()}, {"text", action.text()}});
                }
                notify_main_async(
                    json{{"type", "CANDIDATE_ACTIONS"}, {"data", {{"index", index}, {"actions", actions}}}}.dump());
            }
        } catch (const std::invalid_argument &e) {
            FCITX_ERROR() << "action candidate index out of range";
        }
    });
}

void activateCandidateAction(int index, int id) {
    with_fcitx([=] {
        auto ic = instance->mostRecentInputContext();
        const auto &list = ic->inputPanel().candidateList();
        if (!list)
            return;
        auto *actionableList = list->toActionable();
        if (!actionableList) {
            return;
        }
        try {
            auto &candidate = list->candidate(index);
            if (actionableList->hasAction(candidate)) {
                actionableList->triggerAction(candidate, id);
            }
        } catch (const std::invalid_argument &e) {
            FCITX_ERROR() << "action candidate index out of range";
        }
    });
}

void activateStatusAreaAction(int id) {
    with_fcitx([id] {
        if (auto *ic = instance->mostRecentInputContext()) {
            auto *action = instance->userInterfaceManager().lookupActionById(id);
            action->activate(ic);
        }
    });
}
} // namespace fcitx
