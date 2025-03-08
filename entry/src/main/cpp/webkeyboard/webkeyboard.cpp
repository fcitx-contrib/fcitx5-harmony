#include <fcitx/inputpanel.h>
#include <sstream>

#include "../src/fcitx.h"
#include "webkeyboard.h"


namespace fcitx {

WebKeyboard::WebKeyboard(Instance *instance) : instance_(instance) {}

void WebKeyboard::showVirtualKeyboard() {
    // TODO
}

void WebKeyboard::update(UserInterfaceComponent component, InputContext *inputContext) {
    switch (component) {
    case UserInterfaceComponent::InputPanel: {
        const InputPanel &inputPanel = inputContext->inputPanel();
        if (const auto &list = inputPanel.candidateList()) {
            int size = list->size();
            std::ostringstream oss;
            oss << "[";
            for (int i = 0; i < size; ++i) {
                const auto &candidate = list->candidate(i);
                const auto &text = instance_->outputFilter(inputContext, candidate.text());
                oss << text.toString();
                if (i != (size - 1))
                    oss << ", ";
            }
            oss << "]";
            notify_main_async(oss.str());
        }
        break;
    }
    case UserInterfaceComponent::StatusArea:
        // TODO
        break;
    }
}

} // namespace fcitx

FCITX_ADDON_FACTORY_V2(webkeyboard, fcitx::WebKeyboardFactory)
