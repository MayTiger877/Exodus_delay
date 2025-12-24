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
	jassert(channel >= 0 && channel < buffer.getNumChannels());
	jassert(dist_drive > 0.0f);

	float* channelData = buffer.getWritePointer(channel);
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		float inputSample = channelData[sample] * dist_drive;
		float distortedSample = 0.0f;
		int distTypeInt = static_cast<int>(dist_type);
		switch (distTypeInt)
		{
		case distType_SoftClip:
			distortedSample = std::tanh(inputSample);
			break;
		case distType_HardClip:
			distortedSample = juce::jlimit(0 - dist_threshold, dist_threshold, inputSample);
			break;
		case distType_Exponential:
			distortedSample = (inputSample >= 0.0f) ? (1.0f - std::exp(inputSample)) : (-1.0f + std::exp(inputSample));
			break;
		case distType_Foldback:
		{
			if ((inputSample > dist_threshold) || (inputSample < -dist_threshold))
				distortedSample = fabs(fabs(fmod(inputSample - dist_threshold, dist_threshold * 4)) - dist_threshold * 2) - dist_threshold;
			else
				distortedSample = inputSample;
			break;
		}
		default:
			distortedSample = inputSample;
			break;
		}
			
		distortedSample = distortedSample / dist_drive;
		channelData[sample] = (1.0f - dist_mix) * inputSample + dist_mix * distortedSample;
	}
}