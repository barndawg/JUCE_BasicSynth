/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicSynthAudioProcessorEditor::BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), waveformMenuAttachment(audioProcessor.getState(), "waveform", waveformMenu), gainSliderAttachment (audioProcessor.getState(), "gain", gainSlider)
{
    waveformMenu.addItem("Sine", 1);
    waveformMenu.addItem("Saw", 2);
    waveformMenu.addItem("Triangle", 3);
    waveformMenu.setSelectedId(1);
    addAndMakeVisible(waveformMenu);
    
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
    float width = getWidth();
    float height = getHeight();
    
    waveformMenu.setBounds (width*0.5 - width*0.2, height*0.05, width*0.4, height*0.1);
    
    gainSlider.setBounds (width*0.5 - width*0.4, height*0.25, width*0.8, height*0.5);
}
