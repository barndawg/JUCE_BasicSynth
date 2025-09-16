#pragma once

#include <JuceHeader.h>

class WavetableSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int) { return true; }
    bool appliesToChannel(int) { return true; }
};
