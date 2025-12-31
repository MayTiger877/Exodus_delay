/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
Exodus_2AudioProcessorEditor::Exodus_2AudioProcessorEditor (Exodus_2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (SCREEN_WIDTH, SCREEN_HEIGHT);

	startTimerHz(FPS);
	m_timer = 0;

    auto backgroundSVGFile = juce::MemoryInputStream(BinaryData::bg_svg, BinaryData::bg_svgSize, false);
    backgroundDrawable = juce::Drawable::createFromImageDataStream(backgroundSVGFile);

	backgroundTextureDrawable = juce::Drawable::createFromImageData(BinaryData::BrushedMetal_Texture_png, BinaryData::BrushedMetal_Texture_pngSize);

	leftPanelWoodDrawable = juce::Drawable::createFromImageData(BinaryData::black_wood_texture_png, BinaryData::black_wood_texture_pngSize);
	rightPanelWoodDrawable = juce::Drawable::createFromImageData(BinaryData::black_wood_texture_png, BinaryData::black_wood_texture_pngSize);

	auto ventSVGFile = juce::MemoryInputStream(BinaryData::Vent_svg, BinaryData::Vent_svgSize, false);
	ventDrawable = juce::Drawable::createFromImageDataStream(ventSVGFile);

	valuePopup.setVisible(false);
	valuePopup.setJustificationType(juce::Justification::centred);
	valuePopup.setInterceptsMouseClicks(false, false);
	valuePopup.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.85f));
	valuePopup.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(valuePopup);

    initiateChannelStrips();
	initiateGeneralSettings();
	initiateEffectSettings();

}

void Exodus_2AudioProcessorEditor::attachPopup(ValuePopupSlider& s)
{
	s.onPopupStart = [this](ValuePopupSlider& slider)
	{
		activeSlider = &slider;
		valuePopup.setVisible(true);
	};

	s.onPopupUpdate = [this](ValuePopupSlider& slider)
	{
		if (activeSlider != &slider)
			return;
			
		juce::String toDisplay(slider.getValue(), 2);

		if (slider.getName().containsIgnoreCase("Gain"))
			toDisplay.append(" db", 3);

		if (slider.getName().containsIgnoreCase("Rate"))
			toDisplay.append(" Hz", 3);

		valuePopup.setText(juce::String(slider.getValue(), 2), juce::dontSendNotification);
		auto b = slider.getBounds();
		valuePopup.setBounds(b.getCentreX() - 30, b.getY() - 24, 60, 20);
	};

	s.onPopupEnd = [this](ValuePopupSlider&)
	{
		valuePopup.setVisible(false);
		activeSlider = nullptr;
	};
}

Exodus_2AudioProcessorEditor::~Exodus_2AudioProcessorEditor()
{
	for (int i = 0; i < 16; ++i)
	{
		m_channelStrips[i].gainSlider.setLookAndFeel(nullptr);
		m_channelStrips[i].panSlider.setLookAndFeel(nullptr);
		m_channelStrips[i].distortionMixSlider.setLookAndFeel(nullptr);
		m_channelStrips[i].reverbMixSlider.setLookAndFeel(nullptr);
		m_channelStrips[i].phaserMixSlider.setLookAndFeel(nullptr);
	}

	m_delayTimeSlider.setLookAndFeel(nullptr);
	m_feedbackSlider.setLookAndFeel(nullptr);
	m_dryLevelSlider.setLookAndFeel(nullptr);
	m_wetLevelSlider.setLookAndFeel(nullptr);

	m_distortionSettings.distortionTypeSlider.setLookAndFeel(nullptr);
	m_distortionSettings.distortionDriveSlider.setLookAndFeel(nullptr);
	m_distortionSettings.distortionThresholdSlider.setLookAndFeel(nullptr);

	m_phaserSettings.phaserTypeSlider.setLookAndFeel(nullptr);
	m_phaserSettings.phaserRateSlider.setLookAndFeel(nullptr);
	m_phaserSettings.phaserDepthSlider.setLookAndFeel(nullptr);
	m_phaserSettings.phaserFreqSlider.setLookAndFeel(nullptr);

	m_reverbSettings.reverbRoomSizeSlider.setLookAndFeel(nullptr);
	m_reverbSettings.reverbDampingSlider.setLookAndFeel(nullptr);
	m_reverbSettings.reverbWidthSlider.setLookAndFeel(nullptr);

	m_generalDelayTempoToggleButton.setLookAndFeel(nullptr);
	m_phaserDelayTempoToggleButton.setLookAndFeel(nullptr);
}

