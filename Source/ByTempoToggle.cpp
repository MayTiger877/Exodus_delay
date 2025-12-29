/*
  ==============================================================================

    ByTempoToggle.cpp
    Created: 29 Dec 2025 1:23:22pm
    Author:  MayTigerStation

  ==============================================================================
*/

#include "ByTempoToggle.h"

ByTempoToggle::ByTempoToggle()
{
    auto toggleUpSvg = juce::MemoryInputStream(BinaryData::toggleUp_svg, BinaryData::toggleUp_svgSize, false);
    byTempoToggleUpDrawable = juce::Drawable::createFromImageDataStream(toggleUpSvg);
    
    auto toggleDownSvg = juce::MemoryInputStream(BinaryData::toggleDown_svg, BinaryData::toggleDown_svgSize, false);
    byTempoToggleDownDrawable = juce::Drawable::createFromImageDataStream(toggleDownSvg);
}

void ByTempoToggle::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                      bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    bool isButtonUP = button.getToggleState();
	if (isButtonUP)
        byTempoToggleUpDrawable->drawWithin(g, juce::Rectangle<float>(0, 0, GNRL_BY_TEMPO_TOGGLE_WIDTH, GNRL_BY_TEMPO_TOGGLE_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
    else
        byTempoToggleDownDrawable->drawWithin(g, juce::Rectangle<float>(0, 0, GNRL_BY_TEMPO_TOGGLE_WIDTH, GNRL_BY_TEMPO_TOGGLE_HEIGHT).toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
}

