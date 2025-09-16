#include "WavetableVoice.h"
#include <cmath>

WavetableVoice::WavetableVoice(const std::vector<float>& wt)
    : waveTable(wt)
{}

bool WavetableVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return true;
}

void WavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    playing = true;
    index = 0.0f;
    auto freq = 440.f * std::pow(2.0f, (midiNoteNumber - 69) / 12.0f);
    indexIncrement = freq * waveTable.size() / sampleRate;
}

void WavetableVoice::stopNote(float velocity, bool allowTailOff)
{
    playing = false;
    clearCurrentNote();
}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!playing) return;

    auto* left = outputBuffer.getWritePointer(0, startSample);
    auto* right = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

    for (int i = 0; i < numSamples; ++i)
    {
        float sample = getSample();

        left[i] += sample;
        if (right) right[i] += sample;

        index += indexIncrement;
        if (index >= waveTable.size())
            index -= waveTable.size();
    }
}

float WavetableVoice::getSample()
{
    auto truncatedIndex = static_cast<int>(index);
    auto nextIndex = (truncatedIndex + 1) % waveTable.size();
    auto frac = index - truncatedIndex;
    float sample = waveTable[truncatedIndex] * (1.0f - frac) + waveTable[nextIndex] * frac;
    float scaledSample = sample * 0.5;
    return scaledSample;
}
