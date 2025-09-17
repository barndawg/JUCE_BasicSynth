/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicSynthAudioProcessorEditor::BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), gainSliderAttachment (audioProcessor.getState(), "gain", gainSlider)
{
    gainSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible (gainSlider);
    
    setSize (400, 300);
}

BasicSynthAudioProcessorEditor::~BasicSynthAudioProcessorEditor()
{
}

//==============================================================================
void BasicSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void BasicSynthAudioProcessorEditor::resized()
{
    gainSlider.setBounds (getWidth() / 2 - 100, getHeight() / 2 - 100, 200, 200);
}
