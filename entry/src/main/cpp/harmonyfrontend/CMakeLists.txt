add_library(harmonyfrontend STATIC harmonyfrontend.cpp)
target_link_libraries(harmonyfrontend Fcitx5::Core nlohmann_json::nlohmann_json)

configure_file(harmonyfrontend.conf.in.in harmonyfrontend.conf.in @ONLY)
fcitx5_translate_desktop_file(${CMAKE_CURRENT_BINARY_DIR}/harmonyfrontend.conf.in harmonyfrontend.conf)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/harmonyfrontend.conf DESTINATION "${CMAKE_INSTALL_PREFIX}/share/fcitx5/addon" COMPONENT config)
