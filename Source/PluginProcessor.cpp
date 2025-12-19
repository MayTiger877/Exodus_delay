/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MyDistortion.h"

//==============================================================================
Exodus_2AudioProcessor::Exodus_2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
	                 ), apvts(*this, nullptr, "PARAMETER", creatParametersLayout())
#endif
{
	m_delayEngine = std::make_unique<DelayEngine>();

	m_parameters = std::make_unique<parameters>(apvts);

}

Exodus_2AudioProcessor::~Exodus_2AudioProcessor()
{
}

//==============================================================================
const juce::String Exodus_2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Exodus_2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Exodus_2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Exodus_2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Exodus_2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Exodus_2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Exodus_2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Exodus_2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Exodus_2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Exodus_2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Exodus_2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	m_delayEngine->prepareToPlay(sampleRate, samplesPerBlock);
}

void Exodus_2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Exodus_2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


static void test__Distortion(juce::AudioBuffer<float>& buffer)
{
    MyDistortion distortion;
    distortion.setMix(1.0f);
    distortion.setDrive(1.0f); // You can set drive value as needed
    distortion.setType(distType_SoftClip); // You can set distortion type as needed
    distortion.processBuffer(buffer, 0);
    distortion.processBuffer(buffer, 1);
}

static void test__Phaser(juce::AudioBuffer<float>& buffer, juce::dsp::Phaser<float>& phaser)
{
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    phaser.process(context);
}

static void test__Reverb(juce::AudioBuffer<float>& buffer, juce::dsp::Reverb& reverb)
{
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    reverb.process(context);
}

void Exodus_2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    m_delayEngine->setDelayEngineParameters({
        m_parameters->delayTimeParam->get(),
				m_parameters->delayFeedbackParam->get(),
				m_parameters->delayDryLevelParam->get(),
				m_parameters->delayWetLevelParam->get() });

    m_delayEngine->setDelayLineSettings(m_index, {
        m_parameters->gainMixParam[m_index]->get(),
        m_parameters->panMixParam[m_index]->get(),
        m_parameters->distortionMixParam[m_index]->get(),
        m_parameters->reverbMixParam[m_index]->get(),
        m_parameters->phaserMixParam[m_index]->get() });

    m_delayEngine->setDistortionSettings(
        m_parameters->distortionTypeParam->get(),
		m_parameters->distortionDriveParam->get(),
		m_parameters->distortionThresholdParam->get());

	m_delayEngine->setPhaserSettings(
		m_parameters->phaserTypeParam->get(),
		m_parameters->phaserRateParam->get(),
		m_parameters->phaserDepthParam->get(),
		m_parameters->phaserFeedbackParam->get(),
		m_parameters->phaserMixParam[m_index]->get());

    m_delayEngine->setReverbSettings(
		m_parameters->reverbRoomSizeParam->get(),
		m_parameters->reverbDampingParam->get(),
		m_parameters->reverbWidthParam->get(),
		m_parameters->reverbMixParam[m_index]->get());


    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamples = buffer.getNumSamples();

	this->m_delayTimeInMs = m_delayEngine->getDelayEngineParameters().delayTimeInSec * 1000.0f;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, numSamples);
    }

    if (numSamples == 0)
        return;

	//test__Phaser(buffer, m_delayEngine->d_phaser);
    //return;

	//test__Reverb(buffer, m_delayEngine->d_reverb);
    //return;

	m_delayEngine->fillDelayBuffer(0, numSamples, buffer.getReadPointer(0));
    m_delayEngine->fillDelayBuffer(1, numSamples, buffer.getReadPointer(1));

	m_delayEngine->fillFromDelayBuffer(0, buffer, numSamples);
    m_delayEngine->fillFromDelayBuffer(1, buffer, numSamples);

	m_delayEngine->applyEffectsAndCopyToBuffer(buffer, numSamples, m_index);

	m_delayEngine->feedbackDelay(0, numSamples);
    m_delayEngine->feedbackDelay(1, numSamples);

	m_delayEngine->incrementWritePosition(numSamples);

}

//==============================================================================
bool Exodus_2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Exodus_2AudioProcessor::createEditor()
{
    return new Exodus_2AudioProcessorEditor (*this);
}

//==============================================================================
void Exodus_2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Exodus_2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Exodus_2AudioProcessor();
}