//==============================================================================

void Exodus_2AudioProcessorEditor::timerCallback()
{
	m_timer += this->getTimerInterval();
	float delayTimeInMs = this->audioProcessor.getDelayTimeInMs();
	if (m_timer > delayTimeInMs)
    {
        m_index = (m_index + 1) % 16;
		this->audioProcessor.setIndex(m_index);
		m_timer = m_timer - delayTimeInMs;
    }

    repaint();
}

void Exodus_2AudioProcessorEditor::initiateChannelStrips()
{
    for (int i = 0; i < 16; ++i)
    {
        auto& strip = m_channelStrips[i];
        
		strip.gainSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, GAIN_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.gainSlider.setValue(GAIN_SLIDER_DEFAULT_VALUE);
		strip.gainSlider.setRange(GAIN_SLIDER_MIN_VALUE, GAIN_SLIDER_MAX_VALUE, GAIN_SLIDER_INTERVAL);
		strip.gainSlider.setName(gainMixParamID[i]);
        strip.gainSlider.setSliderStyle(juce::Slider::LinearVertical);
		strip.gainSlider.setInterceptsMouseClicks(false, false);
		strip.gainSlider.setSliderSnapsToMousePosition(false);
		strip.gainSlider.setVelocityBasedMode(false);
		strip.gainSlider.setScrollWheelEnabled(false);
		strip.gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.gainSlider.setLookAndFeel(&gainLAF);
        addAndMakeVisible(strip.gainSlider);
        strip.gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, gainMixParamID[i], strip.gainSlider);
        
		strip.panSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, PAN_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
        strip.panSlider.setValue(PAN_SLIDER_DEFAULT_VALUE);
		strip.panSlider.setRange(PAN_SLIDER_MIN_VALUE, PAN_SLIDER_MAX_VALUE, PAN_SLIDER_INTERVAL);
		strip.panSlider.setSliderStyle(juce::Slider::LinearVertical);
		strip.panSlider.setInterceptsMouseClicks(false, false);
		strip.panSlider.setSliderSnapsToMousePosition(false);
		strip.panSlider.setVelocityBasedMode(false);
		strip.panSlider.setScrollWheelEnabled(false);
        strip.panSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.panSlider.setLookAndFeel(&panLAF);
        addAndMakeVisible(strip.panSlider);
		strip.panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, panMixParamID[i], strip.panSlider);

		strip.distortionMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, DISTORTION_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.distortionMixSlider.setValue(DISTORTION_MIX_SLIDER_DEFAULT_VALUE);
		strip.distortionMixSlider.setRange(DISTORTION_MIX_SLIDER_MIN_VALUE, DISTORTION_MIX_SLIDER_MAX_VALUE, DISTORTION_MIX_SLIDER_INTERVAL);
		strip.distortionMixSlider.setSliderStyle(juce::Slider::LinearVertical);
		strip.distortionMixSlider.setInterceptsMouseClicks(false, false);
		strip.distortionMixSlider.setSliderSnapsToMousePosition(false);
		strip.distortionMixSlider.setVelocityBasedMode(false);
		strip.distortionMixSlider.setScrollWheelEnabled(false);
		strip.distortionMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.distortionMixSlider.setLookAndFeel(&distortionLAF);
		addAndMakeVisible(strip.distortionMixSlider);
		strip.distortionMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, distortionMixParamID[i], strip.distortionMixSlider);

		strip.reverbMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, REVERB_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.reverbMixSlider.setValue(REVERB_MIX_SLIDER_DEFAULT_VALUE);
		strip.reverbMixSlider.setRange(REVERB_MIX_SLIDER_MIN_VALUE, REVERB_MIX_SLIDER_MAX_VALUE, REVERB_MIX_SLIDER_INTERVAL);
		strip.reverbMixSlider.setSliderStyle(juce::Slider::LinearVertical);
		strip.reverbMixSlider.setInterceptsMouseClicks(false, false);
		strip.reverbMixSlider.setSliderSnapsToMousePosition(false);
		strip.reverbMixSlider.setVelocityBasedMode(false);
		strip.reverbMixSlider.setScrollWheelEnabled(false);
		strip.reverbMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.reverbMixSlider.setLookAndFeel(&reverbLAF);
		addAndMakeVisible(strip.reverbMixSlider);
		strip.reverbMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, reverbMixParamID[i], strip.reverbMixSlider);

		strip.phaserMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, PHASER_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.phaserMixSlider.setValue(PHASER_MIX_SLIDER_DEFAULT_VALUE);
		strip.phaserMixSlider.setRange(PHASER_MIX_SLIDER_MIN_VALUE, PHASER_MIX_SLIDER_MAX_VALUE, PHASER_MIX_SLIDER_INTERVAL);
		strip.phaserMixSlider.setSliderStyle(juce::Slider::LinearVertical);
		strip.phaserMixSlider.setInterceptsMouseClicks(false, false);
		strip.phaserMixSlider.setSliderSnapsToMousePosition(false);
		strip.phaserMixSlider.setVelocityBasedMode(false);
		strip.phaserMixSlider.setScrollWheelEnabled(false);
		strip.phaserMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.phaserMixSlider.setLookAndFeel(&phaserLAF);
		addAndMakeVisible(strip.phaserMixSlider);
		strip.phaserMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserMixParamID[i], strip.phaserMixSlider);
    }
}

