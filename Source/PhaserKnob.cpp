/*
  ==============================================================================

    PhaserKnob.cpp
    Created: 11 Dec 2025 3:23:51pm
    Author:  MayTigerStation

  ==============================================================================
*/

#include "PhaserKnob.h"

PhaserKnob::PhaserKnob()
{
	auto sineLinesSvg = juce::MemoryInputStream(BinaryData::sine_line_choose_svg, BinaryData::sine_line_choose_svgSize, false);
	sineLines = juce::Drawable::createFromImageDataStream(sineLinesSvg);

	auto squereLinesSvg = juce::MemoryInputStream(BinaryData::square_lines_choose_svg, BinaryData::square_lines_choose_svgSize, false);
	squereLines = juce::Drawable::createFromImageDataStream(squereLinesSvg);

	auto sawUpLinesSvg = juce::MemoryInputStream(BinaryData::saw_up_lines_choose_svg, BinaryData::saw_up_lines_choose_svgSize, false);
	sawUpLines = juce::Drawable::createFromImageDataStream(sawUpLinesSvg);

	auto sawDownLinesSvg = juce::MemoryInputStream(BinaryData::saw_down_lines_choose_svg, BinaryData::saw_down_lines_choose_svgSize, false);
	sawDownLines = juce::Drawable::createFromImageDataStream(sawDownLinesSvg);

	auto triangleLinesSvg = juce::MemoryInputStream(BinaryData::triangle_lines_choose_svg, BinaryData::triangle_lines_choose_svgSize, false);
	triangleLines = juce::Drawable::createFromImageDataStream(triangleLinesSvg);
}

void PhaserKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	if (sliderPos == 0.0f)
	{
		sineLines->drawWithin(g, juce::Rectangle<float>(x, y, 2, 46).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
		return;
	}
	else if (sliderPos == 0.25f)
	{
		triangleLines->drawWithin(g, juce::Rectangle<float>(x+5, y+6, 34, 40).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
		return;
	}
	else if (sliderPos == 0.5f)
	{
		squereLines->drawWithin(g, juce::Rectangle<float>(x+9, y+16, 47, 30).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
		return;
	}
	else if (sliderPos == 0.75f)
	{
		sawUpLines->drawWithin(g, juce::Rectangle<float>(x+13, y+28, 43, 18).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
		return;
	}
	else if (sliderPos == 1.0f)
	{
		sawDownLines->drawWithin(g, juce::Rectangle<float>(x+17, y+37, 52, 13).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
		return;
	}
}