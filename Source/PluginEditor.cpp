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

    initiateChannelStrips();
	initiateGeneralSettings();
	initiateEffectSettings();
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
	m_phaserSettings.phaserFeedbackSlider.setLookAndFeel(nullptr);

	m_reverbSettings.reverbRoomSizeSlider.setLookAndFeel(nullptr);
	m_reverbSettings.reverbDampingSlider.setLookAndFeel(nullptr);
	m_reverbSettings.reverbWidthSlider.setLookAndFeel(nullptr);
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
        strip.gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.gainSlider.setLookAndFeel(&gainLAF);
        addAndMakeVisible(strip.gainSlider);
        strip.gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN_" + std::to_string(i), strip.gainSlider);
        
		strip.panSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, PAN_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
        strip.panSlider.setValue(PAN_SLIDER_DEFAULT_VALUE);
		strip.panSlider.setRange(PAN_SLIDER_MIN_VALUE, PAN_SLIDER_MAX_VALUE, PAN_SLIDER_INTERVAL);
		strip.panSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        strip.panSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.panSlider.setLookAndFeel(&panLAF);
        addAndMakeVisible(strip.panSlider);
		strip.panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PAN_" + std::to_string(i), strip.panSlider);

		strip.distortionMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, DISTORTION_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.distortionMixSlider.setValue(DISTORTION_MIX_SLIDER_DEFAULT_VALUE);
		strip.distortionMixSlider.setRange(DISTORTION_MIX_SLIDER_MIN_VALUE, DISTORTION_MIX_SLIDER_MAX_VALUE, DISTORTION_MIX_SLIDER_INTERVAL);
		strip.distortionMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.distortionMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.distortionMixSlider.setLookAndFeel(&distortionLAF);
		addAndMakeVisible(strip.distortionMixSlider);
		strip.distortionMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION_MIX_" + std::to_string(i), strip.distortionMixSlider);

		strip.reverbMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, REVERB_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.reverbMixSlider.setValue(REVERB_MIX_SLIDER_DEFAULT_VALUE);
		strip.reverbMixSlider.setRange(REVERB_MIX_SLIDER_MIN_VALUE, REVERB_MIX_SLIDER_MAX_VALUE, REVERB_MIX_SLIDER_INTERVAL);
		strip.reverbMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.reverbMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.reverbMixSlider.setLookAndFeel(&reverbLAF);
		addAndMakeVisible(strip.reverbMixSlider);
		strip.reverbMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_MIX_" + std::to_string(i), strip.reverbMixSlider);

		strip.phaserMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, PHASER_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.phaserMixSlider.setValue(PHASER_MIX_SLIDER_DEFAULT_VALUE);
		strip.phaserMixSlider.setRange(PHASER_MIX_SLIDER_MIN_VALUE, PHASER_MIX_SLIDER_MAX_VALUE, PHASER_MIX_SLIDER_INTERVAL);
		strip.phaserMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.phaserMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		strip.phaserMixSlider.setLookAndFeel(&phaserLAF);
		addAndMakeVisible(strip.phaserMixSlider);
		strip.phaserMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_MIX_" + std::to_string(i), strip.phaserMixSlider);
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
	addAndMakeVisible(m_delayTimeSlider);
	m_delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_DELAY_TIME", m_delayTimeSlider);

	m_feedbackSlider.setValue(GNRL_FEEDBACK_SLIDER_DEFAULT_VALUE);
	m_feedbackSlider.setRange(GNRL_FEEDBACK_SLIDER_MIN_VALUE, GNRL_FEEDBACK_SLIDER_MAX_VALUE, GNRL_FEEDBACK_SLIDER_INTERVAL);
	m_feedbackSlider.setBounds(GNRL_FEEDBACK_SLIDER_BOUNDS);
	m_feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_feedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_feedbackSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_feedbackSlider);
	m_feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_FEEDBACK", m_feedbackSlider);

	m_dryLevelSlider.setValue(GNRL_DRY_LEVEL_SLIDER_DEFAULT_VALUE);
	m_dryLevelSlider.setRange(GNRL_DRY_LEVEL_SLIDER_MIN_VALUE, GNRL_DRY_LEVEL_SLIDER_MAX_VALUE, GNRL_DRY_LEVEL_SLIDER_INTERVAL);
	m_dryLevelSlider.setBounds(GNRL_DRY_LEVEL_SLIDER_BOUNDS);
	m_dryLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_dryLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_dryLevelSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_dryLevelSlider);
	m_dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_DRY_LEVEL", m_dryLevelSlider);

	m_wetLevelSlider.setValue(GNRL_WET_LEVEL_SLIDER_DEFAULT_VALUE);
	m_wetLevelSlider.setRange(GNRL_WET_LEVEL_SLIDER_MIN_VALUE, GNRL_WET_LEVEL_SLIDER_MAX_VALUE, GNRL_WET_LEVEL_SLIDER_INTERVAL);
	m_wetLevelSlider.setBounds(GNRL_WET_LEVEL_SLIDER_BOUNDS);
	m_wetLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_wetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_wetLevelSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_wetLevelSlider);
	m_wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_WET_LEVEL", m_wetLevelSlider);

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
	addAndMakeVisible(m_distortionSettings.distortionTypeSlider);
	m_distortionSettings.distortionTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION_TYPE", m_distortionSettings.distortionTypeSlider);

	m_distortionSettings.distortionDriveSlider.setValue(DISTORTION_DRIVE_SLIDER_DEFAULT_VALUE);
	m_distortionSettings.distortionDriveSlider.setRange(DISTORTION_DRIVE_SLIDER_MIN_VALUE, DISTORTION_DRIVE_SLIDER_MAX_VALUE, DISTORTION_DRIVE_SLIDER_INTERVAL);
	m_distortionSettings.distortionDriveSlider.setBounds(DISTORTION_DRIVE_SLIDER_BOUNDS);
	m_distortionSettings.distortionDriveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_distortionSettings.distortionDriveSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_distortionSettings.distortionDriveSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_distortionSettings.distortionDriveSlider);
	m_distortionSettings.distortionDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION_DRIVE", m_distortionSettings.distortionDriveSlider);

	m_distortionSettings.distortionThresholdSlider.setValue(DISTORTION_THRESHOLD_SLIDER_DEFAULT_VALUE);
	m_distortionSettings.distortionThresholdSlider.setRange(DISTORTION_THRESHOLD_SLIDER_MIN_VALUE, DISTORTION_THRESHOLD_SLIDER_MAX_VALUE, DISTORTION_THRESHOLD_SLIDER_INTERVAL);
	m_distortionSettings.distortionThresholdSlider.setBounds(DISTORTION_THRESHOLD_SLIDER_BOUNDS);
	m_distortionSettings.distortionThresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_distortionSettings.distortionThresholdSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_distortionSettings.distortionThresholdSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_distortionSettings.distortionThresholdSlider);
	m_distortionSettings.distortionThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION_THRESHOLD", m_distortionSettings.distortionThresholdSlider);

	// Phaser Settings
	m_phaserSettings.phaserTypeSlider.setValue(PHASER_TYPE_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserTypeSlider.setRange(PHASER_TYPE_SLIDER_MIN_VALUE, PHASER_TYPE_SLIDER_MAX_VALUE, PHASER_TYPE_SLIDER_INTERVAL);
	m_phaserSettings.phaserTypeSlider.setBounds(PHASER_TYPE_SLIDER_BOUNDS);
	m_phaserSettings.phaserTypeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserTypeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserTypeSlider.setLookAndFeel(&phaserKnobLAF);
	addAndMakeVisible(m_phaserSettings.phaserTypeSlider);
	m_phaserSettings.phaserTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_TYPE", m_phaserSettings.phaserTypeSlider);

	m_phaserSettings.phaserRateSlider.setValue(PHASER_RATE_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserRateSlider.setRange(PHASER_RATE_SLIDER_MIN_VALUE, PHASER_RATE_SLIDER_MAX_VALUE, PHASER_RATE_SLIDER_INTERVAL);
	m_phaserSettings.phaserRateSlider.setBounds(PHASER_RATE_SLIDER_BOUNDS);
	m_phaserSettings.phaserRateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserRateSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserRateSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_phaserSettings.phaserRateSlider);
	m_phaserSettings.phaserRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_RATE", m_phaserSettings.phaserRateSlider);

	m_phaserSettings.phaserDepthSlider.setValue(PHASER_DEPTH_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserDepthSlider.setRange(PHASER_DEPTH_SLIDER_MIN_VALUE, PHASER_DEPTH_SLIDER_MAX_VALUE, PHASER_DEPTH_SLIDER_INTERVAL);
	m_phaserSettings.phaserDepthSlider.setBounds(PHASER_DEPTH_SLIDER_BOUNDS);
	m_phaserSettings.phaserDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserDepthSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_phaserSettings.phaserDepthSlider);
	m_phaserSettings.phaserDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_DEPTH", m_phaserSettings.phaserDepthSlider);

	m_phaserSettings.phaserFeedbackSlider.setValue(PHASER_FEEDBACK_SLIDER_DEFAULT_VALUE);
	m_phaserSettings.phaserFeedbackSlider.setRange(PHASER_FEEDBACK_SLIDER_MIN_VALUE, PHASER_FEEDBACK_SLIDER_MAX_VALUE, PHASER_FEEDBACK_SLIDER_INTERVAL);
	m_phaserSettings.phaserFeedbackSlider.setBounds(PHASER_FEEDBACK_SLIDER_BOUNDS);
	m_phaserSettings.phaserFeedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_phaserSettings.phaserFeedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_phaserSettings.phaserFeedbackSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_phaserSettings.phaserFeedbackSlider);
	m_phaserSettings.phaserFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_FEEDBACK", m_phaserSettings.phaserFeedbackSlider);

	// Reverb Settings
	m_reverbSettings.reverbRoomSizeSlider.setValue(REVERB_ROOM_SIZE_SLIDER_DEFAULT_VALUE);
	m_reverbSettings.reverbRoomSizeSlider.setRange(REVERB_ROOM_SIZE_SLIDER_MIN_VALUE, REVERB_ROOM_SIZE_SLIDER_MAX_VALUE, REVERB_ROOM_SIZE_SLIDER_INTERVAL);
	m_reverbSettings.reverbRoomSizeSlider.setBounds(REVERB_ROOM_SIZE_SLIDER_BOUNDS);
	m_reverbSettings.reverbRoomSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_reverbSettings.reverbRoomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_reverbSettings.reverbRoomSizeSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_reverbSettings.reverbRoomSizeSlider);
	m_reverbSettings.reverbRoomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_ROOM_SIZE", m_reverbSettings.reverbRoomSizeSlider);

	m_reverbSettings.reverbDampingSlider.setValue(REVERB_DAMPING_SLIDER_DEFAULT_VALUE);
	m_reverbSettings.reverbDampingSlider.setRange(REVERB_DAMPING_SLIDER_MIN_VALUE, REVERB_DAMPING_SLIDER_MAX_VALUE, REVERB_DAMPING_SLIDER_INTERVAL);
	m_reverbSettings.reverbDampingSlider.setBounds(REVERB_DAMPING_SLIDER_BOUNDS);
	m_reverbSettings.reverbDampingSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_reverbSettings.reverbDampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_reverbSettings.reverbDampingSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_reverbSettings.reverbDampingSlider);
	m_reverbSettings.reverbDampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_DAMPING", m_reverbSettings.reverbDampingSlider);

	m_reverbSettings.reverbWidthSlider.setValue(REVERB_WIDTH_SLIDER_DEFAULT_VALUE);
	m_reverbSettings.reverbWidthSlider.setRange(REVERB_WIDTH_SLIDER_MIN_VALUE, REVERB_WIDTH_SLIDER_MAX_VALUE, REVERB_WIDTH_SLIDER_INTERVAL);
	m_reverbSettings.reverbWidthSlider.setBounds(REVERB_WIDTH_SLIDER_BOUNDS);
	m_reverbSettings.reverbWidthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_reverbSettings.reverbWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
	m_reverbSettings.reverbWidthSlider.setLookAndFeel(&costumeKnobLAF);
	addAndMakeVisible(m_reverbSettings.reverbWidthSlider);
	m_reverbSettings.reverbWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_WIDTH", m_reverbSettings.reverbWidthSlider);
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