void Exodus_2AudioProcessorEditor::initiateGeneralSettings()
{
	m_delayTimeSlider.setValue(GNRL_DELAY_TIME_SLIDER_DEFAULT_VALUE);
	m_delayTimeSlider.setRange(GNRL_DELAY_TIME_SLIDER_MIN_VALUE, GNRL_DELAY_TIME_SLIDER_MAX_VALUE, GNRL_DELAY_TIME_SLIDER_INTERVAL);
 	m_delayTimeSlider.setBounds(GNRL_DELAY_TIME_SLIDER_BOUNDS);
	m_delayTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_delayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_delayTimeSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_delayTimeSlider);
	addAndMakeVisible(m_delayTimeSlider);
	m_delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, delayTimeParamID, m_delayTimeSlider);

	m_generalDelayTempoToggleButton.setBounds(GNRL_BY_TEMPO_TOGGLE_BOUNDS);
	m_generalDelayTempoToggleButton.setLookAndFeel(&TempoToggleButtonLAF);
	m_generalDelayTempoToggleButton.onClick = [this]() { updateGeneralDelayTimeAttachment(); };
	addAndMakeVisible(m_generalDelayTempoToggleButton);
	m_generalDelayTempoToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, generalTempoSyncToggleParamID, m_generalDelayTempoToggleButton);

	m_feedbackSlider.setValue(GNRL_FEEDBACK_SLIDER_DEFAULT_VALUE);
	m_feedbackSlider.setRange(GNRL_FEEDBACK_SLIDER_MIN_VALUE, GNRL_FEEDBACK_SLIDER_MAX_VALUE, GNRL_FEEDBACK_SLIDER_INTERVAL);
	m_feedbackSlider.setBounds(GNRL_FEEDBACK_SLIDER_BOUNDS);
	m_feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_feedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_feedbackSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_feedbackSlider);
	addAndMakeVisible(m_feedbackSlider);
	m_feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, delayFeedbackParamID, m_feedbackSlider);

	m_dryLevelSlider.setValue(GNRL_DRY_LEVEL_SLIDER_DEFAULT_VALUE);
	m_dryLevelSlider.setRange(GNRL_DRY_LEVEL_SLIDER_MIN_VALUE, GNRL_DRY_LEVEL_SLIDER_MAX_VALUE, GNRL_DRY_LEVEL_SLIDER_INTERVAL);
	m_dryLevelSlider.setBounds(GNRL_DRY_LEVEL_SLIDER_BOUNDS);
	m_dryLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_dryLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_dryLevelSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_dryLevelSlider);
	addAndMakeVisible(m_dryLevelSlider);
	m_dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, delayDryLevelParamID, m_dryLevelSlider);

	m_wetLevelSlider.setValue(GNRL_WET_LEVEL_SLIDER_DEFAULT_VALUE);
	m_wetLevelSlider.setRange(GNRL_WET_LEVEL_SLIDER_MIN_VALUE, GNRL_WET_LEVEL_SLIDER_MAX_VALUE, GNRL_WET_LEVEL_SLIDER_INTERVAL);
	m_wetLevelSlider.setBounds(GNRL_WET_LEVEL_SLIDER_BOUNDS);
	m_wetLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_wetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_wetLevelSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_wetLevelSlider);
	addAndMakeVisible(m_wetLevelSlider);
	m_wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, delayWetLevelParamID, m_wetLevelSlider);
}

