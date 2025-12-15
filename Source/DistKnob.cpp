/*
  ==============================================================================

    DistKnob.cpp
    Created: 11 Dec 2025 3:23:39pm
    Author:  MayTigerStation

  ==============================================================================
*/

#include "DistKnob.h"

DistKnob::DistKnob()
{
	auto svg = juce::MemoryInputStream(BinaryData::DistKnob_svg, BinaryData::DistKnob_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(svg);
}

void DistKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	float finalPos = (height - DISTORTION_KNOB_HEIGHT) * (1.0f - sliderPos);
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(x, finalPos, DISTORTION_KNOB_WIDTH, DISTORTION_KNOB_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}