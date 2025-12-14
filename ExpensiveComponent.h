//
// Created by damie on 14/12/2025.
//

#ifndef EXPENSIVECOMPONENT_H
#define EXPENSIVECOMPONENT_H

#include <deque>
#include <juce_gui_basics/juce_gui_basics.h>

class ExpensiveComponent : public juce::Component
{
public:

    ExpensiveComponent()
    {
        addAndMakeVisible(mSlider);
        mSlider.setRange(5, 100, 1);
        mSlider.setValue(20);
    }

    void paint(juce::Graphics& g) override
    {
        auto start = juce::Time::getMillisecondCounterHiRes();

        // Update simulation
        mPhase += 0.1f;
        mPaths.push_back(generatePath(mPhase));

        while (mPaths.size() > mSlider.getValue())
            mPaths.pop_front();

        // Clear background
        g.fillAll(juce::Colours::black);

        // Create a gradient for filling
        juce::ColourGradient gradient_fill(
            juce::Colours::red, 0.0f, 0.0f, juce::Colours::blue, (float)getWidth(), (float)getHeight(), false);

        juce::ColourGradient gradient_stroke(
            juce::Colours::white, 0.0f, 0.0f, juce::Colours::yellow, (float)getWidth(), (float)getHeight(), false);

        for (auto& path : mPaths) {
            g.setGradientFill(gradient_fill);
            g.fillPath(path);
            g.setGradientFill(gradient_stroke);
            g.strokePath(path, juce::PathStrokeType(1.0f));

            // Shift towards upper right: 2 pixels right, 1 pixel up (negative Y)
            path.applyTransform(juce::AffineTransform::translation(2.5f, -1.5f));
        }

        auto end = juce::Time::getMillisecondCounterHiRes();

        mPaintDuration.push_back(end - start);
        while (mPaintDuration.size() > 10)
            mPaintDuration.pop_front();

        // Calculate average paint time
        double avgTime =
            std::accumulate(mPaintDuration.begin(), mPaintDuration.end(), 0.0) / (double)mPaintDuration.size();

        mInterPaintDurations.push_back(start - mPaintStart);
        while (mInterPaintDurations.size() > 10)
            mInterPaintDurations.pop_front();

        double avgInterPaintTime = std::accumulate(mInterPaintDurations.begin(), mInterPaintDurations.end(), 0.0)
                                 / (double)mInterPaintDurations.size();

        g.setColour(juce::Colours::white);
        g.drawText("Avg paint time: " + juce::String(avgTime, 2)
                       + " ms,     Time since last paint: " + juce::String(avgInterPaintTime, 2) + " ms",
                   getLocalBounds(),
                   juce::Justification::topLeft,
                   false);

        mPaintStart = start;
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

        path.startNewSubPath(0.0f, midY);

        // One point per pixel
        for (float x = 1.0f; x <= w; x += 1.0f) {
            // Basic sine wave
            float sineY = std::sin(x * 0.05f + phase) * (h * 0.4f);

            path.lineTo(x, midY + sineY);
        }

        // Close the path to make it fillable (down to bottom right, then bottom left)
        path.lineTo(w, h);
        path.lineTo(0.0f, h);
        path.closeSubPath();
        return path;
    }

    juce::Slider mSlider;
    std::deque<juce::Path> mPaths;
    std::deque<double> mPaintDuration;

    std::deque<double> mInterPaintDurations;

    float mPhase = 0.0f;

    double mPaintStart = 0.0;

    juce::VBlankAttachment mAttachment { this, [this]() { repaint(); } };
};

#endif //EXPENSIVECOMPONENT_H
