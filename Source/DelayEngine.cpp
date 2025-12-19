/*/*
  ==============================================================================

    DelayEngine.cpp
    Created: 18 Oct 2025 3:03:00pm
    Author:  MayTigerStation

  ==============================================================================
*/

#include "DelayEngine.h"
#include <cmath>

DelayEngine::DelayEngine() : d_dryDelayBuffer(), d_wetDelayBuffer(), d_phaser()
{
	d_distortion.setDrive(2.0f);
	d_distortion.setType(distType_SoftClip);
	d_distortion.setMix(0.8f);

	d_phaser.setRate(0.5f);
	d_phaser.setDepth(0.5f);
	d_phaser.setCentreFrequency(1000.0f);
	d_phaser.setFeedback(0.0f);
	d_phaser.setMix(0.5f);

	reverbParams.roomSize = 0.8f;
	reverbParams.damping  = 0.2f;
	reverbParams.wetLevel = 1.0f;
	reverbParams.dryLevel = 0.0f;
	reverbParams.width    = 0.7f;
	reverbParams.freezeMode = 0.0f;

	d_reverb.setParameters(reverbParams);
	d_reverb.setEnabled(true);

}

void DelayEngine::prepareToPlay(const double newSampleRate, const int newSamplesPerBlock)
{
    d_sampleRate = newSampleRate;
    d_samplesPerBlock = newSamplesPerBlock;

    d_delayBufferLength = d_sampleRate * 2; // 2 seconds max delay
    d_dryDelayBuffer.setSize(2, d_delayBufferLength); // Stereo buffer
	d_wetDelayBuffer.setSize(2, d_samplesPerBlock); // Stereo buffer for wet signal
    d_wetFeedbackDelayBuffer.setSize(2, d_samplesPerBlock); // Stereo buffer for wet signal

    d_dryDelayBuffer.clear();
	d_wetDelayBuffer.clear();
    d_wetFeedbackDelayBuffer.clear();
    d_writePosition = 0;

    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = d_sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (d_samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (d_maxNumberOfChannels);
	//const juce::dsp::ProcessSpec spec{ d_sampleRate, static_cast<juce::uint32> (d_samplesPerBlock), 2 };
	
	d_phaser.reset();
	d_reverb.reset();

    d_phaser.prepare(spec);
	d_reverb.prepare(spec);
}

void DelayEngine::setDelayEngineParameters(const DelayParameters newDelayParameters)
{
    d_delayParameters.delayTimeInSec = newDelayParameters.delayTimeInSec;
    d_delayParameters.feedback = newDelayParameters.feedback;
    d_delayParameters.dryLevel = newDelayParameters.dryLevel;
    d_delayParameters.wetLevel = newDelayParameters.wetLevel;
}

void DelayEngine::setDelayLineSettings(const int index, const DelayLineSettings newSettings)
{
    if (index >= 0 && index < 16)
    {
        d_delayLines[index] = newSettings;
    }
}

void DelayEngine::setDistortionSettings(const int type, const float drive, const float threshold)
{
    d_distortion.setType(static_cast<DistortionType>(type));
    d_distortion.setDrive(drive);
	d_distortion.setThreshold(threshold); // You can parameterize threshold as needed
}

void DelayEngine::setPhaserSettings(const float type, const float rate, const float depth, const float feedback, const float mix)
{
    d_phaser.setLFOType(static_cast<int>(type));
    d_phaser.setRate(rate);
    d_phaser.setDepth(depth);
    d_phaser.setFeedback(feedback);
	d_phaser.setMix(mix);
	d_phaser.setCentreFrequency(1000.0f); // You can parameterize centre frequency as needed TODO: add this parameter
}

void DelayEngine::setReverbSettings(const float roomSize, const float damping, const float width, const float mix)
{
    reverbParams.roomSize = juce::jlimit(0.0f, 1.0f, roomSize);
    reverbParams.damping = juce::jlimit(0.0f, 1.0f, damping);
    reverbParams.width = juce::jlimit(0.0f, 1.0f, width);
	reverbParams.wetLevel = mix;
	reverbParams.dryLevel = 1.0f - mix;
	d_reverb.setParameters(reverbParams);
}

/***************************************************************************************/

void DelayEngine::fillDelayBuffer(const int channel, const int bufferLength, const float* bufferData)
{

    if (d_delayBufferLength > d_writePosition + bufferLength)
    {
        d_dryDelayBuffer.copyFromWithRamp(channel, d_writePosition, bufferData, bufferLength, 1.0f, 1.0f);
    }
    else
    {
        const int bufferRemaining = d_delayBufferLength - d_writePosition;
        d_dryDelayBuffer.copyFromWithRamp(channel, d_writePosition, bufferData, bufferRemaining, 1.0f, 1.0f);
        d_dryDelayBuffer.copyFromWithRamp(channel, 0, bufferData + bufferRemaining, bufferLength - bufferRemaining, 1.0f, 1.0f);
    }
}

void DelayEngine::fillFromDelayBuffer(const int channel, juce::AudioBuffer<float>& buffer, const int bufferLength)
{
    const int readPosition = static_cast<int>(d_delayBufferLength + d_writePosition - (d_delayParameters.delayTimeInSec * d_sampleRate)) % d_delayBufferLength;
    const float* dryDelayBufferData = d_dryDelayBuffer.getReadPointer(channel);
    
    if (d_delayBufferLength > readPosition + bufferLength)
    {
        d_wetDelayBuffer.copyFrom(channel, 0, (dryDelayBufferData + readPosition), bufferLength);
    }
    else
    {
        const int bufferRemaining = d_delayBufferLength - readPosition;
        d_wetDelayBuffer.copyFrom(channel, 0, (dryDelayBufferData + readPosition), bufferRemaining);
        d_wetDelayBuffer.copyFrom(channel, bufferRemaining, dryDelayBufferData, (bufferLength - bufferRemaining));
    }
	
	d_wetFeedbackDelayBuffer.copyFrom(channel, 0, d_wetDelayBuffer.getReadPointer(channel), bufferLength);
}

void DelayEngine::applyEffectsAndCopyToBuffer(juce::AudioBuffer<float>& buffer, const int bufferLength, const int index)
{
    jassert(d_wetDelayBuffer.getNumSamples() >= bufferLength);
    jassert(d_wetDelayBuffer.getNumChannels() >= 2);

    //applyDelayLineEffects(buffer, bufferLength, index);

    // apply dry level
    
    buffer.applyGain(d_delayParameters.dryLevel);
    
    // add wet and apply wet level
    buffer.addFrom(0, 0, d_wetDelayBuffer, 0, 0, bufferLength, d_delayParameters.wetLevel);
    buffer.addFrom(1, 0, d_wetDelayBuffer, 1, 0, bufferLength, d_delayParameters.wetLevel);
}


void DelayEngine::feedbackDelay(const int channel, const int bufferLength)
{
	const float* dryDelayBufferData = d_wetFeedbackDelayBuffer.getReadPointer(channel);

    if (d_delayBufferLength > d_writePosition + bufferLength)
    {
        d_dryDelayBuffer.addFromWithRamp(channel, d_writePosition, dryDelayBufferData, bufferLength, d_delayParameters.feedback, d_delayParameters.feedback);
    }
    else
    {
        const int bufferRemaining = d_delayBufferLength - d_writePosition;
        d_dryDelayBuffer.addFromWithRamp(channel, d_writePosition, dryDelayBufferData, bufferRemaining,d_delayParameters.feedback, d_delayParameters.feedback);
        d_dryDelayBuffer.addFromWithRamp(channel, 0, (dryDelayBufferData + bufferRemaining), (bufferLength - bufferRemaining), d_delayParameters.feedback, d_delayParameters.feedback);
    }

    d_wetDelayBuffer.clear();
}


void DelayEngine::incrementWritePosition(const int bufferSize)
{
    d_writePosition = (d_writePosition + bufferSize) % d_delayBufferLength;
}

float DelayEngine::calculatePanMargin(const int channel, const float pan)
{
    if (channel == 0)
    {
        if (pan < 0)
        {
            return 1;
        }
        else if (pan > 0)
        {
            return 1 - (pan * pan);
        }
    }
    else if (channel == 1)
    {
        if (pan > 0)
        {
            return 1;
        }
        else if (pan < 0)
        {
            return 1 - (pan * pan);
        }
    }

    return 1;
}

void DelayEngine::applyDelayLineEffects(juce::AudioBuffer<float>& Buffer, const int bufferLength, const int index)
{   
    jassert(d_wetDelayBuffer.getNumSamples() >= bufferLength);

	DelayLineSettings currentSettings = d_delayLines[index];

    // apply gain and pan
    float calculatedPanGain = calculatePanMargin(0, currentSettings.pan);
	Buffer.applyGain(0, 0, bufferLength, currentSettings.gain * calculatedPanGain);
    calculatedPanGain = calculatePanMargin(1, currentSettings.pan);
    Buffer.applyGain(1, 0, bufferLength, currentSettings.gain * calculatedPanGain);

	// apply distortion- mix is handled inside distortion class
	d_distortion.setMix(currentSettings.distortionMix);
	d_distortion.processBuffer(Buffer, 0);
    d_distortion.processBuffer(Buffer, 1);

	// apply phaser
    //juce::dsp::AudioBlock<float> block(buffer);
    //juce::dsp::ProcessContextReplacing<float> context(block);    
    //d_phaser.process(context);

	// apply reverb
    juce::dsp::AudioBlock<float> block_2(Buffer);
	juce::dsp::AudioBlock<float> block_2_Sub = block_2.getSubBlock(0, bufferLength);
    juce::dsp::ProcessContextReplacing<float> context_2(block_2);

    //d_reverb.process(context_2);
}