/*
  ==============================================================================

    TileSlider.h
    Created: 23 Oct 2025 3:40:17pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"

class TileSlider : public juce::LookAndFeel_V4
{
public:
    TileSlider() = default;
    ~TileSlider() override = default;

protected:
    std::unique_ptr<juce::Drawable> sliderDrawable;

	//juce::Rectangle<float> bgArea{ 0, 0, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT };
	juce::Rectangle<float> sliderArea{ 0, 0, TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT };

    //juce::MemoryInputStream backgroundSVGFile = juce::MemoryInputStream(BinaryData::TileBG_svg, BinaryData::TileBG_svgSize, false);
    //std::unique_ptr<juce::Drawable> backgroundDrawable = juce::Drawable::createFromImageDataStream(backgroundSVGFile);

};

class GainLAF : public TileSlider
{
    public:
    GainLAF();
    ~GainLAF() override = default;
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};

class PanLAF : public TileSlider
{
    public:
    PanLAF();
    ~PanLAF() override = default;
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};

class DistortionLAF : public TileSlider
{
public:
	DistortionLAF();
    ~DistortionLAF() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};

class ReverbLAF : public TileSlider
{
public:
    ReverbLAF();
    ~ReverbLAF() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};


class PhaserLAF : public TileSlider
{
public:
    PhaserLAF();
    ~PhaserLAF() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
};

