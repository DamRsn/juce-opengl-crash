//
// Created by Damien Ronssin on 21/10/2025.
//

#ifndef SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H
#define SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>

class DummyCachedComponent : public juce::Component
{
public:

    explicit DummyCachedComponent(bool shouldBufferToImageOnStart)
    {
        mBufferedImage = shouldBufferToImageOnStart;
        setBufferedToImage(mBufferedImage);
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
    }

private:

    bool mBufferedImage = false;
};

#endif //SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H
