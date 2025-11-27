/*
  ==============================================================================

	Distortion.h
	Created: 18 Oct 2025 3:02:16pm
	Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

typedef enum
{
	distType_SoftClip = 1,
	distType_HardClip = 2,
	distType_Exponential = 3,
	distType_numberOfTypes = 3
}DistortionType;

class Distortion
{
public:
	Distortion() = default;
	~Distortion() = default;

	void setDrive(float newDrive);
	void setMix(float newMix);
	void setType(int newType);

	void processBuffer(juce::AudioBuffer<float>& buffer, int channel);

private:
	float dist_drive = 0.0f;
	float dist_mix = 0.0f;
	int dist_type = distType_SoftClip;
};


// graphics

struct DistortionTab
{
	juce::Slider distortionDriveSlider, distortionMixSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distortionDrive, distortionMix;

	juce::ToggleButton distTypeSoftClipButton, distTypeHardClipButton, distTypeExponentialButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> distTypeSoftClipAttachment, distTypeHardClipAttachment, distTypeExponentialAttachment;
};