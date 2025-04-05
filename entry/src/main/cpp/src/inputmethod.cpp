#include <fcitx/instance.h>
#include <fcitx/inputmethodentry.h>
#include <fcitx/inputmethodmanager.h>
#include "inputmethod.h"

namespace fcitx {
extern std::unique_ptr<Instance> instance;
void setInputMethods(const std::vector<std::string> &inputMethods) {
    auto &imMgr = instance->inputMethodManager();
    auto group = imMgr.currentGroup();
    auto &imList = group.inputMethodList();
    imList.clear();
    for (const auto inputMethod : inputMethods) {
        imList.emplace_back(inputMethod);
    }
    imMgr.setGroup(group);
    imMgr.save();
}

static nlohmann::json imToJson(const InputMethodEntry *entry) {
    nlohmann::json j;
    j["name"] = entry->uniqueName();
    j["displayName"] = entry->nativeName() != "" ? entry->nativeName()
                       : entry->name() != ""     ? entry->name()
                                                 : entry->uniqueName();
    j["languageCode"] = entry->languageCode();
    return j;
}

nlohmann::json getInputMethodsJson() {
    auto j = nlohmann::json::array();
    auto &imMgr = instance->inputMethodManager();
    auto group = imMgr.currentGroup();
    for (const auto &im : group.inputMethodList()) {
        auto entry = imMgr.entry(im.name());
        if (!entry)
            continue;
        j.push_back(imToJson(entry));
    }
    return j;
}
} // namespace fcitx
