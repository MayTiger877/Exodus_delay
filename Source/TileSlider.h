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

	juce::Rectangle<float> sliderArea{ 0, 0, TILE_SLIDER_WIDTH, TILE_SLIDER_HEIGHT };
};

class GainLAF : public TileSlider
{
    public:
    GainLAF();
    ~GainLAF() override = default;
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
		const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};

class PanLAF : public TileSlider
{
    public:
    PanLAF();
    ~PanLAF() override = default;
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};

class DistortionLAF : public TileSlider
{
public:
	DistortionLAF();
    ~DistortionLAF() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};

class ReverbLAF : public TileSlider
{
public:
    ReverbLAF();
    ~ReverbLAF() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};


class PhaserLAF : public TileSlider
{
public:
    PhaserLAF();
    ~PhaserLAF() override = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};

