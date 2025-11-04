#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SimpleEffectAudioProcessorEditor::SimpleEffectAudioProcessorEditor(SimpleEffectAudioProcessor& p)
: AudioProcessorEditor(&p),
  processorRef(p)
{
    juce::ignoreUnused(processorRef);

    mUseOpenGL = true;
    bool start_with_cached_image = true;

    mOpenGLButton = std::make_unique<juce::TextButton>(mUseOpenGL ? "Disable OpenGL" : "Use OpenGL");
    mOpenGLButton->onClick = [this]() {
        mUseOpenGL = !mUseOpenGL;
        mOpenGLButton->setButtonText(mUseOpenGL ? "Disable OpenGL" : "Use OpenGL");
        checkRenderEngine();
    };
    addAndMakeVisible(mOpenGLButton.get());

    mSetBufferedToImageButton = std::make_unique<juce::TextButton>("Set Buffered To Image");
    mSetBufferedToImageButton->onClick = [this] {
        mDummyCachedComponent->changeBufferedToImage();
        repaint();

        mInvalidateCachedImageButton->setEnabled(mDummyCachedComponent->isUsingBufferedImage());
    };
    addAndMakeVisible(mSetBufferedToImageButton.get());

    mDummyCachedComponent = std::make_unique<DummyCachedComponent>(start_with_cached_image);
    addAndMakeVisible(mDummyCachedComponent.get());

    mInvalidateCachedImageButton = std::make_unique<juce::TextButton>("Invalidate");
    mInvalidateCachedImageButton->onClick = [this] {
        jassert(mDummyCachedComponent->isUsingBufferedImage());
        jassert(mDummyCachedComponent->getCachedComponentImage() != nullptr);
        mDummyCachedComponent->getCachedComponentImage()->invalidateAll();
    };
    mInvalidateCachedImageButton->setEnabled(mDummyCachedComponent->isUsingBufferedImage());
    addAndMakeVisible(mInvalidateCachedImageButton.get());

    setSize(400, 300);
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

    if (mUseOpenGL) {
        g.drawFittedText(
            "Using OpenGL Rendering", juce::Rectangle(0, 60, getWidth() / 2, 20), juce::Justification::centredLeft, 1);
    } else {
        g.drawFittedText("NOT using OpenGL Rendering",
                         juce::Rectangle(0, 60, getWidth() / 2, 20),
                         juce::Justification::centredRight,
                         1);
    }

    if (mDummyCachedComponent->isUsingBufferedImage()) {
        g.drawFittedText("Using Buffered Image",
                         juce::Rectangle(getWidth() / 2, 60, getWidth() / 2, 20),
                         juce::Justification::centred,
                         1);
    } else {
        g.drawFittedText("NOT Using Buffered Image",
                         juce::Rectangle(getWidth() / 2, 60, getWidth() / 2, 20),
                         juce::Justification::centred,
                         1);
    }
}

void SimpleEffectAudioProcessorEditor::resized()
{
    mOpenGLButton->setBounds(10, 10, 100, 30);
    mSetBufferedToImageButton->setBounds(getWidth() - 160, 10, 150, 30);
    mInvalidateCachedImageButton->setBounds(getWidth() - 160, 100, 150, 30);

    mDummyCachedComponent->setBounds(10, 200, getWidth() - 20, getHeight() - 210);
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
    int renderer_idx = 1;
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
