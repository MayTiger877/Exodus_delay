<<<<<<< HEAD
﻿/*
=======
/*
>>>>>>> master
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
<<<<<<< HEAD

//==============================================================================
ExodusAudioProcessor::ExodusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    tree_state(*this, nullptr, "PARAMETER", create_parameter_layout()),
    reverb(),
    distortion(),
    current_instence(0), processor_buffer_write_pos(0),
    on_off_marked(INSTENCE_OFF), reverb_marked(INSTENCE_OFF), dist_marked(INSTENCE_OFF),
    d_on_off{ INSTENCE_OFF }, d_reverb{ INSTENCE_OFF }, d_dist{ INSTENCE_OFF }
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
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_room_size_id", "m_reverb_room_size_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_damping_id", "m_reverb_damping_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_width_id", "m_reverb_width_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_wet_level_id", "m_reverb_wet_level_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_reverb_dry_level_id", "m_reverb_dry_level_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_dist_drive_id", "m_dist_drive_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_dist_brightness_id", "m_dist_brightness_name", NormalisableRange<float>(40.0, 20000.0, 1.0, 1.0), 8192.0f)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_dist_wet_level_id", "m_dist_wet_level_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));
    parameters.add((std::make_unique<AudioParameterFloat>("m_dist_dry_level_id", "m_dist_dry_level_name", NormalisableRange<float>(0.0, 1.0, 0.05), 0.4)));


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
        std::string dist_button_id = "m_dist_button_id_";
        dist_button_id.append(to_string(i));
        std::string dist_button_name = "m_dist_button_name_";
        dist_button_name.append(to_string(i));
        parameters.add((std::make_unique<AudioParameterBool>(dist_button_id, dist_button_name, false)));
    }

    return parameters;
}

void ExodusAudioProcessor::promoteInstence()
{
    current_instence++;
    current_instence %= NUM_OF_INSTENCES;
}

void ExodusAudioProcessor::setSize(int new_num_channels, int new_num_samples)
{
    wet_delay_buffer.setSize(new_num_channels, new_num_samples);
    dry_delay_buffer.setSize(new_num_channels, new_num_samples);
    delay_buffer_length = wet_delay_buffer.getNumSamples();
}

void ExodusAudioProcessor::setParameters(const Parameters& new_params)
{
    parameters.delay_time = new_params.delay_time;
    parameters.delay_feedback = new_params.delay_feedback;
    parameters.delay_mix = new_params.delay_mix;
}

void ExodusAudioProcessor::setSampleRate(double new_sample_rate)
{
    sample_rate = new_sample_rate;
}

void ExodusAudioProcessor::applyReverb()
{
    dsp::AudioBlock<float> audio_block{ wet_delay_buffer };
    juce::dsp::ProcessContextReplacing<float> ctx(audio_block);
    reverb.process(ctx);
}

void ExodusAudioProcessor::applyDist(int channel)
{
    distortion.process(wet_delay_buffer, channel);
}


void ExodusAudioProcessor::fillDelayBuffers(int channel, const int buffer_length, const float* read_pointer, int buffer_write_position)
{
    float* smoothed_read_pointer;

    //smoothed_read_pointer->setCurrentAndTargetValue(*read_pointer);
    if (delay_buffer_length > buffer_length + buffer_write_position)
    {
        dry_delay_buffer.copyFromWithRamp(channel, buffer_write_position, smoothed_read_pointer, buffer_length, parameters.delay_feedback, parameters.delay_feedback);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - buffer_write_position;
        dry_delay_buffer.copyFromWithRamp(channel, buffer_write_position, smoothed_read_pointer, buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
        dry_delay_buffer.copyFromWithRamp(channel, 0, smoothed_read_pointer, (buffer_length - buffer_remaining), parameters.delay_feedback, parameters.delay_feedback);
    }
    wet_delay_buffer.copyFromWithRamp(channel, 0, dry_delay_buffer.getReadPointer(channel), delay_buffer_length, 1, 1);
}


void ExodusAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length, int buffer_write_position)
{
    const int read_position = static_cast<int> (delay_buffer_length + buffer_write_position - (sample_rate * parameters.delay_time / 1000)) % delay_buffer_length;
    const float* wet_delay_buffer_data = wet_delay_buffer.getReadPointer(channel);
    parameters.app_delay_pan = calculatePanMargin(channel);
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, buffer.getReadPointer(channel), buffer_length, (parameters.app_delay_pan * powf((parameters.delay_mix / 100), 1.5)));
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - read_position;
        //buffer.copyFromWithRamp(channel, 0, wet_delay_buffer_data + read_position, buffer_remaining, parameters.app_delay_pan * powf((parameters.delay_mix / 100), 1.5));
        buffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer_remaining, parameters.app_delay_pan * powf((parameters.delay_mix / 100), 1.5));
        buffer.copyFrom(channel, buffer_remaining, wet_delay_buffer_data, buffer_length - buffer_remaining, parameters.app_delay_pan * powf((parameters.delay_mix / 100), 1.5));
    }
}


void ExodusAudioProcessor::feedbackDelay(int channel, const int buffer_length, float* write_pointer, int buffer_write_position)
{
    if (delay_buffer_length > buffer_length + buffer_write_position)
    {
        dry_delay_buffer.addFromWithRamp(channel, buffer_write_position, write_pointer, buffer_length, parameters.delay_feedback, parameters.delay_feedback);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - buffer_write_position;
        dry_delay_buffer.addFromWithRamp(channel, buffer_remaining, write_pointer, buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
        dry_delay_buffer.addFromWithRamp(channel, 0, write_pointer, buffer_length - buffer_remaining, parameters.delay_feedback, parameters.delay_feedback);
    }
}


float ExodusAudioProcessor::calculatePanMargin(int channel)
{
    if (channel == 0)
    {
        if (parameters.delay_pan < 0)
        {
            return 1;
        }
        else if (parameters.delay_pan > 0)
        {
            return 1 - square(parameters.delay_pan);
        }
    }
    else if (channel == 1)
    {
        if (parameters.delay_pan > 0)
        {
            return 1;
        }
        else if (parameters.delay_pan < 0)
        {
            return 1 - square(parameters.delay_pan);
        }
    }
    return 1;
}


void ExodusAudioProcessor::addOnOffMarked(int instence)
{
    d_on_off[instence] = true;
    on_off_marked++;
}
void ExodusAudioProcessor::subOnOffMarked(int instence)
{
    d_on_off[instence] = false;
    on_off_marked--;
    d_reverb[instence] = false;
}

bool ExodusAudioProcessor::addReverbMarked(int instence)
{
    //if (d_on_off[instence] == false)
    //{
    //    return false;
    //}

    d_reverb[instence] = true;
    reverb_marked++;
    return true;
}
bool ExodusAudioProcessor::subReverbMarked(int instence)
{
    d_reverb[instence] = false;
    reverb_marked--;
    return true;
}

bool ExodusAudioProcessor::addDistMarked(int instence)
{
    //if (d_on_off[instence] == false)
    //{
    //    return false;
    //}

    d_dist[instence] = true;
    dist_marked++;
    return true;
}
bool ExodusAudioProcessor::subDistMarked(int instence)
{
    d_dist[instence] = false;
    dist_marked--;
    return true;
}

//==============================================================================
const juce::String ExodusAudioProcessor::getName() const
=======
#include "Distortion.h"

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

}

Exodus_2AudioProcessor::~Exodus_2AudioProcessor()
{
}

//==============================================================================
const juce::String Exodus_2AudioProcessor::getName() const
>>>>>>> master
{
    return JucePlugin_Name;
}

<<<<<<< HEAD
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
=======
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
>>>>>>> master
{
    return 0.0;
}

<<<<<<< HEAD
int ExodusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int ExodusAudioProcessor::getCurrentProgram()
=======
int Exodus_2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Exodus_2AudioProcessor::getCurrentProgram()
>>>>>>> master
{
    return 0;
}

<<<<<<< HEAD
void ExodusAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ExodusAudioProcessor::getProgramName(int index)
=======
void Exodus_2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Exodus_2AudioProcessor::getProgramName (int index)
>>>>>>> master
{
    return {};
}

<<<<<<< HEAD
void ExodusAudioProcessor::changeProgramName(int index, const juce::String& newName)
=======
void Exodus_2AudioProcessor::changeProgramName (int index, const juce::String& newName)
>>>>>>> master
{
}

//==============================================================================
<<<<<<< HEAD
void ExodusAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    m_visualiser.clear();
    m_visualiser_2.clear();

    setSize(getNumInputChannels(), (sampleRate * samplesPerBlock) * 4);
    setSampleRate(sampleRate);
    distortion.setSampleRate(sampleRate);

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    reverb.prepare(spec);

}

void ExodusAudioProcessor::releaseResources()
=======
void Exodus_2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	m_delayEngine->prepareToPlay(sampleRate, samplesPerBlock);
}

void Exodus_2AudioProcessor::releaseResources()
>>>>>>> master
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
<<<<<<< HEAD
bool ExodusAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
=======
bool Exodus_2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
>>>>>>> master
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
<<<<<<< HEAD
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
    std::string vol_dial_id = "m_vol_dial_id_";
    vol_dial_id.append(to_string(current_instence));
    std::string pan_dial_id = "m_pan_dial_id_";
    pan_dial_id.append(to_string(current_instence));
    parameters.delay_mix = tree_state.getRawParameterValue("m_delay_mix_id")->load();
    parameters.delay_time = tree_state.getRawParameterValue("m_delay_time_id")->load();
    parameters.delay_feedback = tree_state.getRawParameterValue("m_delay_feedback_id")->load();
    parameters.delay_volume = tree_state.getRawParameterValue(vol_dial_id)->load();
    parameters.delay_pan = tree_state.getRawParameterValue(pan_dial_id)->load();

    setParameters(parameters);
}

void ExodusAudioProcessor::updateReverbSettings()
{
    reverb_params.roomSize = tree_state.getRawParameterValue("m_reverb_room_size_id")->load();
    reverb_params.damping = tree_state.getRawParameterValue("m_reverb_damping_id")->load();
    reverb_params.width = tree_state.getRawParameterValue("m_reverb_width_id")->load();
    reverb_params.wetLevel = tree_state.getRawParameterValue("m_reverb_wet_level_id")->load();
    reverb_params.dryLevel = tree_state.getRawParameterValue("m_reverb_dry_level_id")->load();

    reverb.setParameters(reverb_params);
}

void ExodusAudioProcessor::updateDistortionSettings()
{
    dist_params.dist_drive = tree_state.getRawParameterValue("m_dist_drive_id")->load();
    dist_params.dist_brightness = tree_state.getRawParameterValue("m_dist_brightness_id")->load();
    dist_params.dist_wet = tree_state.getRawParameterValue("m_dist_wet_level_id")->load();
    dist_params.dist_dry = tree_state.getRawParameterValue("m_dist_dry_level_id")->load();

    distortion.setParameters(dist_params);
}

//   ▄███████▄    ▄████████  ▄██████▄   ▄████████    ▄████████    ▄████████    ▄████████  ▄██████▄     ▄████████ 
//  ███    ███   ███    ███ ███    ███ ███    ███   ███    ███   ███    ███   ███    ███ ███    ███   ███    ███ 
//  ███    ███   ███    ███ ███    ███ ███    █▀    ███    █▀    ███    █▀    ███    █▀  ███    ███   ███    ███ 
//  ███    ███  ▄███▄▄▄▄██▀ ███    ███ ███         ▄███▄▄▄       ███          ███        ███    ███  ▄███▄▄▄▄██▀ 
//▀█████████▀  ▀▀███▀▀▀▀▀   ███    ███ ███        ▀▀███▀▀▀     ▀███████████ ▀███████████ ███    ███ ▀▀███▀▀▀▀▀   
//  ███        ▀███████████ ███    ███ ███    █▄    ███    █▄           ███          ███ ███    ███ ▀███████████ 
//  ███          ███    ███ ███    ███ ███    ███   ███    ███    ▄█    ███    ▄█    ███ ███    ███   ███    ███ 
// ▄████▀        ███    ███  ▀██████▀  ████████▀    ██████████  ▄████████▀   ▄████████▀   ▀██████▀    ███    ███ 
//               ███    ███                                                                           ███    ███ 


void ExodusAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    updateDelaySettings();
    updateReverbSettings();
    updateDistortionSettings();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    int buffer_length = buffer.getNumSamples();
    distortion.setSize(getNumInputChannels(), delay_buffer_length);

    m_visualiser.pushBuffer(buffer);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(tree_state.getRawParameterValue("m_input_gain_id")->load());
        }

        float* write_pointer = buffer.getWritePointer(channel);
       const  float* read_pointer = buffer.getReadPointer(channel);

        fillDelayBuffers(channel, buffer_length, read_pointer, processor_buffer_write_pos); //now also copy to wet

        if (d_on_off[current_instence] == true)
        {
            if ((d_reverb[current_instence] == true) && (channel == 0))
            {
                applyReverb(); //check if reverb processing changes pointers....
            }
            if (d_dist[current_instence] == true)
            {
                distortion.process(wet_delay_buffer, channel);
            }
        }
        getFromDelayBuffer(buffer, channel, buffer_length, getNumSamples(), processor_buffer_write_pos);
        feedbackDelay(channel, buffer_length, write_pointer, processor_buffer_write_pos);
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(tree_state.getRawParameterValue("m_output_gain_id")->load()) * parameters.delay_volume;
        }
    }

    m_visualiser_2.pushBuffer(buffer);

    processor_buffer_write_pos += buffer_length;
    processor_buffer_write_pos %= getNumSamples();
}



//==============================================================================
bool ExodusAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ExodusAudioProcessor::createEditor()
{
    return new ExodusAudioProcessorEditor(*this);
}

//==============================================================================
void ExodusAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
=======
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
    Distortion distortion;
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
        apvts.getRawParameterValue("GNRL_DELAY_TIME")->load(),
        apvts.getRawParameterValue("GNRL_FEEDBACK")->load(),
        apvts.getRawParameterValue("GNRL_DRY_LEVEL")->load(),
        apvts.getRawParameterValue("GNRL_WET_LEVEL")->load() });

    m_delayEngine->setDelayLineSettings(m_index, {
        apvts.getRawParameterValue("GAIN_" + std::to_string(m_index))->load(),
        apvts.getRawParameterValue("PAN_" + std::to_string(m_index))->load(),
        apvts.getRawParameterValue("DISTORTION_MIX_" + std::to_string(m_index))->load(),
        apvts.getRawParameterValue("REVERB_MIX_" + std::to_string(m_index))->load(),
        apvts.getRawParameterValue("PHASER_MIX_" + std::to_string(m_index))->load()});

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	int numSamples = buffer.getNumSamples();
	this->m_delayTimeInMs = m_delayEngine->getDelayEngineParameters().delayTimeInSec * 1000.0f;

	float maxSample = 0.0f;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, numSamples);
    }

	//test__Distortion(buffer); // Test Distortion effect

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		m_delayEngine->fillDelayBuffer(channel, numSamples, buffer.getReadPointer(channel));
		m_delayEngine->fillFromDelayBuffer(channel, buffer, numSamples, m_index);
		m_delayEngine->feedbackDelay(channel, numSamples);
	}

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
>>>>>>> master
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

<<<<<<< HEAD
void ExodusAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
=======
void Exodus_2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
>>>>>>> master
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
<<<<<<< HEAD
    return new ExodusAudioProcessor();
}
=======
    return new Exodus_2AudioProcessor();
}


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



    return layout;
}
>>>>>>> master
