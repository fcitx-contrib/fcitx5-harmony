#pragma once

#include <fcitx-config/configuration.h>
#include <fcitx/addonfactory.h>
#include <fcitx/addoninstance.h>
#include <fcitx/addonmanager.h>
#include <fcitx/focusgroup.h>
#include <fcitx/instance.h>

namespace fcitx {
class HarmonyInputContext;

class HarmonyFrontend : public AddonInstance {
public:
    HarmonyFrontend(Instance *instance);
    Instance *instance() { return instance_; }

    void reloadConfig() override {}
    void save() override {}
    const Configuration *getConfig() const override { return nullptr; }
    void setConfig(const RawConfig &config) override {}

    void createInputContext();
    bool keyEvent(const Key &key, bool isRelease);
    void focusIn();
    void focusOut();

private:
    Instance *instance_;
    FocusGroup focusGroup_;
    HarmonyInputContext *ic_;
};

class HarmonyFrontendFactory : public AddonFactory {
public:
    AddonInstance *create(AddonManager *manager) override { return new HarmonyFrontend(manager->instance()); }
};

class HarmonyInputContext : public InputContext {
public:
    HarmonyInputContext(HarmonyFrontend *frontend, InputContextManager &inputContextManager);
    ~HarmonyInputContext();

    const char *frontend() const override { return "harmony"; }
    void commitStringImpl(const std::string &text) override;
    void deleteSurroundingTextImpl(int offset, unsigned int size) override {}
    void forwardKeyImpl(const ForwardKeyEvent &key) override {}
    void updatePreeditImpl() override;

private:
    HarmonyFrontend *frontend_;
};
} // namespace fcitx