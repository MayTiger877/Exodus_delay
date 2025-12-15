/*
  ==============================================================================

	Distortion.h
	Created: 18 Oct 2025 3:02:16pm
	Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"


class MyDistortion
{
public:
	MyDistortion() = default;
	~MyDistortion() = default;

	void setDrive(float newDrive);
	void setMix(float newMix);
	void setType(float newType);
	void setThreshold(float newThreshold);

	void processBuffer(juce::AudioBuffer<float>& buffer, int channel);

private:
	float dist_drive = DISTORTION_DRIVE_SLIDER_DEFAULT_VALUE;
	float dist_mix = 0.0f;
	float dist_threshold = DISTORTION_THRESHOLD_SLIDER_DEFAULT_VALUE;
	float dist_type = DISTORTION_TYPE_SLIDER_DEFAULT_VALUE;
};
