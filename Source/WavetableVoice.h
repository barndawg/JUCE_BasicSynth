#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"

class WavetableVoice : public juce::SynthesiserVoice
{
public:
    WavetableVoice(BasicSynthAudioProcessor&, const std::vector<float>& waveTable);
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override {}
    void controllerMoved(int controllerNumber, int newControllerValue) override {}

private:
    float getSample();
    
    std::vector<float> waveTable;
    double sampleRate = 44100.0;
    float index = 0.f;
    float indexIncrement = 0.f;
    bool playing = false;
    
    BasicSynthAudioProcessor& audioProcessor;
};
