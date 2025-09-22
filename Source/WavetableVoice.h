#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"

class WavetableVoice : public juce::SynthesiserVoice
{
public:
    WavetableVoice(BasicSynthAudioProcessor&, std::vector<float>& sint, std::vector<float>& sawt, std::vector<float>& trit);
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override {}
    void controllerMoved(int controllerNumber, int newControllerValue) override {}

private:
    BasicSynthAudioProcessor& audioProcessor;
    
    // Getter functions
    float getSample();
    float getParam(const juce::String& paramID) const;
    
    // Wavetable types
    std::vector<float>& sineTable;
    std::vector<float>& sawTable;
    std::vector<float>& triTable;
    
    // Parameters
    float param_gain = 1.f;
    int param_waveform = 1;
    
    // Playback stuff
    int tableSize = 512;
    double sampleRate = 44100.f;
    float index = 0.f;
    float indexIncrement = 0.f;
    bool playing = false;
    
    // Envelope
    enum EnvelopePhase { Idle, Attack, Sustain, Release };
    
    EnvelopePhase envelopePhase = Idle;
    float envelopeValue = 0.f;
    float attackTime = 0.01f;
    float releaseTime = 0.2f;
    float attackIncrement = 0.f;
    float releaseIncrement = 0.f;
};
