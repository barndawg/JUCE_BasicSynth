#include "GenerateWaveforms.h"

std::vector<float> generateSineWavetable()
{
    std::vector<float> sineWt = std::vector<float>(WT_SIZE);
    for (auto i = 0; i < WT_SIZE; ++i)
        sineWt[i] = std::sinf(2 * PI * static_cast<float>(i) / WT_SIZE);
    
    return sineWt;
}

std::vector<float> generateSawWavetable()
{
    constexpr auto NUM_HARMONICS = WT_SIZE / 3;
    
    std::vector<float> sawWt = std::vector<float>(WT_SIZE, 0.f);
    
    for (size_t n = 1; n < NUM_HARMONICS; ++n)
    {
        float harmonicAmplitude = 1.f / n;
        
        for (size_t i = 0; i < WT_SIZE; ++i)
        {
            float phase = 2.f * PI * static_cast<float>(n) * static_cast<float>(i) / static_cast<float>(WT_SIZE);
            sawWt[i] += harmonicAmplitude * std::sinf(phase);
        }
    }
    
    return normaliseWavetable(sawWt);
}

std::vector<float> generateTriWavetable()
{
    constexpr auto NUM_HARMONICS = WT_SIZE / 3;
    std::vector<float> triWt(WT_SIZE, 0.f);

    for (size_t k = 0; k < NUM_HARMONICS; ++k)
    {
        size_t n = 2 * k + 1; // Odd harmonics
        float harmonicAmplitude = 1.0f / (n * n);
        // Alternate sign for each harmonic
        if (k % 2 == 1)
            harmonicAmplitude = -harmonicAmplitude;

        for (size_t i = 0; i < WT_SIZE; ++i)
        {
            float phase = 2.f * PI * static_cast<float>(n) * static_cast<float>(i) / static_cast<float>(WT_SIZE);
            triWt[i] += harmonicAmplitude * std::sinf(phase);
        }
    }

    return normaliseWavetable(triWt);
}

std::vector<float> normaliseWavetable(std::vector<float> tableToNormalise)
{
    float maxVal = 0.f;
    for (auto v : tableToNormalise)
        maxVal = std::max(maxVal, std::abs(v));
    
    if (maxVal > 0.f)
        for (auto& v : tableToNormalise) v /= maxVal;
    
    return tableToNormalise;
}
