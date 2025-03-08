find_package(nlohmann_json REQUIRED)

add_library(webkeyboard STATIC webkeyboard.cpp)
target_link_libraries(webkeyboard Fcitx5::Core nlohmann_json::nlohmann_json)

configure_file(webkeyboard.conf.in.in webkeyboard.conf.in @ONLY)
fcitx5_translate_desktop_file(${CMAKE_CURRENT_BINARY_DIR}/webkeyboard.conf.in webkeyboard.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/webkeyboard.conf"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/share/fcitx5/addon"
    COMPONENT config
)