void Exodus_2AudioProcessorEditor::initiateEffectSettings()
{
	// Distortion Settings
	m_distortionSettings.distortionTypeSlider.setValue(DISTORTION_TYPE_SLIDER_DEFAULT_VALUE);
	m_distortionSettings.distortionTypeSlider.setRange(DISTORTION_TYPE_SLIDER_MIN_VALUE, DISTORTION_TYPE_SLIDER_MAX_VALUE, DISTORTION_TYPE_SLIDER_INTERVAL);
	m_distortionSettings.distortionTypeSlider.setBounds(DISTORTION_TYPE_SLIDER_BOUNDS);
	m_distortionSettings.distortionTypeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_distortionSettings.distortionTypeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_distortionSettings.distortionTypeSlider.setLookAndFeel(&distKnobLAF);
	attachPopup(m_distortionSettings.distortionTypeSlider);
	addAndMakeVisible(m_distortionSettings.distortionTypeSlider);
	m_distortionSettings.distortionTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, distortionTypeParamID, m_distortionSettings.distortionTypeSlider);

	m_distortionSettings.distortionDriveSlider.setValue(DISTORTION_DRIVE_SLIDER_DEFAULT_VALUE);
	m_distortionSettings.distortionDriveSlider.setRange(DISTORTION_DRIVE_SLIDER_MIN_VALUE, DISTORTION_DRIVE_SLIDER_MAX_VALUE, DISTORTION_DRIVE_SLIDER_INTERVAL);
	m_distortionSettings.distortionDriveSlider.setBounds(DISTORTION_DRIVE_SLIDER_BOUNDS);
	m_distortionSettings.distortionDriveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_distortionSettings.distortionDriveSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_distortionSettings.distortionDriveSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_distortionSettings.distortionDriveSlider);
	addAndMakeVisible(m_distortionSettings.distortionDriveSlider);
	m_distortionSettings.distortionDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, distortionDriveParamID, m_distortionSettings.distortionDriveSlider);

	m_distortionSettings.distortionThresholdSlider.setValue(DISTORTION_THRESHOLD_SLIDER_DEFAULT_VALUE);
	m_distortionSettings.distortionThresholdSlider.setRange(DISTORTION_THRESHOLD_SLIDER_MIN_VALUE, DISTORTION_THRESHOLD_SLIDER_MAX_VALUE, DISTORTION_THRESHOLD_SLIDER_INTERVAL);
	m_distortionSettings.distortionThresholdSlider.setBounds(DISTORTION_THRESHOLD_SLIDER_BOUNDS);
	m_distortionSettings.distortionThresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_distortionSettings.distortionThresholdSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_distortionSettings.distortionThresholdSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_distortionSettings.distortionThresholdSlider);
	addAndMakeVisible(m_distortionSettings.distortionThresholdSlider);
	m_distortionSettings.distortionThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, distortionThresholdParamID, m_distortionSettings.distortionThresholdSlider);

	// Phaser Settings
	m_phaserSettings.phaserTypeSlider.setValue(PHASER_TYPE_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserTypeSlider.setRange(PHASER_TYPE_SLIDER_MIN_VALUE, PHASER_TYPE_SLIDER_MAX_VALUE, PHASER_TYPE_SLIDER_INTERVAL);
	m_phaserSettings.phaserTypeSlider.setBounds(PHASER_TYPE_SLIDER_BOUNDS);
	m_phaserSettings.phaserTypeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserTypeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserTypeSlider.setLookAndFeel(&phaserKnobLAF);
	attachPopup(m_phaserSettings.phaserTypeSlider);
	addAndMakeVisible(m_phaserSettings.phaserTypeSlider);
	m_phaserSettings.phaserTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserTypeParamID, m_phaserSettings.phaserTypeSlider);

	m_phaserSettings.phaserRateSlider.setValue(PHASER_RATE_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserRateSlider.setRange(PHASER_RATE_SLIDER_MIN_VALUE, PHASER_RATE_SLIDER_MAX_VALUE);
	m_phaserSettings.phaserRateSlider.setBounds(PHASER_RATE_SLIDER_BOUNDS);
	m_phaserSettings.phaserRateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserRateSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserRateSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_phaserSettings.phaserRateSlider);
	addAndMakeVisible(m_phaserSettings.phaserRateSlider);
	m_phaserSettings.phaserRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserRateParamID, m_phaserSettings.phaserRateSlider);

	m_phaserDelayTempoToggleButton.setBounds(PHASER_BY_TEMPO_TOGGLE_BOUNDS);
	m_phaserDelayTempoToggleButton.setLookAndFeel(&TempoToggleButtonLAF);
	m_phaserDelayTempoToggleButton.onClick = [this]() { updatePhaserDelayTimeAttachment(); };
	addAndMakeVisible(m_phaserDelayTempoToggleButton);
	m_phaserDelayTempoToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, phaserTempoSyncToggleParamID, m_phaserDelayTempoToggleButton);

	m_phaserSettings.phaserDepthSlider.setValue(PHASER_DEPTH_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserDepthSlider.setRange(PHASER_DEPTH_SLIDER_MIN_VALUE, PHASER_DEPTH_SLIDER_MAX_VALUE, PHASER_DEPTH_SLIDER_INTERVAL);
	m_phaserSettings.phaserDepthSlider.setBounds(PHASER_DEPTH_SLIDER_BOUNDS);
	m_phaserSettings.phaserDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserDepthSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_phaserSettings.phaserDepthSlider);
	addAndMakeVisible(m_phaserSettings.phaserDepthSlider);
	m_phaserSettings.phaserDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserDepthParamID, m_phaserSettings.phaserDepthSlider);

	m_phaserSettings.phaserFreqSlider.setValue(PHASER_FREQ_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserFreqSlider.setRange(PHASER_FREQ_SLIDER_MIN_VALUE, PHASER_FREQ_SLIDER_MAX_VALUE);
	m_phaserSettings.phaserFreqSlider.setBounds(PHASER_FREQ_SLIDER_BOUNDS);
	m_phaserSettings.phaserFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserFreqSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_phaserSettings.phaserFreqSlider);
	addAndMakeVisible(m_phaserSettings.phaserFreqSlider);
	m_phaserSettings.phaserFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserFreqParamID, m_phaserSettings.phaserFreqSlider);

	// Reverb Settings
	m_reverbSettings.reverbRoomSizeSlider.setValue(REVERB_ROOM_SIZE_SLIDER_DEFAULT_VALUE);
	m_reverbSettings.reverbRoomSizeSlider.setRange(REVERB_ROOM_SIZE_SLIDER_MIN_VALUE, REVERB_ROOM_SIZE_SLIDER_MAX_VALUE, REVERB_ROOM_SIZE_SLIDER_INTERVAL);
	m_reverbSettings.reverbRoomSizeSlider.setBounds(REVERB_ROOM_SIZE_SLIDER_BOUNDS);
	m_reverbSettings.reverbRoomSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_reverbSettings.reverbRoomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_reverbSettings.reverbRoomSizeSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_reverbSettings.reverbRoomSizeSlider);
	addAndMakeVisible(m_reverbSettings.reverbRoomSizeSlider);
	m_reverbSettings.reverbRoomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, reverbRoomSizeParamID, m_reverbSettings.reverbRoomSizeSlider);

	m_reverbSettings.reverbDampingSlider.setValue(REVERB_DAMPING_SLIDER_DEFAULT_VALUE);
	m_reverbSettings.reverbDampingSlider.setRange(REVERB_DAMPING_SLIDER_MIN_VALUE, REVERB_DAMPING_SLIDER_MAX_VALUE, REVERB_DAMPING_SLIDER_INTERVAL);
	m_reverbSettings.reverbDampingSlider.setBounds(REVERB_DAMPING_SLIDER_BOUNDS);
	m_reverbSettings.reverbDampingSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_reverbSettings.reverbDampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_reverbSettings.reverbDampingSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_reverbSettings.reverbDampingSlider);
	addAndMakeVisible(m_reverbSettings.reverbDampingSlider);
	m_reverbSettings.reverbDampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, reverbDampingParamID, m_reverbSettings.reverbDampingSlider);

	m_reverbSettings.reverbWidthSlider.setValue(REVERB_WIDTH_SLIDER_DEFAULT_VALUE);
	m_reverbSettings.reverbWidthSlider.setRange(REVERB_WIDTH_SLIDER_MIN_VALUE, REVERB_WIDTH_SLIDER_MAX_VALUE, REVERB_WIDTH_SLIDER_INTERVAL);
	m_reverbSettings.reverbWidthSlider.setBounds(REVERB_WIDTH_SLIDER_BOUNDS);
	m_reverbSettings.reverbWidthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_reverbSettings.reverbWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_reverbSettings.reverbWidthSlider.setLookAndFeel(&costumeKnobLAF);
	attachPopup(m_reverbSettings.reverbWidthSlider);
	addAndMakeVisible(m_reverbSettings.reverbWidthSlider);
	m_reverbSettings.reverbWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, reverbWidthParamID, m_reverbSettings.reverbWidthSlider);
}


