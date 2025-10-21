#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SimpleEffectAudioProcessorEditor::SimpleEffectAudioProcessorEditor(SimpleEffectAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p)
{
    juce::ignoreUnused(processorRef);


    mOpenGLButton = std::make_unique<juce::TextButton>("Use OpenGL");
    mOpenGLButton->onClick = [this]()
    {
        mUseOpenGL = !mUseOpenGL;
        mOpenGLButton->setButtonText(mUseOpenGL ? "Disable OpenGL" : "Use OpenGL");
        checkRenderEngine();
    };
    addAndMakeVisible(mOpenGLButton.get());

    mDummyCachedComponent = std::make_unique<DummyCachedComponent>();
    addAndMakeVisible(mDummyCachedComponent.get());

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
}

SimpleEffectAudioProcessorEditor::~SimpleEffectAudioProcessorEditor()
{
    mOpenGLContext.detach();
}

//==============================================================================
void SimpleEffectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Simple Effect", getLocalBounds(), juce::Justification::centred, 1);

    if (mUseOpenGL)
    {
        g.drawFittedText(
            "Using OpenGL Rendering", getLocalBounds().withY(40).withHeight(20), juce::Justification::centred, 1);
    }
    else
    {
        g.drawFittedText(
            "NOT using OpenGL Rendering", getLocalBounds().withY(40).withHeight(20), juce::Justification::centred, 1);
    }
}

void SimpleEffectAudioProcessorEditor::resized()
{
    mOpenGLButton->setBounds(10, 10, 100, 30);

    mDummyCachedComponent->setBounds(10, 200, getWidth() - 20, getHeight() - 210);
}

void SimpleEffectAudioProcessorEditor::checkRenderEngine()
{
    auto* peer = getPeer();

    if (peer == nullptr)
    {
        return;
    }

    /**
   * NOTE: setCurrentRenderingEngine should always be called regardless of
   * whether or not we're using OpenGL. This ensures that the component peer is
   * *immediately* using our desired context and that we don't use an undesired
   * context even if OpenGL is enabled. This is because JUCE will still attempt
   * to do some top-level operations using the native context as opposed to the
   * OpenGL context.
   */
#if JUCE_WINDOWS
    int renderer_idx = 1;
#elif JUCE_MAC
    int renderer_idx = 1;
#else
    int renderer_idx = 0;
#endif

    peer->setCurrentRenderingEngine(renderer_idx);

    if (mUseOpenGL)
    {
        mOpenGLContext.setImageCacheSize(64 * 1024 * 1024);
        // Windows Ableton gets confused about the editor size if we attach to
        // getTopLevelComponent() and Ableton's "auto-scale plugin window" option is
        // enabled. See ENG-2336.
        mOpenGLContext.attachTo(*this);
    }
    else
    {
        mOpenGLContext.detach();
    }
}

void SimpleEffectAudioProcessorEditor::parentHierarchyChanged()
{
    checkRenderEngine();
}
