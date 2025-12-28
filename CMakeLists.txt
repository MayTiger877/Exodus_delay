cmake_minimum_required(VERSION 4.2)

project(ExodusDelay VERSION 1.0.0)

# Enable JUCE modules
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# First, we'll add the CMake folder, incase we'll need to find_package later:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

# Compile commands, useful for some IDEs like VS-Code
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Minimum MacOS target, set globally
if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0 CACHE STRING "Minimum OS X deployment version" FORCE)
else ()
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version" FORCE)
endif ()

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif ()

# Static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set this to the path where JUCE was cloned by GitHub Actions or locally
# set(JUCE_DIR "${CMAKE_SOURCE_DIR}/../JUCE") # my pc
# set(JUCE_DIR "${CMAKE_SOURCE_DIR}/JUCE") # CI pipeline

if(EXISTS "${CMAKE_SOURCE_DIR}/../JUCE")
    set(JUCE_DIR "${CMAKE_SOURCE_DIR}/../JUCE")
    message(STATUS "Using JUCE from parent directory")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/JUCE")
    set(JUCE_DIR "${CMAKE_SOURCE_DIR}/JUCE")
    message(STATUS "Using JUCE from current directory")
else()
    message(FATAL_ERROR "JUCE directory not found in either location")
endif()

# Add JUCE to the project directly (this is the standard way for JUCE projects)
add_subdirectory(${JUCE_DIR} JUCE)

juce_add_plugin(ExodusDelay
    COMPANY_NAME "MayTigerPlugins"
    IS_SYNTH FALSE
    IS_EFFECT TRUE
    IS_MIDI_EFFECT FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE ExDe
    FORMATS VST3
    PRODUCT_NAME "ExodusDelay"
)

juce_generate_juce_header(ExodusDelay)

target_sources(ExodusDelay PRIVATE
    Source/
    Source/DelayEngine.h
    Source/DelayEngine.cpp
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
    Source/Parameters.h
    Source/Configs.h
    Source/MyReverb.h
    Source/MyReverb.cpp
    Source/MyDistortion.h
    Source/MyDistortion.cpp
    Source/MyPhaser.h
    Source/MyPhaser.cpp
    Source/TileSlider.h
    Source/TileSlider.cpp
    Source/PhaserKnob.h
    Source/PhaserKnob.cpp
    Source/DistKnob.h
    Source/DistKnob.cpp
    Source/CostumeKnob.h
    Source/CostumeKnob.cpp
)

juce_add_binary_data(ResourcesData SOURCES
    Resources/bg.svg
    Resources/Knob.svg
    Resources/DistKnob.svg
    Resources/TileBG.svg
    Resources/TileShnatot.svg
    Resources/Vent.svg
    Resources/BrushedMetal_Texture.png
    Resources/black_wood_texture.png
    Resources/green_Slider.svg
    Resources/yellow_Slider.svg
    Resources/purple_Slider.svg
    Resources/orange_Slider.svg
    Resources/Red_Slider.svg
    Resources/triangle_lines_choose.svg
    Resources/aquere_lines_choose.svg
    Resources/sine_line_choose.svg
    Resources/saw_up_lines_choose.svg
    Resources/saw_down_lines_choose.svg
)

# # Get all preset files automatically
# file(GLOB PRESET_FILES "Presets/*.preset")
# juce_add_binary_data(PresetsData SOURCES ${PRESET_FILES})

target_link_libraries(ExodusDelay PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_processors_headless
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_dsp
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    ResourcesData
)

# Fix VST3 parameter automation conflict
target_compile_definitions(ExodusDelay PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Safe clean target that doesn't remove the directory it's running from
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/BallPit_artefacts
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/_deps
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/*.vcxproj
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/*.vcxproj.filters
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/*.sln
    COMMENT "Cleaning all build artifacts and CMake generated files"
)

# cmake -B build -G "Visual Studio 18 2026" -DCMAKE_BUILD_TYPE=Release
# cmake --build build --config Release