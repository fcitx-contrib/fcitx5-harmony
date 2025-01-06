#include <fcitx-utils/event.h>
#include <fcitx-utils/eventdispatcher.h>
#include <fcitx-utils/standardpath.h>
#include <fcitx/addonmanager.h>
#include <fcitx/instance.h>
#include <thread>
#include <sys/stat.h>
#include <filesystem>
#include "nativestreambuf.h"
#include "../harmonyfrontend/harmonyfrontend.h"

#ifdef __x86_64__
#define ARCH "x86_64"
#elif defined(__aarch64__)
#define ARCH "arm64-v8a"
#endif

namespace fs = std::filesystem;

FCITX_DEFINE_STATIC_ADDON_REGISTRY(getStaticAddon)

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
    setenv("XDG_DATA_DIRS", xdg_data_dirs.c_str(), 1);
    setenv("FCITX_DATA_DIRS", fcitx_data_dirs.c_str(), 1);
}

void init(const std::string &bundle, const std::string &resfile) {
    setupLog();
    setupEnv(bundle, resfile);
    // f5a and f5j need it. Not sure if f5h needs it but just do it.
    umask(007);
    StandardPath::global().syncUmask(); // Must happen after setupEnv.

    instance = std::make_unique<Instance>(0, nullptr);
    auto &addonMgr = instance->addonManager();
    addonMgr.registerDefaultLoader(&getStaticAddon());
    instance->initialize();
    dispatcher = std::make_unique<fcitx::EventDispatcher>();
    dispatcher->attach(&instance->eventLoop());
    fcitx_thread = std::thread([] { instance->eventLoop().exec(); });
    frontend = dynamic_cast<HarmonyFrontend *>(addonMgr.addon("harmonyfrontend"));
}
} // namespace fcitx
