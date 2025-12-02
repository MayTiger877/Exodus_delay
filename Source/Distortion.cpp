/*
  ==============================================================================

	Distortion.cpp
	Created: 18 Oct 2025 3:02:16pm
	Author:  MayTigerStation

  ==============================================================================
*/

#include "Distortion.h"

void Distortion::setDrive(float newDrive)
{
	jassert(newDrive >= 0.0f && newDrive <= 1.0);
	dist_drive = newDrive;
}

void Distortion::setMix(float newMix)
{
	jassert(newMix >= 0.0f && newMix <= 1.0f);
	dist_mix = newMix;
}

void Distortion::setType(int newType)
{
	jassert(newType >= 0 && newType <= distType_numberOfTypes);

	dist_type = newType;
}

void Distortion::processBuffer(juce::AudioBuffer<float>& buffer, int channel)
{
	float* channelData = buffer.getWritePointer(channel);
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		float inputSample = channelData[sample];
		float distortedSample = 0.0f;
		switch (dist_type)
		{
		case distType_SoftClip:
			distortedSample = std::tanh(dist_drive * inputSample);
			break;
		case distType_HardClip:
			distortedSample = juce::jlimit(-1.0f + dist_drive, 1.0f - dist_drive,  inputSample); // TODO: change the limits based on drive
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