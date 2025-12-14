//
// Created by damie on 14/12/2025.
//

#ifndef EXPENSIVECOMPONENT_H
#define EXPENSIVECOMPONENT_H

#include <deque>
#include <juce_gui_basics/juce_gui_basics.h>
#include <random>

class ExpensiveComponent : public juce::Component
{
public:

    ExpensiveComponent()
    {
        addAndMakeVisible(mSlider);
        mSlider.setRange(0.0, 1.0);
        mSlider.setValue(0.5);
    }

    void paint(juce::Graphics& g) override
    {
        // Update simulation
        mPhase += 0.1f;
        mPaths.push_back(generatePath(mPhase));
        if (mPaths.size() > 60)
            mPaths.pop_front();

        // Clear background
        g.fillAll(juce::Colours::black);

        // Create a gradient for filling
        juce::ColourGradient gradient_fill(juce::Colours::red.withAlpha(0.1f),
                                           0.0f,
                                           0.0f,
                                           juce::Colours::blue.withAlpha(0.1f),
                                           (float)getWidth(),
                                           (float)getHeight(),
                                           true);

        juce::ColourGradient gradient_stroke(juce::Colours::white.withAlpha(0.5f),
                                             0.0f,
                                             0.0f,
                                             juce::Colours::yellow.withAlpha(0.5f),
                                             (float)getWidth(),
                                             (float)getHeight(),
                                             true);

        int i = 0;

        for (auto& path : mPaths) {
            if (i == mPaths.size() - 1) {
                g.setColour(juce::Colours::black);
                g.fillPath(path);
            } else {
                g.setGradientFill(gradient_fill);
                g.fillPath(path);
                g.setGradientFill(gradient_stroke);
                g.strokePath(path, juce::PathStrokeType(1.0f));
            }

            // Shift towards upper right: 2 pixels right, 1 pixel up (negative Y)
            path.applyTransform(juce::AffineTransform::translation(2.5f, -1.5f));

            i++;
        }
    }

    void resized() override
    {
        mSlider.setBounds(getLocalBounds());
        mPaths.clear();
    }

private:

    juce::Path generatePath(float phase)
    {
        juce::Path path;
        auto w = (float)getWidth();
        auto h = (float)getHeight();
        auto midY = h * 0.5f;
        // Random number generator for noise
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

        path.startNewSubPath(0.0f, midY);

        // One point per pixel
        for (float x = 1.0f; x <= w; x += 1.0f) {
            // Basic sine wave
            float sineY = std::sin(x * 0.05f + phase) * (h * 0.4f);

            // Add noise
            float noise = dis(gen);

            path.lineTo(x, midY + sineY + 0.1 * noise);
        }

        // Close the path to make it fillable (down to bottom right, then bottom left)
        path.lineTo(w, h);
        path.lineTo(0.0f, h);
        path.closeSubPath();
        return path;
    }

    juce::Slider mSlider;
    std::deque<juce::Path> mPaths;
    float mPhase = 0.0f;

    juce::VBlankAttachment mAttachment { this, [this]() { repaint(); } };
};

#endif //EXPENSIVECOMPONENT_H
