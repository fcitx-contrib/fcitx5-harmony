#pragma once

#include <string>
#include <sstream>

#include "pugixml.hpp"

inline std::string escape_html(const std::string &content) {
    std::ostringstream oss;
    pugi::xml_document doc;
    doc.append_child(pugi::node_pcdata).set_value(content);
    doc.print(oss, "", pugi::format_raw);
    return oss.str();
}