void Exodus_2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    if (backgroundDrawable != nullptr)
    {
        backgroundDrawable->setBounds(getLocalBounds());
        backgroundDrawable->draw(g, 1.0f);
    }

	if (backgroundTextureDrawable != nullptr)
	{
		backgroundTextureDrawable->setBounds(0.0f, 0.0f, 945.0f, 648.0f);
		backgroundTextureDrawable->drawWithin(g, juce::Rectangle(15.0f, 8.0f, 945.0f, 648.0f), juce::RectanglePlacement::stretchToFit, 0.2f);
	}

	if (leftPanelWoodDrawable != nullptr)
	{
		leftPanelWoodDrawable->setBounds(0.0f, 0.0f, 14.0f, 650.0f);
		leftPanelWoodDrawable->drawWithin(g, juce::Rectangle<float>(0.0f, 0.0f, (float)14, (float)650), juce::RectanglePlacement::stretchToFit, 1.0f);
	}

	if (rightPanelWoodDrawable != nullptr)
	{
		rightPanelWoodDrawable->setBounds(0.0f, 0.0f, 14.0f, 650.0f);
		rightPanelWoodDrawable->drawWithin(g, juce::Rectangle<float>(961.0f, 0.0f, (float)14, (float)650), juce::RectanglePlacement::stretchToFit, 1.0f);
	}

	if (ventDrawable != nullptr)
	{
		ventDrawable->setBounds(0, 0, 209, 46);
		ventDrawable->drawWithin(g, juce::Rectangle<float>(745.0f, 15.0f, (float)209, (float)46), juce::RectanglePlacement::stretchToFit, 1.0f);
	}

	g.setColour(juce::Colours::forestgreen.darker(0.8f));
	g.fillRect(TILE_SLIDER_STARTING_X - 1 + (m_index * TILE_SLIDER_GAP_X), 418, 40, 9);
}

