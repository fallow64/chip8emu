cmake_minimum_required(VERSION 3.15)

project(chip8emu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to select platform
option(USE_QT "Build with Qt platform instead of SDL3" OFF)

# Core emulator sources (shared between platforms)
set(CORE_SOURCES src/chip8emu.cpp)

# Include directories
set(INCLUDE_DIRS src)

# Compiler warnings
if (MSVC)
    set(WARNING_FLAGS /W4 /WX)
else()
    set(WARNING_FLAGS -Wall -Wextra -pedantic -Werror)
    set(SANITIZER_FLAGS -fsanitize=address)
endif()

if (USE_QT)
    # Qt build
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt6 REQUIRED COMPONENTS Widgets)

    set(QT_SOURCES src/qt)

    add_executable(chip8emu_qt ${CORE_SOURCES} ${QT_SOURCES})
    target_include_directories(chip8emu_qt PRIVATE ${INCLUDE_DIRS})
    target_link_libraries(chip8emu_qt PRIVATE Qt6::Widgets)

    if (MSVC)
        target_compile_options(chip8emu_qt PRIVATE ${WARNING_FLAGS})
    else()
        target_compile_options(chip8emu_qt PRIVATE ${WARNING_FLAGS} ${SANITIZER_FLAGS})
        target_link_options(chip8emu_qt PRIVATE ${SANITIZER_FLAGS})
    endif()

else()
    # SDL3 build (default)
    set(SDL_SOURCES src/sdl3)

    add_executable(chip8emu ${CORE_SOURCES} ${SDL_SOURCES})
    target_include_directories(chip8emu PRIVATE ${INCLUDE_DIRS})

    # Link SDL3
    add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)
    target_link_libraries(chip8emu PRIVATE SDL3::SDL3)

    if (MSVC)
        target_compile_options(chip8emu PRIVATE ${WARNING_FLAGS})
    else()
        target_compile_options(chip8emu PRIVATE ${WARNING_FLAGS} ${SANITIZER_FLAGS})
        target_link_options(chip8emu PRIVATE ${SANITIZER_FLAGS})
    endif()
endif()
