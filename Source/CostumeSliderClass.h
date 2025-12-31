/*
  ==============================================================================

    CostumeSliderClass.h
    Created: 31 Dec 2025 1:34:20pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class ValuePopupSlider : public juce::Slider
{
public:
    ValuePopupSlider()
    {
        onDragStart = [this]()
            {
                if (onPopupStart)
                    onPopupStart(*this);
            };

        onDragEnd = [this]()
            {
                if (onPopupEnd)
                    onPopupEnd(*this);
            };

        onValueChange = [this]()
            {
                if (onPopupUpdate)
                    onPopupUpdate(*this);
            };
    }

    // Editor hooks (set once)
    std::function<void(ValuePopupSlider&)> onPopupStart;
    std::function<void(ValuePopupSlider&)> onPopupUpdate;
    std::function<void(ValuePopupSlider&)> onPopupEnd;
};