void Exodus_2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//----------------------------------------------------------------------------------------------------------------

void Exodus_2AudioProcessorEditor::tileMouseDown(const juce::MouseEvent& e)
{
	const int mouseXPos = e.getMouseDownPosition().getX();
	if ((mouseXPos < TILE_SLIDER_STARTING_X) || (mouseXPos > TILE_SLIDER_ENDING_X))
	{
		tile_activeSliderType = -1;
		return;
	}

	const int sliderIndex = (mouseXPos - TILE_SLIDER_STARTING_X) / TILE_SLIDER_GAP_X;
	const int mouseYPos = e.getMouseDownPosition().getY();
	if ((mouseYPos > GAIN_SLIDER_Y) && (mouseYPos < GAIN_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_activeSliderType = 0; // gain
		tile_relativeYPos = GAIN_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].gainSlider;
	}
	else if ((mouseYPos > PAN_SLIDER_Y) && (mouseYPos < PAN_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_activeSliderType = 1; // pan
		tile_relativeYPos = PAN_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].panSlider;
	}
	else if ((mouseYPos > DISTORTION_MIX_SLIDER_Y) && (mouseYPos < DISTORTION_MIX_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_activeSliderType = 2; // distortion mix
		tile_relativeYPos = DISTORTION_MIX_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].distortionMixSlider;
	}
	else if ((mouseYPos > REVERB_MIX_SLIDER_Y) && (mouseYPos < REVERB_MIX_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_activeSliderType = 3; // reverb mix
		tile_relativeYPos = REVERB_MIX_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].reverbMixSlider;
	}
	else if ((mouseYPos > PHASER_MIX_SLIDER_Y) && (mouseYPos < PHASER_MIX_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_activeSliderType = 4; // phaser mix
		tile_relativeYPos = PHASER_MIX_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].phaserMixSlider;
	}
	else
	{
		tile_activeSliderType = -1;
		return;
	}

	const double YToValue = 1.0 - ((static_cast<double>(mouseYPos) - tile_relativeYPos) / TILE_SLIDER_BG_HEIGHT);
	double newValue = juce::jlimit(tile_activeSlider->getMinimum(), tile_activeSlider->getMaximum(), YToValue);
	if (tile_activeSliderType == 1)
	{
		newValue = juce::jmap<double>(newValue, 0.0, 1.0, -1.0, 1.0);
	}
	tile_activeSlider->setValue(newValue, juce::sendNotificationSync);
}


