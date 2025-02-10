cmake_minimum_required(VERSION 3.24)
project(fcitx5-harmony VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(PREBUILT_DIR "${PROJECT_SOURCE_DIR}/prebuilt/${OHOS_ARCH}")
set(CMAKE_INSTALL_PREFIX "/usr")

# fcitx5-harmony/entry/.cxx/default/default/debug/hvigor/x86_64 this is also PROJECT_BINARY_DIR
# C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native
# C:/Program Files/Huawei/DevEco Studio/sdk/default/hms/native/sysroot/usr
list(APPEND CMAKE_FIND_ROOT_PATH "${PREBUILT_DIR}")
set(ENV{PKG_CONFIG_PATH} "${PREBUILT_DIR}/lib/pkgconfig")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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

option(ENABLE_DATA "" OFF)
option(ENABLE_TOOLS "" OFF)
add_subdirectory(libime)

option(USE_DLOPEN "" OFF)
add_subdirectory(fcitx5-lua)
target_link_libraries(luaaddonloader "-L${PREBUILT_DIR}/lib" lua)

option(ENABLE_GUI "" OFF)
set(OPENCC_INCLUDE_DIR "${PREBUILT_DIR}/include/opencc")
add_subdirectory(fcitx5-chinese-addons)
target_link_libraries(chttrans "-L${PREBUILT_DIR}/lib" marisa)

add_subdirectory(harmonyfrontend)
add_subdirectory(src)

add_library(entry SHARED napi_init.cpp)
target_include_directories(entry PRIVATE src)
target_link_libraries(entry PRIVATE fcitx)
target_link_libraries(entry PUBLIC libace_napi.z.so)

# XXX: This won't be bundled into HAP for the first run of hvigorw assembleHap.
add_custom_command(TARGET entry
    POST_BUILD  COMMAND
    node "${PROJECT_SOURCE_DIR}/../../../../scripts/postbuild.mjs" "${PROJECT_SOURCE_DIR}/../resources/resfile" "${PROJECT_BINARY_DIR}"
)
