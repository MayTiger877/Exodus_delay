/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExodusAudioProcessor::ExodusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                    tree_state(*this, nullptr, "PARAMETER", create_parameter_layout()),
                    delay(), 
                    reverb()
#endif
{
}

ExodusAudioProcessor::~ExodusAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout ExodusAudioProcessor::create_parameter_layout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameters;

    parameters.add((std::make_unique<AudioParameterFloat>("m_input_gain_id", "m_input_gain_name", NormalisableRange<float>(-60.0, 6.0, 1.0), 0.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_output_gain_id", "m_output_gain_name", NormalisableRange<float>(-60.0, 6.0, 1.0), 0.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_delay_time_id", "m_delay_time_name", NormalisableRange<float>(0.0, 1500.0, 1.0), 500.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_delay_feedback_id", "m_delay_feedback_name", NormalisableRange<float>(0.0, 0.85, 0.01), 0.42)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_delay_mix_id", "m_delay_mix_name", NormalisableRange<float>(0.0, 100.0, 1.0), 40.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_room_size_id", "m_reverb_room_size_name", NormalisableRange<float>(0.0, 100.0, 1.0), 40.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_damping_id", "m_reverb_damping_name", NormalisableRange<float>(0.0, 100.0, 1.0), 40.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_width_id", "m_reverb_width_name", NormalisableRange<float>(0.0, 100.0, 1.0), 40.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_wet_level_id", "m_reverb_wet_level_name", NormalisableRange<float>(0.0, 100.0, 1.0), 40.0)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_dry_level_id", "m_reverb_dry_level_name", NormalisableRange<float>(0.0, 100.0, 1.0), 40.0)));


    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        std::string vol_dial_id = "m_vol_dial_id_";
        vol_dial_id.append(to_string(i));
        std::string vol_dial_name = "m_vol_dial_name_";
        vol_dial_name.append(to_string(i));
        parameters.add((std::make_unique<AudioParameterFloat>(vol_dial_id, vol_dial_name, NormalisableRange<float>(0.0, 1.5, 0.01), 1.0)));
        std::string pan_dial_id = "m_pan_dial_id_";
        pan_dial_id.append(to_string(i));
        std::string pan_dial_name = "m_pan_dial_name_";
        pan_dial_name.append(to_string(i));
        parameters.add((std::make_unique<AudioParameterFloat>(pan_dial_id, pan_dial_name, NormalisableRange<float>(-1.0, 1.0, 0.01), 0.0)));
        std::string on_off_button_id = "m_on_off_button_id_";
        on_off_button_id.append(to_string(i));
        std::string on_off_button_name = "m_on_off_button_name_";
        on_off_button_name.append(to_string(i));
        parameters.add((std::make_unique<AudioParameterBool>(on_off_button_id, on_off_button_name, false)));
        std::string reverb_button_id = "m_reverb_button_id_";
        reverb_button_id.append(to_string(i));
        std::string reverb_button_name = "m_reverb_button_name_";
        reverb_button_name.append(to_string(i));
        parameters.add((std::make_unique<AudioParameterBool>(reverb_button_id, reverb_button_name, false)));
    }

    return parameters;
}

void ExodusAudioProcessor::promoteInstence()
{
    current_instence++;
    current_instence %= NUM_OF_INSTENCES;
}

//==============================================================================
const juce::String ExodusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ExodusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ExodusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ExodusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ExodusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ExodusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ExodusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExodusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ExodusAudioProcessor::getProgramName (int index)
{
    return {};
}

void ExodusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ExodusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_visualiser.clear();
    m_visualiser_2.clear();

    delay.setSize(getNumInputChannels(), (sampleRate + samplesPerBlock) * 2);
    delay.setSampleRate(sampleRate);

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    reverb.prepare(spec);

}

void ExodusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ExodusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ExodusAudioProcessor::updateDelaySettings()
{
    delay_params.delay_mix = tree_state.getRawParameterValue("m_delay_mix_id")->load();
    delay_params.delay_time = tree_state.getRawParameterValue("m_delay_time_id")->load();
    delay_params.delay_feedback = tree_state.getRawParameterValue("m_delay_feedback_id")->load();

    delay.setParameters(delay_params);
}

void ExodusAudioProcessor::updateReverbSettings()
{
    reverb_params.roomSize = 0.7f; //tree_state.getParameter(ParamIDs::size)->getValue();
    reverb_params.damping =  0.4f; //tree_state.getParameter(ParamIDs::damp)->getValue();
    reverb_params.width =    0.7f; //tree_state.getParameter(ParamIDs::width)->getValue();
    reverb_params.wetLevel = 0.7f; //tree_state.getParameter(ParamIDs::dryWet)->getValue();
    reverb_params.dryLevel = 0.3f; //tree_state.getParameter(ParamIDs::dryWet)->getValue();

    reverb.setParameters(reverb_params);
}

void ExodusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    updateDelaySettings();
    updateReverbSettings();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int buffer_length = buffer.getNumSamples();

    dsp::AudioBlock<float> audio_block{ buffer };

    m_visualiser.pushBuffer(buffer);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(tree_state.getRawParameterValue("m_input_gain_id")->load());
        }

        float* dry_buffer = buffer.getWritePointer(channel);
        const float* buffer_data = buffer.getReadPointer(channel);
        AudioBuffer<float> tmp = buffer;

        if (delay.getOnOffMarked() > 0)
        {
            delay.fillDelayBuffer(channel, buffer_length, buffer_data, processor_buffer_write_pos);
            delay.getFromDelayBuffer(buffer, channel, buffer_length, delay.getNumSamples(), processor_buffer_write_pos);
            delay.feedbackDelay(channel, buffer_length, dry_buffer, processor_buffer_write_pos);
        }
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(tree_state.getRawParameterValue("m_output_gain_id")->load());
        }
    }
    m_visualiser_2.pushBuffer(buffer);
    
    processor_buffer_write_pos += buffer_length;
    processor_buffer_write_pos %= delay.getNumSamples();
}



//==============================================================================
bool ExodusAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ExodusAudioProcessor::createEditor()
{
    return new ExodusAudioProcessorEditor (*this);
}

//==============================================================================
void ExodusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ExodusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExodusAudioProcessor();
}
