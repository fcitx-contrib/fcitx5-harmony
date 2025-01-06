add_library(fcitx STATIC
    fcitx.cpp
)
target_link_libraries(fcitx Fcitx5::Core)

fcitx5_import_addons(fcitx
    REGISTRY_VARNAME getStaticAddon
    ADDONS harmonyfrontend
)
