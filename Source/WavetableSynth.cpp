#include "WavetableSynth.h"

// ====================   Init   =========================

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
    
    initializeOscillators();
}

void WavetableSynth::initializeOscillators()
{
    constexpr auto osc_count = 128;
    
    const auto waveTable = generateSineWaveTable();
    
    oscillators.clear();
    for (auto i = 0; i < osc_count; ++i)
    {
        oscillators.emplace_back(waveTable, sampleRate);
    }
}

std::vector<float> WavetableSynth::generateSineWaveTable()
{
    constexpr auto wt_length = 512;
    
    std::vector<float> sineWaveTable(wt_length);
    
//    const auto PI = std::atanf(1.f) * 4;
//    
//    for (auto i = 0; i < wt_length; ++i)
//    {
//        sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / static_cast<float>(wt_length));
//    }
    
    for (auto i = 0; i < wt_length; ++i){
        sineWaveTable[i] = (static_cast<float>(i) / static_cast<float>(wt_length)) * 2 - 1;
    }
    
    return sineWaveTable;
}

// ====================   Playback   =========================

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    if (midiEvent.isNoteOn())
    {
        const auto oscillatorId = midiEvent.getNoteNumber();
        const auto frequency = midiNoteNumberToFrequency(oscillatorId);
        oscillators[oscillatorId].setFrequency(frequency);
    }
    else if (midiEvent.isNoteOff())
    {
        const auto oscillatorId = midiEvent.getNoteNumber();
        oscillators[oscillatorId].stop();
    }
    else if (midiEvent.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }
    }
}

// ====================   Render   =========================

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto currentSample = 0;
    
    for (const auto midiMetadata : midiMessages)
    {
        const auto message = midiMetadata.getMessage();
        const auto messagePosition = static_cast<int>(message.getTimeStamp());
        
        render(buffer, currentSample, messagePosition); // render up until start of first MIDI event
        handleMidiEvent(message); // turn on/off notes
        
        currentSample = messagePosition; // begin next render from previous message timestamp
    }
    
    render(buffer, currentSample, buffer.getNumSamples()); // render until end of block
}

// Renders to the audio buffer (output) - called by processBlock per MIDI message
void WavetableSynth::render(juce::AudioBuffer<float> &buffer, int beginSample, int endSample)
{
    auto firstChannel = buffer.getWritePointer(0);
    
    for (auto& oscillator : oscillators) // iterate through all oscillators
    {
        if (oscillator.isPlaying())
        {
            for (auto sample = beginSample; sample < endSample; ++sample)
            {
                firstChannel[sample] += oscillator.getSample(); // add signal to buffer so we don't overwrite other voices
            }
        }
    }
    
    for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        // duplicate signal to all other channels
        std::copy(firstChannel + beginSample, firstChannel + endSample, buffer.getWritePointer(channel) + beginSample);
    }
}

// ====================   Helper   =========================

float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
    constexpr auto A4_freq = 440.f;
    constexpr auto A4_note = 69.f;
    constexpr auto oct_steps = 12.f;
    return A4_freq * std::powf(2.f, (midiNoteNumber - A4_note) / oct_steps);
}