void Exodus_2AudioProcessorEditor::tileMouseDrag(const juce::MouseEvent& e)
{
	if (!tile_activeSlider)
		return;

	const auto editorPos = e.getPosition();
	const auto sliderPos = tile_activeSlider->getBounds();
	int newSliderIndex;
	if ((editorPos.getX() < tile_activeSlider->getX()) && (editorPos.getX() > TILE_SLIDER_STARTING_X))
	{
		newSliderIndex = ((tile_activeSlider->getX() - TILE_SLIDER_STARTING_X) / TILE_SLIDER_GAP_X) - 1;
		switch (tile_activeSliderType)
		{
		case 0:
			tile_activeSlider = &m_channelStrips[newSliderIndex].gainSlider;
			break;
		case 1:
			tile_activeSlider = &m_channelStrips[newSliderIndex].panSlider;
			break;
		case 2:
			tile_activeSlider = &m_channelStrips[newSliderIndex].distortionMixSlider;
			break;
		case 3:
			tile_activeSlider = &m_channelStrips[newSliderIndex].reverbMixSlider;
			break;
		case 4:
			tile_activeSlider = &m_channelStrips[newSliderIndex].phaserMixSlider;
			break;
		default:
			break;
		}
	}
	else if ((editorPos.getX() > (tile_activeSlider->getX() + TILE_SLIDER_GAP_X)) && (editorPos.getX() < TILE_SLIDER_ENDING_X))
	{
		newSliderIndex = ((tile_activeSlider->getX() - TILE_SLIDER_STARTING_X) / TILE_SLIDER_GAP_X) + 1;
		switch (tile_activeSliderType)
		{
		case 0:
			tile_activeSlider = &m_channelStrips[newSliderIndex].gainSlider;
			break;
		case 1:
			tile_activeSlider = &m_channelStrips[newSliderIndex].panSlider;
			break;
		case 2:
			tile_activeSlider = &m_channelStrips[newSliderIndex].distortionMixSlider;
			break;
		case 3:
			tile_activeSlider = &m_channelStrips[newSliderIndex].reverbMixSlider;
			break;
		case 4:
			tile_activeSlider = &m_channelStrips[newSliderIndex].phaserMixSlider;
			break;
		default:
			break;
		}
	}

	const double YToValue = 1.0 - ((static_cast<double>(editorPos.getY()) - tile_relativeYPos) / TILE_SLIDER_BG_HEIGHT);
	double newValue = juce::jlimit(tile_activeSlider->getMinimum(), tile_activeSlider->getMaximum(), YToValue);
	if (tile_activeSliderType == 1)
	{
		newValue = juce::jmap<double>(newValue, 0.0, 1.0, -1.0, 1.0);
	}
	tile_activeSlider->setValue(newValue, juce::sendNotificationSync);
}


void Exodus_2AudioProcessorEditor::tileMouseUp(const juce::MouseEvent&)
{
	if (!tile_activeSlider)
		return;

	tile_activeSlider = nullptr;
	tile_relativeYPos = 0.0;
	tile_activeSliderType = -1;
}


