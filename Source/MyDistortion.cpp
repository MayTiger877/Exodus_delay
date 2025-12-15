/*
  ==============================================================================

	Distortion.cpp
	Created: 18 Oct 2025 3:02:16pm
	Author:  MayTigerStation

  ==============================================================================
*/

#include "MyDistortion.h"

void MyDistortion::setDrive(float newDrive)
{
	jassert(newDrive >= DISTORTION_DRIVE_SLIDER_MIN_VALUE && newDrive <= DISTORTION_DRIVE_SLIDER_MAX_VALUE);
	dist_drive = newDrive;
}

void MyDistortion::setMix(float newMix)
{
	jassert(newMix >= 0.0f && newMix <= 1.0f);
	dist_mix = newMix;
}

void MyDistortion::setType(float newType)
{
	jassert(newType >= 0.0f && newType <= distType_numberOfTypes);

	dist_type = newType;
}

void MyDistortion::setThreshold(float newThreshold)
{
	jassert(newThreshold >= 0.0f && newThreshold <= 1.0f);
	dist_threshold = newThreshold;
}

void MyDistortion::processBuffer(juce::AudioBuffer<float>& buffer, int channel)
{
	float* channelData = buffer.getWritePointer(channel);
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		float inputSample = channelData[sample];
		float distortedSample = 0.0f;
		int distTypeInt = static_cast<int>(dist_type);
		switch (distTypeInt)
		{
		case distType_SoftClip:
			distortedSample = std::tanh(dist_drive * inputSample);
			break;
		case distType_HardClip:
			distortedSample = juce::jlimit(0 - dist_threshold, dist_threshold, inputSample * dist_drive);
			break;
		case distType_Exponential:
			distortedSample = (inputSample >= 0.0f) ? (1.0f - std::exp(dist_drive * inputSample)) : (-1.0f + std::exp(dist_drive * inputSample));
			break;
		default:
			distortedSample = inputSample;
			break;
		}
			
		channelData[sample] = (1.0f - dist_mix) * inputSample + dist_mix * distortedSample;
	}
}