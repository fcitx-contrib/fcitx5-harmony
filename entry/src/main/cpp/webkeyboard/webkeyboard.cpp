#include <fcitx/action.h>
#include <fcitx/inputpanel.h>
#include <fcitx/menu.h>
#include <fcitx/statusarea.h>

#include "../src/fcitx.h"
#include "webkeyboard.h"
#include <sstream>

#include "pugixml.hpp"

std::string escape_html(const std::string &content) {
    std::ostringstream oss;
    pugi::xml_document doc;
    doc.append_child(pugi::node_pcdata).set_value(content);
    doc.print(oss, "", pugi::format_raw);
    return oss.str();
}

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
        auto auxUp = instance_->outputFilter(inputContext, inputPanel.auxUp()).toString();
        auto preedit = instance_->outputFilter(inputContext, inputPanel.preedit()).toString();
        notify_main_async(json{{"type", "PREEDIT"}, {"data", {{"auxUp", auxUp}, {"preedit", preedit}}}}.dump());
        if (const auto &list = inputPanel.candidateList()) {
            const auto &bulk = list->toBulk();
            if (bulk) {
                return expand();
            }
            int size = list->size();
            candidates.reserve(size);
            for (int i = 0; i < size; ++i) {
                const auto &label = stringutils::trim(list->label(i).toString());
                const auto &candidate = list->candidate(i);
                candidates.push_back({instance_->outputFilter(inputContext, candidate.text()).toString(), label,
                                      instance_->outputFilter(inputContext, candidate.comment()).toString()});
            }
            highlighted = list->cursorIndex();
        }
        if (auxUp.empty() && preedit.empty() && candidates.empty()) {
            notify_main_async(R"JSON({"type":"CLEAR"})JSON");
        } else {
            setCandidatesAsync(candidates, highlighted, 0, false, false);
        }
        break;
    }
    case UserInterfaceComponent::StatusArea:
        updateStatusArea(inputContext);
        break;
    }
}

void WebKeyboard::setCandidatesAsync(const std::vector<Candidate> &candidates, int highlighted, int scrollState,
                                     bool scrollStart, bool scrollEnd) {
    auto j = json{{"type", "CANDIDATES"},
                  {"data",
                   {{"candidates", candidates},
                    {"highlighted", highlighted},
                    {"scrollState", scrollState},
                    {"scrollStart", scrollStart},
                    {"scrollEnd", scrollEnd}}}};
    notify_main_async(j.dump());
}

void WebKeyboard::expand() { scroll(0, 20); }

void WebKeyboard::scroll(int start, int count) {
    auto ic = instance_->mostRecentInputContext();
    const auto &list = ic->inputPanel().candidateList();
    if (!list) {
        return;
    }
    const auto &bulk = list->toBulk();
    if (!bulk) {
        return;
    }
    int size = bulk->totalSize();
    int end = size < 0 ? start + count : std::min(start + count, size);
    bool endReached = size == end;
    std::vector<Candidate> candidates;
    for (int i = start; i < end; ++i) {
        try {
            auto &candidate = bulk->candidateFromAll(i);
            candidates.push_back({instance_->outputFilter(ic, candidate.text()).toString(), "",
                                  instance_->outputFilter(ic, candidate.comment()).toString()});
        } catch (const std::invalid_argument &e) {
            // size == -1 but actual limit is reached
            endReached = true;
            break;
        }
    }
    setCandidatesAsync(candidates, start == 0 ? 0 : -1, 2, start == 0, endReached);
}

static nlohmann::json actionToJson(Action *action, InputContext *ic) {
    nlohmann::json j;
    j["id"] = action->id();
    j["desc"] = action->shortText(ic);
    j["icon"] = action->icon(ic);
    if (action->isSeparator()) {
        j["separator"] = true;
    }
    if (action->isCheckable()) {
        bool checked = action->isChecked(ic);
        j["checked"] = checked;
    }
    if (auto *menu = action->menu()) {
        for (auto *subaction : menu->actions()) {
            j["children"].emplace_back(actionToJson(subaction, ic));
        }
    }
    return j;
}

void WebKeyboard::updateStatusArea(InputContext *ic) {
    nlohmann::json actions = nlohmann::json::array();
    auto &statusArea = ic->statusArea();
    for (auto *action : statusArea.allActions()) {
        if (!action->id()) {
            // Not registered with UI manager.
            continue;
        }
        actions.emplace_back(actionToJson(action, ic));
    }
    notify_main_async(json{{"type", "STATUS_AREA"}, {"data", actions}}.dump());
}
} // namespace fcitx

FCITX_ADDON_FACTORY_V2(webkeyboard, fcitx::WebKeyboardFactory)
