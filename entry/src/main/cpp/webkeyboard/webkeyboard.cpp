#include <fcitx/inputpanel.h>

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
        int highlighted = -1;
        std::vector<Candidate> candidates;
        const InputPanel &inputPanel = inputContext->inputPanel();
        if (const auto &list = inputPanel.candidateList()) {
            int size = list->size();
            candidates.reserve(size);
            for (int i = 0; i < size; ++i) {
                const auto &label = stringutils::trim(list->label(i).toString());
                const auto &candidate = list->candidate(i);
                candidates.push_back({label, instance_->outputFilter(inputContext, candidate.text()).toString(),
                                      instance_->outputFilter(inputContext, candidate.comment()).toString()});
            }
            highlighted = list->cursorIndex();
        }
        setCandidatesAsync(candidates, highlighted);
        if (candidates.empty()) {
            notify_main_async(R"JSON({"type":"CLEAR"})JSON");
        }
        break;
    }
    case UserInterfaceComponent::StatusArea:
        // TODO
        break;
    }
}

void WebKeyboard::setCandidatesAsync(const std::vector<Candidate> &candidates, int highlighted) {
    auto j = json{{"type", "CANDIDATES"}, {"data", {{"candidates", candidates}, {"highlighted", highlighted}}}};
    notify_main_async(j.dump());
}

} // namespace fcitx

FCITX_ADDON_FACTORY_V2(webkeyboard, fcitx::WebKeyboardFactory)
