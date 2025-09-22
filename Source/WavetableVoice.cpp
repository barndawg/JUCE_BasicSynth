#include "WavetableVoice.h"
#include <cmath>

WavetableVoice::WavetableVoice(BasicSynthAudioProcessor& p, std::vector<float>& sint, std::vector<float>& sawt, std::vector<float>& trit)
    : audioProcessor(p), sineTable(sint), sawTable(sawt), triTable(trit)
{}

bool WavetableVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return true;
}

void WavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    index = 0.0f;
    auto freq = 440.f * std::pow(2.0f, (midiNoteNumber - 69) / 12.0f);
    indexIncrement = freq * tableSize / sampleRate;
    
    // Envelope - start attack
    envelopePhase = Attack;
    envelopeValue = 0.0f;
    attackIncrement = 1.f / (attackTime * sampleRate);
    releaseIncrement = 1.f / (releaseTime * sampleRate);
    
}

void WavetableVoice::stopNote(float velocity, bool allowTailOff)
{
    envelopePhase = Release;
}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (envelopePhase == Idle) return;
    
    auto* left = outputBuffer.getWritePointer(0, startSample);
    auto* right = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;
    
    param_gain = std::pow(10.0f, (-60.0f + 60.0f * getParam("gain")) / 20.0f);
    param_waveform = getParam("waveform");

    for (int i = 0; i < numSamples; ++i)
    {
        switch (envelopePhase)
        {
            case Attack:
                envelopeValue += attackIncrement;
                if (envelopeValue >= 1.0f)
                {
                    envelopeValue = 1.0f;
                    envelopePhase = Sustain;
                }
                break;
            case Sustain:
                break;
            case Release:
                envelopeValue -= releaseIncrement;
                if (envelopeValue <= 0.0f)
                {
                    envelopeValue = 0.0f;
                    envelopePhase = Idle;
                    playing = false;
                    clearCurrentNote();
                }
                break;
            case Idle:
                break;
            default:
                break;
        }
        
        float sample = getSample() * envelopeValue * param_gain;

        left[i] += sample;
        if (right) right[i] += sample;

        index += indexIncrement;
        if (index >= tableSize)
            index -= tableSize;
    }
}

float WavetableVoice::getSample()
{
    std::vector<float>& currentTable = (param_waveform == 1) ? sawTable :
                                        (param_waveform == 2) ? triTable :
                                        sineTable;
    
    auto truncatedIndex = static_cast<int>(index);
    auto nextIndex = (truncatedIndex + 1) % tableSize;
    auto frac = index - truncatedIndex;
    float sample = currentTable[truncatedIndex] * (1.0f - frac) + currentTable[nextIndex] * frac;
    float scaledSample = sample * 0.5;
    return scaledSample;
}

float WavetableVoice::getParam(const juce::String& paramID) const
{
    if (auto* param = audioProcessor.apvts.getRawParameterValue(paramID))
        return param -> load();
    else jassertfalse;
    return 0.f;
}
