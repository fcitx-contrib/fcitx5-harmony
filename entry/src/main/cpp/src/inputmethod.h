#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace fcitx {
void setInputMethods(const std::vector<std::string> &inputMethods);
nlohmann::json getInputMethodsJson();
} // namespace fcitx
