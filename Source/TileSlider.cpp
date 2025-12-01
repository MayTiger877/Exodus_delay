/*
  ==============================================================================

	TileSlider.cpp
	Created: 23 Oct 2025 3:40:17pm
	Author:  MayTigerStation

  ==============================================================================
*/

#include "TileSlider.h"

void GainLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
									float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	// bg
	backgroundDrawable->drawWithin(g, this->bgArea, juce::RectanglePlacement::stretchToFit, 1.0f);

	// slider
	auto sliderSVGFile = juce::MemoryInputStream(BinaryData::Red_Slider_svg, BinaryData::Red_Slider_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(sliderSVGFile);
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(6, 56 - (56 * sliderPos), TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}

void PanLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
									float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	// bg
	backgroundDrawable->drawWithin(g, this->bgArea, juce::RectanglePlacement::stretchToFit, 1.0f);

	// slider
	auto sliderSVGFile = juce::MemoryInputStream(BinaryData::purple_Slider_svg, BinaryData::purple_Slider_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(sliderSVGFile);
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(6, 56 - (56 * sliderPos), TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}

void DistortionLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
										float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	// bg
	backgroundDrawable->drawWithin(g, this->bgArea, juce::RectanglePlacement::stretchToFit, 1.0f);

	// slider
	auto sliderSVGFile = juce::MemoryInputStream(BinaryData::yellow_Slider_svg, BinaryData::yellow_Slider_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(sliderSVGFile);
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(6, 56 - (56 * sliderPos), TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);

}


void ReverbLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
									float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	// bg
	backgroundDrawable->drawWithin(g, this->bgArea, juce::RectanglePlacement::stretchToFit, 1.0f);

	// slider
	auto sliderSVGFile = juce::MemoryInputStream(BinaryData::green_Slider_svg, BinaryData::green_Slider_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(sliderSVGFile);
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(6, 56 - (56 * sliderPos), TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}


void PhaserLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
									float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	// bg
	backgroundDrawable->drawWithin(g, this->bgArea, juce::RectanglePlacement::stretchToFit, 1.0f);

	// slider
	auto sliderSVGFile = juce::MemoryInputStream(BinaryData::orange_Slider_svg, BinaryData::orange_Slider_svgSize, false);
	sliderDrawable = juce::Drawable::createFromImageDataStream(sliderSVGFile);
	sliderDrawable->drawWithin(g, juce::Rectangle<float>(6, 56 - (56 * sliderPos), TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}