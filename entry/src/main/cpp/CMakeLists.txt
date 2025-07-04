cmake_minimum_required(VERSION 3.24)
project(fcitx5-harmony VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-fexperimental-library) # TODO: check if clang 16+ needs it.

set(PREBUILT_ROOT "${PROJECT_SOURCE_DIR}/prebuilt/${OHOS_ARCH}")
set(PREBUILT_USR "${PREBUILT_ROOT}/usr")
set(CMAKE_INSTALL_PREFIX "/usr")

# fcitx5-harmony/entry/.cxx/default/default/debug/hvigor/x86_64 this is also PROJECT_BINARY_DIR
# C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native
# C:/Program Files/Huawei/DevEco Studio/sdk/default/hms/native/sysroot/usr
list(APPEND CMAKE_FIND_ROOT_PATH "${PREBUILT_USR}")

if (CMAKE_HOST_WIN32)
    set(PKG_CONFIG_EXECUTABLE "C:/msys64/usr/bin/pkg-config.exe" CACHE FILEPATH "" FORCE)
endif()
set(ENV{PKG_CONFIG_SYSROOT_DIR} "${PREBUILT_ROOT}")
set(ENV{PKG_CONFIG_PATH} "${PREBUILT_USR}/lib/pkgconfig")

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
option(USE_SYSTEMD "" OFF)
option(ENABLE_XDGAUTOSTART "" OFF)
option(ENABLE_EMOJI "" OFF)
option(ENABLE_LIBUUID "" OFF)
option(BUILD_SPELL_DICT "" OFF)

# Override iso-codes paths and xkb default rules file
set(ISOCODES_ISO3166_JSON "${CMAKE_INSTALL_PREFIX}/share/iso-codes/json/iso_3166-1.json")
set(ISOCODES_ISO639_JSON "${CMAKE_INSTALL_PREFIX}/share/iso-codes/json/iso_639-3.json")
set(XKEYBOARDCONFIG_XKBBASE "${CMAKE_INSTALL_PREFIX}/share/xkeyboard-config-2")
set(XKEYBOARDCONFIG_DATADIR "${CMAKE_INSTALL_PREFIX}/share")

add_subdirectory(fcitx5)
target_link_libraries(Fcitx5Utils PRIVATE c++experimental) # TODO: check if clang 16+ needs it.

option(ENABLE_DATA "" OFF)
option(ENABLE_TOOLS "" OFF)
add_subdirectory(libime)

option(USE_DLOPEN "" OFF)
add_subdirectory(fcitx5-lua)
target_link_libraries(luaaddonloader "-L${PREBUILT_USR}/lib" lua)

option(ENABLE_GUI "" OFF)
set(OPENCC_INCLUDE_DIR "${PREBUILT_USR}/include/opencc")
add_subdirectory(fcitx5-chinese-addons)
target_link_libraries(chttrans "-L${PREBUILT_USR}/lib" marisa)

find_package(nlohmann_json)
add_subdirectory(harmonyfrontend)
add_subdirectory(webkeyboard)
add_subdirectory(src)

add_library(entry SHARED napi_init.cpp)
target_include_directories(entry PRIVATE src)
target_link_libraries(entry PRIVATE fcitx)
target_link_libraries(entry PUBLIC libace_napi.z.so)

add_custom_command(TARGET entry
    POST_BUILD COMMAND
    node "${PROJECT_SOURCE_DIR}/../../../../scripts/postbuild.mjs" "${PROJECT_SOURCE_DIR}/../resources/resfile" "${PROJECT_BINARY_DIR}" "${PREBUILT_USR}"
)
