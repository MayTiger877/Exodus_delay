/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
<<<<<<< HEAD
#include "PluginProcessor.h"

using namespace juce;
using namespace std;
//==============================================================================
/**
*/
class ExodusAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Button::Listener,
    public juce::Timer

{
public:
    ExodusAudioProcessorEditor(ExodusAudioProcessor&);
    ~ExodusAudioProcessorEditor() override;

    void buttonClicked(Button* button) override;
    void reAlphaComponents();
    void initiateComponents(AudioProcessor&);
    void printComponents();
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    ToggleButton instence_indicator;

    void timerCallback() override;

    double winSizeRatio;

    // Input Gain
    Slider m_input_gain;
    Label m_input_gain_label;

    // Output Gain
    Slider m_output_gain;
    Label m_output_gain_label;

    // Volume
    Slider m_volume_dials[NUM_OF_INSTENCES];

    // Pan
    Slider m_pan_dials[NUM_OF_INSTENCES];

    // On/Off
    ToggleButton m_on_off_buttons[NUM_OF_INSTENCES];

    // Reverb
    ToggleButton m_reverb_buttons[NUM_OF_INSTENCES];

    // Distortion
    ToggleButton m_dist_buttons[NUM_OF_INSTENCES];

    // Lables
    Label m_volume_dials_label;
    Label m_pan_dials_label;
    Label m_on_off_buttons_label;
    Label m_reverb_buttons_label;
    Label m_dist_buttons_label;

    // Delay time
    Slider m_delay_time;
    Label m_delay_time_label;

    // Delay feedback
    Slider m_delay_feedback;
    Label m_delay_feedback_label;

    // Delay mix
    Slider m_delay_mix;
    Label m_delay_mix_label;

    //Reverb room size
    Slider m_reverb_room_size;
    Label m_reverb_room_size_label;

    //Reverb damping
    Slider m_reverb_damping;
    Label m_reverb_damping_label;

    //Reverb width
    Slider m_reverb_width;
    Label m_reverb_width_label;

    //Reverb wet level
    Slider m_reverb_wet_level;
    Label m_reverb_wet_level_label;

    //Reverb dry level
    Slider m_reverb_dry_level;
    Label m_reverb_dry_level_label;

    //Dist drive
    Slider m_dist_drive;
    Label m_dist_drive_label;

    //Dist dry level
    Slider m_dist_dry_level;
    Label m_dist_dry_level_label;

    //Dist wet level
    Slider m_dist_wet_level;
    Label m_dist_wet_level_label;

    //Dist brightness
    Slider m_dist_brightness;
    Label m_dist_brightness_label;

    // Backgroungd 
    ImageComponent m_background;

public:

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_input_gain_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_output_gain_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_time_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_feedback_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_mix_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverb_room_size_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverb_damping_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverb_width_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverb_wet_level_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverb_dry_level_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_dist_drive_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_dist_brightness_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_dist_wet_level_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_dist_dry_level_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_volume_dials_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pan_dials_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_on_off_buttons_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_reverb_buttons_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_dist_buttons_attach[NUM_OF_INSTENCES];

    ExodusAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExodusAudioProcessorEditor)
=======
#include "Configs.h"
#include "PluginProcessor.h"
#include "TileSlider.h"
#include <chrono>

//==============================================================================
/**                       
*/

struct channelStrip
{
    juce::Slider gainSlider, panSlider, distortionMixSlider, reverbMixSlider, phaserMixSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment, panAttachment, distortionMixAttachment, reverbMixAttachment, phaserMixAttachment;
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

    std::unique_ptr<juce::Drawable> backgroundDrawable;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Exodus_2AudioProcessorEditor)
>>>>>>> master
};
