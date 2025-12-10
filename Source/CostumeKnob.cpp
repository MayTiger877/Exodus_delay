/*
  ==============================================================================

    CostumeKnob.cpp
    Created: 10 Dec 2025 3:14:41pm
    Author:  MayTigerStation

  ==============================================================================
*/

#include "CostumeKnob.h"


CostumeKnob::CostumeKnob()
{
	auto svg = juce::MemoryInputStream(BinaryData::Knob_svg, BinaryData::Knob_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(svg);
}

void CostumeKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	const float angle = 0.0f + sliderPos * 4.8f;
	const float centreX = x + COSTUME_KNOB_WIDTH * 0.5f;
	const float centreY = y + COSTUME_KNOB_HEIGHT * 0.5f;
	sliderDrawable->setTransform(juce::AffineTransform::rotation(angle, centreX, centreY));
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(0, 0, COSTUME_KNOB_WIDTH, COSTUME_KNOB_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}