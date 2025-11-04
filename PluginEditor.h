#pragma once

#include "DummyCachedComponent.h"
#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

//==============================================================================
class SimpleEffectAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:

    explicit SimpleEffectAudioProcessorEditor(SimpleEffectAudioProcessor&);
    ~SimpleEffectAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void checkRenderEngine();

    void parentHierarchyChanged() override;

private:

    SimpleEffectAudioProcessor& processorRef;

    bool mUseOpenGL = false;

    juce::OpenGLContext mOpenGLContext;

    std::unique_ptr<juce::TextButton> mOpenGLButton;
    std::unique_ptr<juce::TextButton> mSetBufferedToImageButton;
    std::unique_ptr<juce::TextButton> mInvalidateCachedImageButton;

    std::unique_ptr<DummyCachedComponent> mDummyCachedComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEffectAudioProcessorEditor)
};
