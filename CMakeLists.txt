cmake_minimum_required(VERSION 3.16)
project(paranoidFilteroid VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# VS2026 compatibility and exception handling
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/EHsc)  # Enable exception handling
    add_compile_options(/permissive-)
endif()

# IMPORTANT: Disable VST2 BEFORE adding JUCE subdirectory
set(JUCE_BUILD_VST2 OFF CACHE BOOL "Build VST2" FORCE)

# Add JUCE as subdirectory
# UPDATE THIS PATH to match your JUCE installation
add_subdirectory(C:/dev/JUCE JUCE)

# Create plugin target
juce_add_plugin(paranoidFilteroid
    PRODUCT_NAME "paranoidFilteroid"
    COMPANY_NAME "RetardedMuffin"
    VERSION 1.0.0
    DESCRIPTION "Radio/phone voice-like effect VST plugin"
    
    # Plugin identifiers (4 chars each, must be unique)
    PLUGIN_MANUFACTURER_CODE Rmff
    PLUGIN_CODE Pfld
    
    # Audio format - VST3 ONLY (no VST2)
    FORMATS VST3
    
    # Plugin type configuration
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    
    # Build options
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST_COPY_DIR "C:/Program Files/Common Files/VST3"
)

# Add source files
target_sources(paranoidFilteroid PRIVATE
    Source/core/PluginProcessor.cpp
    Source/core/PluginEditor.cpp
)

# Include directories
target_include_directories(paranoidFilteroid PRIVATE
    Source/
)

# Link JUCE modules
target_link_libraries(paranoidFilteroid PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
)

target_compile_definitions(paranoidFilteroid PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Set compiler flags
if(MSVC)
    target_compile_options(paranoidFilteroid PRIVATE /W4)
else()
    target_compile_options(paranoidFilteroid PRIVATE -Wall -Wextra)
endif()
