#include "harmonyfrontend.h"

namespace fcitx {
HarmonyFrontend::HarmonyFrontend(Instance *instance)
    : instance_(instance), focusGroup_("harmony", instance->inputContextManager()) {
    createInputContext();
}

void HarmonyFrontend::createInputContext() {
    ic_ = new HarmonyInputContext(this, instance_->inputContextManager());
    ic_->setFocusGroup(&focusGroup_);
}

InputContextState HarmonyFrontend::keyEvent(const Key &key, bool isRelease) {
    KeyEvent event(ic_, key, isRelease);
    ic_->keyEvent(event);
    return ic_->popState(event.accepted());
}

void HarmonyFrontend::focusIn() { ic_->focusIn(); }

void HarmonyFrontend::focusOut() { ic_->focusOut(); }

HarmonyInputContext::HarmonyInputContext(HarmonyFrontend *frontend, InputContextManager &inputContextManager)
    : InputContext(inputContextManager, ""), frontend_(frontend) {
    CapabilityFlags flags = CapabilityFlag::Preedit;
    setCapabilityFlags(flags);
    created();
}

HarmonyInputContext::~HarmonyInputContext() { destroy(); }

void HarmonyInputContext::commitStringImpl(const std::string &text) { state_.commit += text; }

void HarmonyInputContext::updatePreeditImpl() {
    auto preedit = frontend_->instance()->outputFilter(this, inputPanel().clientPreedit());
    state_.preedit = preedit.toString();
    state_.cursorPos = preedit.cursor();
}

InputContextState HarmonyInputContext::popState(bool accepted) {
    auto state = state_;
    // Don't clear preedit as fcitx may not update it when not changed.
    state_.commit = "";
    state.accepted = accepted;
    return state;
}
} // namespace fcitx

FCITX_ADDON_FACTORY_V2(harmonyfrontend, fcitx::HarmonyFrontendFactory);
