add_library(fcitx STATIC
    fcitx.cpp
)
target_link_libraries(fcitx Fcitx5::Core)
