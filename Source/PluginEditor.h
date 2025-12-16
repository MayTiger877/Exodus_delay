/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"
#include "PluginProcessor.h"
#include "TileSlider.h"
#include "CostumeKnob.h"
#include "DistKnob.h"
#include "PhaserKnob.h"
#include <chrono>

//==============================================================================
/**                       
*/

struct channelStrip
{
    juce::Slider gainSlider, panSlider, distortionMixSlider, reverbMixSlider, phaserMixSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment, panAttachment, distortionMixAttachment, reverbMixAttachment, phaserMixAttachment;
};

struct distortionSettings
{
	juce::Slider distortionDriveSlider, distortionThresholdSlider, distortionTypeSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distortionDriveAttachment, distortionThresholdAttachment, distortionTypeAttachment;
};

struct phaserSettings
{
	juce::Slider phaserRateSlider, phaserDepthSlider, phaserFeedbackSlider, phaserTypeSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserRateAttachment, phaserDepthAttachment, phaserFeedbackAttachment, phaserTypeAttachment;
};

struct reverbSettings
{
	juce::Slider reverbRoomSizeSlider, reverbDampingSlider, reverbWidthSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbRoomSizeAttachment, reverbDampingAttachment, reverbWidthAttachment;
};

class Exodus_2AudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    Exodus_2AudioProcessorEditor (Exodus_2AudioProcessor&);
    ~Exodus_2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

	void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Exodus_2AudioProcessor& audioProcessor;

    uint32_t m_timer = 0;
	int m_index = 0;

    std::unique_ptr<juce::Drawable> backgroundDrawable, backgroundTextureDrawable, leftPanelWoodDrawable, rightPanelWoodDrawable, ventDrawable;

	channelStrip m_channelStrips[16];
	void initiateChannelStrips();

	//general settings
	juce::Slider m_delayTimeSlider, m_feedbackSlider, m_dryLevelSlider, m_wetLevelSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delayTimeAttachment, m_feedbackAttachment, m_dryLevelAttachment, m_wetLevelAttachment;
	void initiateGeneralSettings();

	// LookAndFeel objects
	GainLAF gainLAF;
	PanLAF panLAF;
	DistortionLAF distortionLAF;
	ReverbLAF reverbLAF;
	PhaserLAF phaserLAF;
	CostumeKnob costumeKnobLAF;
	DistKnob distKnobLAF;
	PhaserKnob phaserKnobLAF;

	distortionSettings m_distortionSettings;
	phaserSettings m_phaserSettings;
	reverbSettings m_reverbSettings;
	void initiateEffectSettings();
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Exodus_2AudioProcessorEditor)
};
