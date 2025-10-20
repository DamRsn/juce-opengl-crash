cmake_minimum_required(VERSION 3.22)

project(SimpleEffect VERSION 1.0.0)

# Add JUCE as a subdirectory
add_subdirectory(JUCE)

# Create the plugin target
juce_add_plugin(SimpleEffect
    VERSION 1.0.0
    COMPANY_NAME "MyCompany"
    PLUGIN_MANUFACTURER_CODE MyCo
    PLUGIN_CODE MAEf
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "SimpleEffect"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE)

# Add source files
target_sources(SimpleEffect
    PRIVATE
        PluginProcessor.cpp
        PluginEditor.cpp)

# Set compile definitions
target_compile_definitions(SimpleEffect
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

# Link libraries
target_link_libraries(SimpleEffect
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
