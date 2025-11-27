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

    initiateChannelStrips();
	initiateGeneralSettings();
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
		strip.gainSlider.setLookAndFeel(&gainLAF);
        strip.gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        addAndMakeVisible(strip.gainSlider);
        strip.gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN_" + std::to_string(i), strip.gainSlider);
        
		strip.panSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, PAN_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
        strip.panSlider.setValue(PAN_SLIDER_DEFAULT_VALUE);
		strip.panSlider.setRange(PAN_SLIDER_MIN_VALUE, PAN_SLIDER_MAX_VALUE, PAN_SLIDER_INTERVAL);
		strip.panSlider.setLookAndFeel(&panLAF);
		strip.panSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        strip.panSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        addAndMakeVisible(strip.panSlider);
		strip.panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PAN_" + std::to_string(i), strip.panSlider);

		strip.distortionMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, DISTORTION_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.distortionMixSlider.setValue(DISTORTION_MIX_SLIDER_DEFAULT_VALUE);
		strip.distortionMixSlider.setRange(DISTORTION_MIX_SLIDER_MIN_VALUE, DISTORTION_MIX_SLIDER_MAX_VALUE, DISTORTION_MIX_SLIDER_INTERVAL);
		strip.distortionMixSlider.setLookAndFeel(&distortionLAF);
		strip.distortionMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.distortionMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		addAndMakeVisible(strip.distortionMixSlider);
		strip.distortionMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION_MIX_" + std::to_string(i), strip.distortionMixSlider);

		strip.reverbMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, REVERB_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.reverbMixSlider.setValue(REVERB_MIX_SLIDER_DEFAULT_VALUE);
		strip.reverbMixSlider.setRange(REVERB_MIX_SLIDER_MIN_VALUE, REVERB_MIX_SLIDER_MAX_VALUE, REVERB_MIX_SLIDER_INTERVAL);
		strip.reverbMixSlider.setLookAndFeel(&reverbLAF);
		strip.reverbMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.reverbMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		addAndMakeVisible(strip.reverbMixSlider);
		strip.reverbMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_MIX_" + std::to_string(i), strip.reverbMixSlider);

		strip.phaserMixSlider.setBounds(TILE_SLIDER_STARTING_X + i * TILE_SLIDER_GAP_X, PHASER_MIX_SLIDER_Y, TILE_SLIDER_BG_WIDTH, TILE_SLIDER_BG_HEIGHT);
		strip.phaserMixSlider.setValue(PHASER_MIX_SLIDER_DEFAULT_VALUE);
		strip.phaserMixSlider.setRange(PHASER_MIX_SLIDER_MIN_VALUE, PHASER_MIX_SLIDER_MAX_VALUE, PHASER_MIX_SLIDER_INTERVAL);
		strip.phaserMixSlider.setLookAndFeel(&phaserLAF);
		strip.phaserMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		strip.phaserMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
	m_delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	addAndMakeVisible(m_delayTimeSlider);
	m_delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_DELAY_TIME", m_delayTimeSlider);

	m_feedbackSlider.setValue(GNRL_FEEDBACK_SLIDER_DEFAULT_VALUE);
	m_feedbackSlider.setRange(GNRL_FEEDBACK_SLIDER_MIN_VALUE, GNRL_FEEDBACK_SLIDER_MAX_VALUE, GNRL_FEEDBACK_SLIDER_INTERVAL);
	m_feedbackSlider.setBounds(GNRL_FEEDBACK_SLIDER_BOUNDS);
	m_feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	addAndMakeVisible(m_feedbackSlider);
	m_feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_FEEDBACK", m_feedbackSlider);

	m_dryLevelSlider.setValue(GNRL_DRY_LEVEL_SLIDER_DEFAULT_VALUE);
	m_dryLevelSlider.setRange(GNRL_DRY_LEVEL_SLIDER_MIN_VALUE, GNRL_DRY_LEVEL_SLIDER_MAX_VALUE, GNRL_DRY_LEVEL_SLIDER_INTERVAL);
	m_dryLevelSlider.setBounds(GNRL_DRY_LEVEL_SLIDER_BOUNDS);
	m_dryLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	addAndMakeVisible(m_dryLevelSlider);
	m_dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_DRY_LEVEL", m_dryLevelSlider);

	m_wetLevelSlider.setValue(GNRL_WET_LEVEL_SLIDER_DEFAULT_VALUE);
	m_wetLevelSlider.setRange(GNRL_WET_LEVEL_SLIDER_MIN_VALUE, GNRL_WET_LEVEL_SLIDER_MAX_VALUE, GNRL_WET_LEVEL_SLIDER_INTERVAL);
	m_wetLevelSlider.setBounds(GNRL_WET_LEVEL_SLIDER_BOUNDS);
	m_wetLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	m_wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	addAndMakeVisible(m_wetLevelSlider);
	m_wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GNRL_WET_LEVEL", m_wetLevelSlider);

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

	g.setColour(juce::Colours::forestgreen.darker(0.8f));
	g.fillRect(TILE_SLIDER_STARTING_X - 1 + (m_index * TILE_SLIDER_GAP_X), 418, 40, 9);
}

void Exodus_2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
