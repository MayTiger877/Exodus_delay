/*
  ==============================================================================

    Phaser.h
    Created: 4 Dec 2025 3:35:56pm
    Author:  MayTigerStation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configs.h"

class MyPhaser
{
public:

    MyPhaser();
    ~MyPhaser() = default;

    void setRate(float newRateHz);
    void setDepth(float newDepth);
    void setCentreFrequency(float newCentreHz);
    void setFeedback(float newFeedback);
    void setLFOType(int newLFOType);
    void setMix(float newMix);
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();

    void process(const juce::dsp::ProcessContextReplacing<float>& context);

private:
    //==============================================================================
    void update();

    //==============================================================================
    juce::dsp::Oscillator<float> osc;
    juce::OwnedArray<juce::dsp::FirstOrderTPTFilter<float>> filters;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> oscVolume;
    std::vector<juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>> feedbackVolume{ 2 };
    juce::dsp::DryWetMixer<float> dryWet;
    std::vector<float> lastOutput{ 2 };
    juce::AudioBuffer<float> bufferFrequency;
    float normCentreFrequency = 0.5;
    double sampleRate = 44100.0;
	int phaser_LFOType = phaserLFOType_Sine;

    int updateCounter = 0;
    static constexpr int my_maxUpdateCounter = 4;

    float rate = 1.0, depth = 0.5, feedback = 0.0, mix = 0.5;
    float centreFrequency = 1300.0;
    static constexpr int numStages = 6;
};