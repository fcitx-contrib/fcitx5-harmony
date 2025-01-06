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

namespace fs = std::filesystem;

namespace fcitx {
FCITX_DEFINE_STATIC_ADDON_REGISTRY(getStaticAddon)

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
    std::string fcitx_addon_dirs = bundlePath / "libs" / "x86_64"; // XXX: fix arch
    setenv("FCITX_ADDON_DIRS", fcitx_addon_dirs.c_str(), 1);
    std::string xdg_data_dirs = resfilePath / "usr" / "share";
    setenv("XDG_DATA_DIRS", xdg_data_dirs.c_str(), 1);
    FCITX_ERROR() << fcitx_addon_dirs << " " << xdg_data_dirs;
}

void init(const std::string &bundle, const std::string &resfile) {
    umask(007);
    StandardPath::global().syncUmask();
    setupLog();
    setupEnv(bundle, resfile);
    instance = std::make_unique<Instance>(0, nullptr);
    auto &addonMgr = instance->addonManager();
    addonMgr.registerDefaultLoader(&getStaticAddon());
    instance->initialize();
    dispatcher = std::make_unique<fcitx::EventDispatcher>();
    dispatcher->attach(&instance->eventLoop());
    fcitx_thread = std::thread([] { instance->eventLoop().exec(); });
    frontend = dynamic_cast<HarmonyFrontend *>(addonMgr.addon("harmonyfrontend"));
    FCITX_ERROR() << frontend;
}
} // namespace fcitx
