#include <fcitx/instance.h>
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
} // namespace fcitx
