//
// Created by Damien Ronssin on 21/10/2025.
//

#ifndef SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H
#define SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>

class DummyCachedComponent : public juce::Component,
                             juce::Timer
{
public:

    explicit DummyCachedComponent(bool shouldBufferToImageOnStart)
    {
        mBufferedImage = shouldBufferToImageOnStart;
        setBufferedToImage(mBufferedImage);
        startTimerHz(30);
    }

    void timerCallback() override
    {
        bool using_cached_real = getCachedComponentImage() != nullptr;

        if (using_cached_real != mUsingBufferedImageReal) {
            mUsingBufferedImageReal = using_cached_real;
            repaint();
        }
    }

    void resized() override {}

    void changeBufferedToImage()
    {
        mBufferedImage = !mBufferedImage;
        setBufferedToImage(mBufferedImage);
    }

    bool isUsingBufferedImage() const
    {
        return mBufferedImage;
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::white);
        g.drawRect(getLocalBounds().reduced(1), 1);
        g.drawText("Some text that can be bufferedToImage", getLocalBounds(), juce::Justification::centred, false);

        if (mUsingBufferedImageReal) {
            g.setColour(juce::Colours::green);
            g.drawText("Using Buffered Image real",
                       getLocalBounds().withTop(0).withHeight(20),
                       juce::Justification::topLeft,
                       false);
        } else {
            g.setColour(juce::Colours::red);
            g.drawText("Not Using Buffered Image real",
                       getLocalBounds().withTop(0).withHeight(20),
                       juce::Justification::topLeft,
                       false);
        }
    }

private:

    bool mBufferedImage = false;

    bool mUsingBufferedImageReal = false;
};

#endif //SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H