void Exodus_2AudioProcessorEditor::tileMouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
	if (wheel.isInertial)
		return;

	const auto mouseXPos = e.position.toInt().getX();
	if ((mouseXPos < TILE_SLIDER_STARTING_X) || (mouseXPos > TILE_SLIDER_ENDING_X))
	{
		return;
	}

	const int sliderIndex = (mouseXPos - TILE_SLIDER_STARTING_X) / TILE_SLIDER_GAP_X;
	const int mouseYPos = e.getMouseDownPosition().getY();
	if ((mouseYPos > GAIN_SLIDER_Y) && (mouseYPos < GAIN_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_relativeYPos = GAIN_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].gainSlider;
	}
	else if ((mouseYPos > PAN_SLIDER_Y) && (mouseYPos < PAN_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_relativeYPos = PAN_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].panSlider;
	}
	else if ((mouseYPos > DISTORTION_MIX_SLIDER_Y) && (mouseYPos < DISTORTION_MIX_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_relativeYPos = DISTORTION_MIX_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].distortionMixSlider;
	}
	else if ((mouseYPos > REVERB_MIX_SLIDER_Y) && (mouseYPos < REVERB_MIX_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_relativeYPos = REVERB_MIX_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].reverbMixSlider;
	}
	else if ((mouseYPos > PHASER_MIX_SLIDER_Y) && (mouseYPos < PHASER_MIX_SLIDER_Y + TILE_SLIDER_BG_HEIGHT))
	{
		tile_relativeYPos = PHASER_MIX_SLIDER_Y;
		tile_activeSlider = &m_channelStrips[sliderIndex].phaserMixSlider;
	}
	else
	{
		return;
	}

	const double newValue = juce::jlimit(tile_activeSlider->getMinimum(), tile_activeSlider->getMaximum(), tile_activeSlider->getValue() + wheel.deltaY);
	tile_activeSlider->setValue(newValue, juce::sendNotificationSync);

	tile_activeSlider = nullptr;
}

void Exodus_2AudioProcessorEditor::updateGeneralDelayTimeAttachment()
{
	m_delayTimeAttachment.reset();
	bool temp = m_generalDelayTempoToggleButton.getToggleState();
	if (temp == true)
	{
		m_delayTimeSlider.setRange(TIME_SYNCED_SLIDER_MIN_VALUE, TIME_SYNCED_SLIDER_MAX_VALUE, TIME_SYNCED_SLIDER_INTERVAL);
		m_delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, delayTempoSyncParamID, m_delayTimeSlider);
	}
	else
	{
		m_delayTimeSlider.setRange(GNRL_DELAY_TIME_SLIDER_MIN_VALUE, GNRL_DELAY_TIME_SLIDER_MAX_VALUE, GNRL_DELAY_TIME_SLIDER_INTERVAL);
		m_delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserTempoSyncParamID, m_delayTimeSlider);
	}
}

void Exodus_2AudioProcessorEditor::updatePhaserDelayTimeAttachment()
{
	m_phaserSettings.phaserRateAttachment.reset();
	if (m_phaserDelayTempoToggleButton.getToggleState() == true)
	{
		m_phaserSettings.phaserRateSlider.setRange(TIME_SYNCED_SLIDER_MIN_VALUE, TIME_SYNCED_SLIDER_MAX_VALUE, TIME_SYNCED_SLIDER_INTERVAL);
		m_phaserSettings.phaserRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserTempoSyncParamID, m_phaserSettings.phaserRateSlider);
	}
	else
	{		
		m_phaserSettings.phaserRateSlider.setRange(PHASER_RATE_SLIDER_MIN_VALUE, PHASER_RATE_SLIDER_MAX_VALUE);
		m_phaserSettings.phaserRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, phaserDepthParamID, m_phaserSettings.phaserRateSlider);
	}
}

//----------------------------------------------------------------------------------------------------------------

void Exodus_2AudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
	tileMouseDown(e);
}

void Exodus_2AudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
	tileMouseDrag(e);
}

void Exodus_2AudioProcessorEditor::mouseUp(const juce::MouseEvent& e)
{
	tileMouseUp(e);
}

void Exodus_2AudioProcessorEditor::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
	tileMouseWheelMove(e, wheel);
}