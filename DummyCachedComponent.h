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

    DummyCachedComponent()
    {
        setRepaintsOnMouseActivity(true);

        startTimerHz(30);
    }

    void resized() override
    {
        // setBufferedToImage(true);
    }

    void timerCallback() override
    {
        if (r.nextFloat() < 0.05f) {
            mBufferedImage = !mBufferedImage;
            DBG("setBufferedToImage: " << (mBufferedImage ? "true" : "false"));
            setBufferedToImage(mBufferedImage);
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto n = 50;
        auto d = 20;
        auto step = (getWidth() - d) / n;

        if (isMouseOver()) {
            g.setGradientFill(
                juce::ColourGradient::horizontal(juce::Colours::red, 0.0f, juce::Colours::yellow, float(getWidth())));
        } else {
            g.setGradientFill(
                juce::ColourGradient::horizontal(juce::Colours::white, 0.0f, juce::Colours::green, float(getWidth())));
        }

        for (int i = 0; i < n; ++i) {
            g.drawEllipse(float(i * step), float(getHeight() - d) / 2.0f, float(d), float(d), 1.0f);
        }

        g.drawText("Some text to display", getLocalBounds(), juce::Justification::centredBottom, false);
    }

private:

    juce::Random r;

    bool mBufferedImage = false;
};

#endif //SIMPLEEFFECT_DUMMYCACHEDCOMPONENT_H
