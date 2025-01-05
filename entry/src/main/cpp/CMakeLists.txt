cmake_minimum_required(VERSION 3.24)
project(fcitx5-harmony)

set(PREBUILT_DIR "${PROJECT_SOURCE_DIR}/prebuilt/${OHOS_ARCH}")

# fcitx5-harmony/entry/.cxx/default/default/debug/hvigor/x86_64
# C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native
# C:Program Files/Huawei/DevEco Studio/sdk/default/hms/native/sysroot/usr
list(APPEND CMAKE_FIND_ROOT_PATH "${PREBUILT_DIR}")

find_library(UV NAMES uv)
add_library(ohos_uv SHARED IMPORTED)
set_target_properties(ohos_uv PROPERTIES
    IMPORTED_LOCATION "${UV}"
)
set(LIBUV_TARGET ohos_uv)

option(ENABLE_TESTING_ADDONS "" OFF)
option(ENABLE_TEST "" OFF)
option(ENABLE_COVERAGE "" OFF)
option(ENABLE_ENCHANT "" OFF)
option(ENABLE_X11 "" OFF)
option(ENABLE_WAYLAND "" OFF)
option(ENABLE_DBUS "" OFF)
option(ENABLE_DOC "" OFF)
option(ENABLE_SERVER "" OFF)
option(ENABLE_KEYBOARD "" OFF)
option(USE_SYSTEMD "" OFF)
option(ENABLE_XDGAUTOSTART "" OFF)
option(ENABLE_EMOJI "" OFF)
option(ENABLE_LIBUUID "" OFF)
option(BUILD_SPELL_DICT "" OFF)
add_subdirectory(fcitx5)

add_library(entry SHARED napi_init.cpp)
target_link_libraries(entry PUBLIC libace_napi.z.so)
