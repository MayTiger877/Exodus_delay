/*
  ==============================================================================

    ByTempoToggle.h
    Created: 29 Dec 2025 1:23:22pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Configs.h"

class ByTempoToggle : public juce::LookAndFeel_V4
{
public:
    ByTempoToggle();
    ~ByTempoToggle() override = default;

    std::unique_ptr<juce::Drawable> byTempoToggleUpDrawable;
	std::unique_ptr<juce::Drawable> byTempoToggleDownDrawable;

private:
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};