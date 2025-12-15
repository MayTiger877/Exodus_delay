/*
  ==============================================================================

    PhaserKnob.h
    Created: 11 Dec 2025 3:23:51pm
    Author:  MayTigerStation

  ==============================================================================
*/
#include <JuceHeader.h>
#include "Configs.h"

#pragma once

class PhaserKnob : public juce::LookAndFeel_V4
{
public:
    PhaserKnob();
    ~PhaserKnob() override = default;

    std::unique_ptr<juce::Drawable> sineLines, squereLines, sawUpLines, sawDownLines, triangleLines;

private:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};
