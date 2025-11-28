cmake_minimum_required(VERSION 3.16)
project(paranoidFilteroid VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    
    # Audio format
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

# Set compiler flags
if(MSVC)
    target_compile_options(paranoidFilteroid PRIVATE /W4)
else()
    target_compile_options(paranoidFilteroid PRIVATE -Wall -Wextra)
endif()