// Parameter Layout
juce::AudioProcessorValueTreeState::ParameterLayout Exodus_2AudioProcessor::creatParametersLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for (int i = 0; i < 16; ++i)
    {
        layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN_" + std::to_string(i), "Gain " + std::to_string(i),
            juce::NormalisableRange<float>(GAIN_SLIDER_MIN_VALUE, GAIN_SLIDER_MAX_VALUE, GAIN_SLIDER_INTERVAL), GAIN_SLIDER_DEFAULT_VALUE));
        
        layout.add(std::make_unique<juce::AudioParameterFloat>("PAN_" + std::to_string(i), "Pan " + std::to_string(i),
            juce::NormalisableRange<float>(PAN_SLIDER_MIN_VALUE, PAN_SLIDER_MAX_VALUE, PAN_SLIDER_INTERVAL), PAN_SLIDER_DEFAULT_VALUE));
		
        layout.add(std::make_unique<juce::AudioParameterFloat>("DISTORTION_MIX_" + std::to_string(i), "Distortion Mix " + std::to_string(i),
			juce::NormalisableRange<float>(DISTORTION_MIX_SLIDER_MIN_VALUE, DISTORTION_MIX_SLIDER_MAX_VALUE, DISTORTION_MIX_SLIDER_INTERVAL), DISTORTION_MIX_SLIDER_DEFAULT_VALUE));
		
        layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_MIX_" + std::to_string(i), "Reverb Mix " + std::to_string(i),
			juce::NormalisableRange<float>(REVERB_MIX_SLIDER_MIN_VALUE, REVERB_MIX_SLIDER_MAX_VALUE, REVERB_MIX_SLIDER_INTERVAL), REVERB_MIX_SLIDER_DEFAULT_VALUE));
		
        layout.add(std::make_unique<juce::AudioParameterFloat>("PHASER_MIX_" + std::to_string(i), "Phaser Mix " + std::to_string(i),
			juce::NormalisableRange<float>(PHASER_MIX_SLIDER_MIN_VALUE, PHASER_MIX_SLIDER_MAX_VALUE, PHASER_MIX_SLIDER_INTERVAL), PHASER_MIX_SLIDER_DEFAULT_VALUE));
    }

    layout.add(std::make_unique<juce::AudioParameterFloat>("GNRL_DELAY_TIME", "General Delay Time",
		juce::NormalisableRange<float>(GNRL_DELAY_TIME_SLIDER_MIN_VALUE, GNRL_DELAY_TIME_SLIDER_MAX_VALUE, GNRL_DELAY_TIME_SLIDER_INTERVAL), GNRL_DELAY_TIME_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("GNRL_FEEDBACK", "General Feedback",
		juce::NormalisableRange<float>(GNRL_FEEDBACK_SLIDER_MIN_VALUE, GNRL_FEEDBACK_SLIDER_MAX_VALUE, GNRL_FEEDBACK_SLIDER_INTERVAL), GNRL_FEEDBACK_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("GNRL_DRY_LEVEL", "General Dry Level",
		juce::NormalisableRange<float>(GNRL_DRY_LEVEL_SLIDER_MIN_VALUE, GNRL_DRY_LEVEL_SLIDER_MAX_VALUE, GNRL_DRY_LEVEL_SLIDER_INTERVAL), GNRL_DRY_LEVEL_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("GNRL_WET_LEVEL", "General Wet Level",
		juce::NormalisableRange<float>(GNRL_WET_LEVEL_SLIDER_MIN_VALUE, GNRL_WET_LEVEL_SLIDER_MAX_VALUE, GNRL_WET_LEVEL_SLIDER_INTERVAL), GNRL_WET_LEVEL_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("DISTORTION_TYPE", "Distortion Type",
		juce::NormalisableRange<float>(DISTORTION_TYPE_SLIDER_MIN_VALUE, DISTORTION_TYPE_SLIDER_MAX_VALUE, DISTORTION_TYPE_SLIDER_INTERVAL), DISTORTION_TYPE_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("DISTORTION_DRIVE", "Distortion Drive",
        juce::NormalisableRange<float>(DISTORTION_DRIVE_SLIDER_MIN_VALUE, DISTORTION_DRIVE_SLIDER_MAX_VALUE, DISTORTION_DRIVE_SLIDER_INTERVAL), DISTORTION_DRIVE_SLIDER_DEFAULT_VALUE));

    layout.add(std::make_unique<juce::AudioParameterFloat>("DISTORTION_THRESHOLD", "Distortion Threshold",
		juce::NormalisableRange<float>(DISTORTION_THRESHOLD_SLIDER_MIN_VALUE, DISTORTION_THRESHOLD_SLIDER_MAX_VALUE, DISTORTION_THRESHOLD_SLIDER_INTERVAL), DISTORTION_THRESHOLD_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("PHASER_TYPE", "Phaser Type",
		juce::NormalisableRange<float>(PHASER_TYPE_SLIDER_MIN_VALUE, PHASER_TYPE_SLIDER_MAX_VALUE, PHASER_TYPE_SLIDER_INTERVAL), PHASER_TYPE_SLIDER_DEFAULT_VALUE));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("PHASER_RATE", "Phaser Rate",
        juce::NormalisableRange<float>(PHASER_RATE_SLIDER_MIN_VALUE, PHASER_RATE_SLIDER_MAX_VALUE, PHASER_RATE_SLIDER_INTERVAL), PHASER_RATE_SLIDER_DEFAULT_VALUE));

    layout.add(std::make_unique<juce::AudioParameterFloat>("PHASER_DEPTH", "Phaser Depth",
		juce::NormalisableRange<float>(PHASER_DEPTH_SLIDER_MIN_VALUE, PHASER_DEPTH_SLIDER_MAX_VALUE, PHASER_DEPTH_SLIDER_INTERVAL), PHASER_DEPTH_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("PHASER_FEEDBACK", "Phaser Feedback",
		juce::NormalisableRange<float>(PHASER_FEEDBACK_SLIDER_MIN_VALUE, PHASER_FEEDBACK_SLIDER_MAX_VALUE, PHASER_FEEDBACK_SLIDER_INTERVAL), PHASER_FEEDBACK_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_ROOM_SIZE", "Reverb Room Size",
		juce::NormalisableRange<float>(REVERB_ROOM_SIZE_SLIDER_MIN_VALUE, REVERB_ROOM_SIZE_SLIDER_MAX_VALUE, REVERB_ROOM_SIZE_SLIDER_INTERVAL), REVERB_ROOM_SIZE_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_DAMPING", "Reverb Damping",
		juce::NormalisableRange<float>(REVERB_DAMPING_SLIDER_MIN_VALUE, REVERB_DAMPING_SLIDER_MAX_VALUE, REVERB_DAMPING_SLIDER_INTERVAL), REVERB_DAMPING_SLIDER_DEFAULT_VALUE));

	layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_WIDTH", "Reverb Width",
		juce::NormalisableRange<float>(REVERB_WIDTH_SLIDER_MIN_VALUE, REVERB_WIDTH_SLIDER_MAX_VALUE, REVERB_WIDTH_SLIDER_INTERVAL), REVERB_WIDTH_SLIDER_DEFAULT_VALUE));

    return layout;
}