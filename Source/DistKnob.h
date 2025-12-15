/*
  ==============================================================================

    DistKnob.h
    Created: 11 Dec 2025 3:23:39pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"

class DistKnob : public juce::LookAndFeel_V4
{
public:
    DistKnob();
    ~DistKnob() override = default;

    std::unique_ptr<juce::Drawable> sliderDrawable;

    juce::Rectangle<float> sliderArea{ 0, 0, DISTORTION_KNOB_WIDTH, DISTORTION_KNOB_HEIGHT };

    juce::MemoryInputStream distKnobSVGFile = juce::MemoryInputStream(BinaryData::DistKnob_svg, BinaryData::DistKnob_svgSize, false);
    std::unique_ptr<juce::Drawable> distKnobDrawable = juce::Drawable::createFromImageDataStream(distKnobSVGFile);

private:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};

