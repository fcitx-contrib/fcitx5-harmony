#include "harmonyfrontend.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
    ic_->isSyncEvent = true;
    ic_->keyEvent(event);
    ic_->isSyncEvent = false;
    return ic_->popState(event.accepted());
}

void HarmonyFrontend::focusIn(bool clientPreedit) {
    CapabilityFlags flags;
    if (clientPreedit) {
        flags |= CapabilityFlag::Preedit;
    }
    ic_->setCapabilityFlags(flags);
    ic_->focusIn();
}

void HarmonyFrontend::focusOut() { ic_->focusOut(); }

void HarmonyFrontend::reset() { ic_->reset(); }

HarmonyInputContext::HarmonyInputContext(HarmonyFrontend *frontend, InputContextManager &inputContextManager)
    : InputContext(inputContextManager, ""), frontend_(frontend) {
    created();
}

HarmonyInputContext::~HarmonyInputContext() { destroy(); }

void HarmonyInputContext::commitStringImpl(const std::string &text) {
    state_.commit += text;
    if (!isSyncEvent) {
        commitStringAsync();
    }
}

void HarmonyInputContext::updatePreeditImpl() {
    auto preedit = filterText(inputPanel().clientPreedit());
    state_.preedit = preedit.toString();
    state_.cursorPos = preedit.cursor();
    if (!isSyncEvent) {
        updatePreeditAsync();
    }
}

InputContextState HarmonyInputContext::popState(bool accepted) {
    auto state = state_;
    // Don't clear preedit as fcitx may not update it when not changed.
    state_.commit = "";
    state.accepted = accepted;
    return state;
}

void HarmonyInputContext::commitStringAsync() {
    auto state = state_;
    state_.commit = "";
    auto j = json{{"type", "COMMIT"}, {"data", state.commit}};
    notify_main_async(j.dump());
}

void HarmonyInputContext::updatePreeditAsync() {
    auto state = state_;
    auto j = json{{"type", "PREEDIT"}, {"data", {{"preedit", state.preedit}, {"cursorPos", state.cursorPos}}}};
    notify_main_async(j.dump());
}

} // namespace fcitx

FCITX_ADDON_FACTORY_V2(harmonyfrontend, fcitx::HarmonyFrontendFactory);
