#pragma once

#include "PluginProcessor.h"

//==============================================================================
class SimpleEffectAudioProcessorEditor final
    : public juce::AudioProcessorEditor {
public:
  explicit SimpleEffectAudioProcessorEditor(SimpleEffectAudioProcessor &);
  ~SimpleEffectAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  SimpleEffectAudioProcessor &processorRef;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEffectAudioProcessorEditor)
};
