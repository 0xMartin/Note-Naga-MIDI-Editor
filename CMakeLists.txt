cmake_minimum_required(VERSION 3.16)
project(NoteNaga LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/.vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(Qt6 COMPONENTS Core Gui Widgets Svg REQUIRED)

# Použijeme pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(FLUIDSYNTH REQUIRED IMPORTED_TARGET fluidsynth)
pkg_check_modules(RTMIDI REQUIRED IMPORTED_TARGET rtmidi)

set(HEADER_FILES
    src/core/midi_file.h
)

set(SOURCE_FILES
    src/core/midi_file.cpp
)

add_executable(NoteNaga
    src/main.cpp
    ${HEADER_FILES}
    ${SOURCE_FILES}
    resources.qrc
)

# Všechno linkování pomocí pkg-config targetů
target_link_libraries(NoteNaga
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
    PkgConfig::GLIB2
    PkgConfig::FLUIDSYNTH
    PkgConfig::RTMIDI
)
