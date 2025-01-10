add_library(fcitx SHARED
    fcitx.cpp
    keycode.cpp
    inputmethod.cpp
)
target_link_libraries(fcitx Fcitx5::Core libhilog_ndk.z.so)

fcitx5_import_addons(fcitx
    REGISTRY_VARNAME getStaticAddon
    ADDONS harmonyfrontend
)
