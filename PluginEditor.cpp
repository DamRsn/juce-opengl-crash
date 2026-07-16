#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SimpleEffectAudioProcessorEditor::SimpleEffectAudioProcessorEditor(SimpleEffectAudioProcessor& p)
: AudioProcessorEditor(&p),
  processorRef(p)
{
    juce::ignoreUnused(processorRef);

    // Default to start with
    mUseD2D = true;
    mUseOpenGL = false;

    mOpenGLButton = std::make_unique<juce::TextButton>(mUseOpenGL ? "Disable OpenGL" : "Use OpenGL");
    mOpenGLButton->onClick = [this]() {
        mUseOpenGL = !mUseOpenGL;
        mOpenGLButton->setButtonText(mUseOpenGL ? "Disable OpenGL" : "Use OpenGL");
        checkRenderEngine();
    };
    addAndMakeVisible(mOpenGLButton.get());

    mD2DButton = std::make_unique<juce::TextButton>(mUseD2D ? "Disable D2D" : "Use D2D");
    mD2DButton->onClick = [this]() {
        mUseD2D = !mUseD2D;
        mD2DButton->setButtonText(mUseD2D ? "Disable D2D" : "Use D2D");
        checkRenderEngine();
    };
    addAndMakeVisible(mD2DButton.get());

    addAndMakeVisible(mExpensiveComponent);

    mDummySlider = std::make_unique<juce::Slider>();
    mDummySlider->setRange(0.0f, 1.0f, 0.0f);
    mDummySlider->setSliderStyle(juce::Slider::LinearHorizontal);
    mDummySlider->setNumDecimalPlacesToDisplay(2);
    mDummySlider->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 10);
    addAndMakeVisible(mDummySlider.get());

    setSize(1200, 800);
}

SimpleEffectAudioProcessorEditor::~SimpleEffectAudioProcessorEditor()
{
    mOpenGLContext.detach();
}

//==============================================================================
void SimpleEffectAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);

    auto open_gl_rect = juce::Rectangle<int>(mOpenGLButton->getBounds().expanded(20).translated(0, 40));
    if (mUseOpenGL) {
        g.drawFittedText("Using OpenGL", open_gl_rect, juce::Justification::centred, 1);
    } else {
        g.drawFittedText("NOT using OpenGL", open_gl_rect, juce::Justification::centred, 1);
    }

    auto d2d_rect = juce::Rectangle<int>(mD2DButton->getBounds().expanded(20).translated(0, 40));

    if (mUseD2D) {
        g.drawFittedText("Using D2D", d2d_rect, juce::Justification::centred, 1);
    } else {
        g.drawFittedText("NOT using D2D", d2d_rect, juce::Justification::centred, 1);
    }

    // Print juce version
    g.drawFittedText("JUCE version: " + juce::String(JUCE_MAJOR_VERSION) + "." + juce::String(JUCE_MINOR_VERSION) + "."
                         + juce::String(JUCE_BUILDNUMBER),
                     getLocalBounds(),
                     juce::Justification::topRight,
                     1);

    // Print compilation date and time
    g.drawFittedText("Compiled on: " + juce::String(__DATE__) + " at " + juce::String(__TIME__),
                     getLocalBounds().removeFromBottom(getHeight() - 20),
                     juce::Justification::topRight,
                     1);
}

void SimpleEffectAudioProcessorEditor::resized()
{
    mOpenGLButton->setBounds(10, 10, 100, 30);
    mD2DButton->setBounds(150, 10, 100, 30);
    mDummySlider->setBounds(260, 45, 250, 50);
    mExpensiveComponent.setBounds(10, 100, getWidth() - 20, getHeight() - 90);
}

void SimpleEffectAudioProcessorEditor::checkRenderEngine()
{
    auto* peer = getPeer();

    if (peer == nullptr) {
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
    int renderer_idx = mUseD2D ? 1 : 0;
#elif JUCE_MAC
    int renderer_idx = 1;
#else
    int renderer_idx = 0;
#endif

    peer->setCurrentRenderingEngine(renderer_idx);

    if (mUseOpenGL) {
        mOpenGLContext.setImageCacheSize(64 * 1024 * 1024);
        mOpenGLContext.attachTo(*this);
    } else {
        mOpenGLContext.detach();
    }
}

void SimpleEffectAudioProcessorEditor::parentHierarchyChanged()
{
    checkRenderEngine();
}
