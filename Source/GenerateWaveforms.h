#pragma once

#include <JuceHeader.h>
#include <cmath>

std::vector<float> generateSineWavetable();
std::vector<float> generateSawWavetable();
std::vector<float> generateTriWavetable();

std::vector<float> normaliseWavetable(std::vector<float> tableToNormalise);

constexpr auto WT_SIZE = 512;
const auto PI = std::atan(1.f) * 4;
