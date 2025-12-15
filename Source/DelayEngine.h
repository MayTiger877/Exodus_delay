/*
  ==============================================================================

    DelayEngine.h
    Created: 18 Oct 2025 3:03:00pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"
#include "MyDistortion.h"
#include "MyPhaser.h"

struct DelayLineSettings
{
	float gain = 1.0f;
	float pan = 0.0f;
	float distortionMix = 0.8f;
	float reverbMix = 0.0f;
	float phaserMix = 0.0f;
	// Add more settings as needed
};

struct DelayParameters
{
	float delayTimeInSec = 0.300f;
	float feedback = 0.7f;
	float dryLevel = 0.8f;
	float wetLevel = 0.8f;
};

//----------------------------------------------------------------

class DelayEngine
{
public:
	DelayEngine();
	~DelayEngine() = default;

	void prepareToPlay(const double newSampleRate, const int newSamplesPerBlock);

	const DelayParameters getDelayEngineParameters() const { return d_delayParameters; }
	void setDelayEngineParameters(const DelayParameters newDelayParameters);

	void setDelayLineSettings(const int index, const DelayLineSettings newSettings);

	void fillDelayBuffer(const int channel, const int bufferLength, const float* bufferData);
	void fillFromDelayBuffer(const int channel, juce::AudioBuffer<float>& buffer, const int bufferLength);
	void applyEffectsAndCopyToBuffer(juce::AudioBuffer<float>& buffer, const int bufferLength, const int index);
	void feedbackDelay(const int channel, const int bufferLength);

	void incrementWritePosition(const int bufferSize);

	void setDistortionSettings(const int type, const float drive, const float threshold);

	void setPhaserSettings(const float type, const float rate, const float depth, const float feedback);

	void setReverbSettings(const float roomSize, const float damping, const float width);

private:
	juce::AudioBuffer<float> d_dryDelayBuffer;
	juce::AudioBuffer<float> d_wetDelayBuffer;
	juce::AudioBuffer<float> d_wetFeedbackDelayBuffer;

	double d_sampleRate = 44100;
	int d_samplesPerBlock = 512;
	int d_delayBufferLength = 0;
	int d_maxNumberOfChannels = 2;
	
	int d_writePosition = 0;
	int d_readPosition = 0;

	DelayParameters d_delayParameters;

	DelayLineSettings d_delayLines[16];

	MyDistortion d_distortion;

	MyPhaser d_phaser;

	juce::dsp::Reverb::Parameters reverbParams;
	juce::dsp::Reverb d_reverb;

	void applyDelayLineEffects(juce::AudioBuffer<float>& buffer, const int bufferLength, const int index);
	float calculatePanMargin(const int channel, const float pan);
};