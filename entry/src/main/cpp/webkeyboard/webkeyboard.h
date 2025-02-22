#pragma once

#include <fcitx/addonfactory.h>
#include <fcitx/addoninstance.h>
#include <fcitx/addonmanager.h>
#include <fcitx/instance.h>

namespace fcitx {

class WebKeyboard final : public VirtualKeyboardUserInterface {
public:
    WebKeyboard(Instance *instance);
    virtual ~WebKeyboard() = default;

    void reloadConfig() override {};
    const Configuration *getConfig() const override { return nullptr; }
    void setConfig(const RawConfig &config) override {};
    void setSubConfig(const std::string &path, const RawConfig &config) override {};

    Instance *instance() { return instance_; }

    bool available() override { return true; }
    void suspend() override {}
    void resume() override {}
    void update(UserInterfaceComponent component, InputContext *inputContext) override;
    bool isVirtualKeyboardVisible() const override { return true; }
    void showVirtualKeyboard() override;
    void hideVirtualKeyboard() override {}

private:
    Instance *instance_;
};

class WebKeyboardFactory : public AddonFactory {
public:
    AddonInstance *create(AddonManager *manager) override { return new WebKeyboard(manager->instance()); }
};

} // namespace fcitx
