/*
  ==============================================================================

    CostumeKnob.h
    Created: 10 Dec 2025 3:14:41pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"

class CostumeKnob : public juce::LookAndFeel_V4
{
public:
    CostumeKnob();
    ~CostumeKnob() override = default;

    std::unique_ptr<juce::Drawable> sliderDrawable;

    juce::Rectangle<float> sliderArea{ 0, 0, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT };

    juce::MemoryInputStream backgroundSVGFile = juce::MemoryInputStream(BinaryData::TileBG_svg, BinaryData::TileBG_svgSize, false);
    std::unique_ptr<juce::Drawable> backgroundDrawable = juce::Drawable::createFromImageDataStream(backgroundSVGFile);

private:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};
