/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayEngine.h"
#include "Parameters.h"

//==============================================================================
/**
*/
class Exodus_2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Exodus_2AudioProcessor();
    ~Exodus_2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	juce::AudioProcessorValueTreeState apvts;

    //----------------------

    double getBPM() const;

	DelayEngine* getDelayEngine() const { return m_delayEngine.get(); }
	void setIndex(int index) { m_index = index; }
    const float getDelayTimeInMs() const { return m_delayTimeInMs.get(); }

private:
	
    juce::Atomic<double> m_bpm{ DEFAULT_BPM };
    juce::Atomic<juce::AudioPlayHead::TimeSignature> m_timeSignature;
	const float getDelayTimeInSec() const;
    const float getPhaserTimeInSec() const;

    std::unique_ptr<DelayEngine> m_delayEngine;

	int m_index = 0;
	juce::Atomic<float> m_delayTimeInMs = 0.0f;

    juce::AudioProcessorValueTreeState::ParameterLayout creatParametersLayout();

	std::unique_ptr<parameters> m_parameters;

	int m_lastPPQPlayed = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Exodus_2AudioProcessor)
};
