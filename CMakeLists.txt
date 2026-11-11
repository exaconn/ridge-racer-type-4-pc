cmake_minimum_required(VERSION 3.21..3.31)
if(CMAKE_HOST_APPLE AND NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET
   AND (NOT DEFINED CMAKE_SYSTEM_NAME OR CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0" CACHE STRING "Minimum supported macOS version")
endif()
project(ridge-racer-type-4-pc VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_TESTING "Build native game tests" OFF)
option(RR4_BUILD_GAME "Compile the game modules (src/main, src/overlays) into rr4_game" ON)
include(CTest)

set(PSYZ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/psyz)
add_subdirectory(${PSYZ_DIR}/psyz ${CMAKE_CURRENT_BINARY_DIR}/psyz)
target_compile_definitions(psyz PRIVATE __psyz)

# Shared workspaces and vehicle/track records are accessed through multiple
# typed layout views and rely on two's-complement wrapping arithmetic.
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options("$<$<COMPILE_LANGUAGE:C>:-Werror=pointer-to-int-cast>"
        "$<$<COMPILE_LANGUAGE:C>:-Werror=int-to-pointer-cast>"
        "$<$<COMPILE_LANGUAGE:C>:-Werror=int-conversion>"
        "$<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>")
    add_compile_options("$<$<COMPILE_LANGUAGE:C>:-fno-strict-aliasing>"
        "$<$<COMPILE_LANGUAGE:C>:-fwrapv>"
        "$<$<COMPILE_LANGUAGE:C>:-ffunction-sections>"
        "$<$<COMPILE_LANGUAGE:C>:-fdata-sections>")
    if(NOT WIN32)
        add_compile_options("$<$<COMPILE_LANGUAGE:C>:-fno-omit-frame-pointer>")
    endif()
elseif(MSVC)
    add_compile_options("$<$<COMPILE_LANGUAGE:C>:/Gy>" "$<$<COMPILE_LANGUAGE:C>:/Gw>")
endif()

if(APPLE)
    add_link_options(-Wl,-dead_strip)
elseif(NOT MSVC)
    add_link_options(-Wl,--gc-sections)
else()
    add_link_options(/OPT:REF)
endif()

# Game modules: the resident program (src/main, including the game-coupled
# SDK helpers in src/main/psyq that PsyZ lacks), the statically linked
# overlays and the native host services they use.
if(RR4_BUILD_GAME)
    file(GLOB_RECURSE RR4_GAME_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/overlays/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/native/*.c)
    add_library(rr4_game STATIC ${RR4_GAME_SOURCES})
    target_include_directories(rr4_game
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        SYSTEM PRIVATE
            ${PSYZ_DIR}/psyz/include
    )
    target_compile_definitions(rr4_game PUBLIC __psyz)
    target_link_libraries(rr4_game PUBLIC psyz)
endif()

# Host services of the desktop build: preferences and the Escape settings
# menu (model, CPU raster, SDL_GPU overlay glue), shared with the tests.
add_library(rr4_host STATIC
    src/port/preferences.c
    src/port/settings_model.c
    src/port/settings_raster.c
    src/port/settings_overlay.c
)
target_include_directories(rr4_host
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/port
    SYSTEM PRIVATE
        ${PSYZ_DIR}/psyz/include
        ${PSYZ_DIR}/external/SDL/src/render/gpu/shaders
)
target_compile_definitions(rr4_host PRIVATE __psyz)
target_link_libraries(rr4_host PUBLIC psyz)

add_executable(ridge-racer-type-4-pc
    src/port/main.c
)
target_include_directories(ridge-racer-type-4-pc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    SYSTEM PRIVATE
        ${PSYZ_DIR}/psyz/include
)
target_compile_definitions(ridge-racer-type-4-pc PRIVATE __psyz)
target_link_libraries(ridge-racer-type-4-pc PRIVATE rr4_host psyz)
if(RR4_BUILD_GAME)
    # src/port/main.c does not enter MainLoop yet, so the archive is built and
    # linked but contributes no objects until the host entry calls the game.
    target_link_libraries(ridge-racer-type-4-pc PRIVATE rr4_game)
endif()
psyz_title(ridge-racer-type-4-pc "Ridge Racer Type 4")

if(BUILD_TESTING)
    # Settings menu and preferences, exercised headlessly.
    add_executable(rr4_settings_test tests/port/settings_test.c)
    target_include_directories(rr4_settings_test SYSTEM PRIVATE ${PSYZ_DIR}/psyz/include)
    target_compile_definitions(rr4_settings_test PRIVATE __psyz)
    target_link_libraries(rr4_settings_test PRIVATE rr4_host psyz)
    add_test(NAME settings COMMAND rr4_settings_test ${CMAKE_CURRENT_BINARY_DIR}/settings-test)
    set_tests_properties(settings PROPERTIES ENVIRONMENT "PSYZ_HEADLESS=1;SDL_AUDIODRIVER=dummy")
endif()
