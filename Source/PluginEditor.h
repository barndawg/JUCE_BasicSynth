/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor&);
    ~BasicSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox waveformMenu;
    juce::Slider gainSlider;
    
    BasicSynthAudioProcessor& audioProcessor;
    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment waveformMenuAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSynthAudioProcessorEditor)
};
