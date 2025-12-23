/*
  ==============================================================================

    Phaser.cpp
    Created: 4 Dec 2025 3:35:56pm
    Author:  MayTigerStation

  ==============================================================================
*/

#include "MyPhaser.h"
#include <JuceHeader.h>

//==============================================================================
MyPhaser::MyPhaser()
{
    auto oscFunction = [](float x) { return std::sin(x); };
    osc.initialise(oscFunction);

    for (auto n = 0; n < numStages; ++n)
    {
        filters.add(new juce::dsp::FirstOrderTPTFilter<float>());
        filters[n]->setType(juce::dsp::FirstOrderTPTFilterType::allpass);
    }

    dryWet.setMixingRule(juce::dsp::DryWetMixingRule::linear);
}


void MyPhaser::setRate(float newRateHz)
{
    jassert(juce::isPositiveAndBelow(newRateHz, static_cast<float> (100.0)));

    rate = newRateHz;
    update();
}


void MyPhaser::setDepth(float newDepth)
{
    jassert(juce::isPositiveAndNotGreaterThan(newDepth, static_cast<float> (1.0)));

    depth = newDepth;
    update();
}

void MyPhaser::setLFOType(int newLFOType)
{
    jassert(newLFOType >= 0 && newLFOType <= phaserLFOType_numberOfTypes);

    if (newLFOType == phaser_LFOType)
		return;

	phaser_LFOType = phaserLFOType_numberOfTypes - 1 - newLFOType; // Invert the LFO type to match the UI order

    switch (phaser_LFOType)
    {
        case phaserLFOType_Sine:
        {
            auto oscFunction = [](float x) { return std::sin(x); };
            osc.initialise(oscFunction);
            break;
		}
        case phaserLFOType_Triangle:
        {
            auto oscFunction = [](float x) { return juce::jmap(juce::jlimit(-1.0f, 1.0f, (2.0f / juce::MathConstants<float>::pi) * std::asin(std::sin(x))), -1.0f, 1.0f, -1.0f, 1.0f); };
            osc.initialise(oscFunction);
            break;
		}
        case phaserLFOType_SawUp:
        {
			auto oscFunction = [](float x) { return juce::jmap(juce::jlimit(-1.0f, 1.0f, (2.0f / juce::MathConstants<float>::pi) * (x - juce::MathConstants<float>::pi * std::floor((x / juce::MathConstants<float>::pi + 1.0f) * 0.5f + 0.5f))), -1.0f, 1.0f, -1.0f, 1.0f); };
            osc.initialise(oscFunction);
            break;
        }
        case phaserLFOType_SawDown:
        {
            auto oscFunction = [](float x) { return juce::jmap(juce::jlimit(-1.0f, 1.0f, (2.0f / juce::MathConstants<float>::pi) * (juce::MathConstants<float>::pi - (x - juce::MathConstants<float>::pi * std::floor((x / juce::MathConstants<float>::pi + 1.0f) * 0.5f + 0.5f)))), -1.0f, 1.0f, -1.0f, 1.0f); };
			osc.initialise(oscFunction);
			break;
        }
        case phaserLFOType_Square:
        {
            auto oscFunction = [](float x) { return (std::sin(x) >= 0.0f) ? 1.0f : -1.0f; };
            osc.initialise(oscFunction);
			break;
		}
        default:
        {
            auto oscFunction = [](float x) { return std::sin(x); };
            osc.initialise(oscFunction);
            break;
        }
    }

	update();
}


void MyPhaser::setCentreFrequency(float newCentreHz)
{
    //newCentreHz = juce::jmap(newCentreHz, static_cast<float> (0.0), static_cast<float> (1.0), static_cast<float> (600.0), static_cast<float> (6000.0));
    jassert(juce::isPositiveAndBelow(newCentreHz, static_cast<float> (sampleRate * 0.5)));

    centreFrequency = newCentreHz;
    normCentreFrequency = juce::mapFromLog10(centreFrequency, static_cast<float> (20.0), static_cast<float> (juce::jmin(20000.0, 0.49 * sampleRate)));
}


void MyPhaser::setFeedback(float newFeedback)
{
    jassert(newFeedback >= static_cast<float> (-1.0) && newFeedback <= static_cast<float> (1.0));

    feedback = newFeedback;
    update();
}


void MyPhaser::setMix(float newMix)
{
    jassert(juce::isPositiveAndNotGreaterThan(newMix, static_cast<float> (1.0)));

    mix = newMix;
    update();
}

//==============================================================================

void MyPhaser::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    jassert(spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    for (auto n = 0; n < numStages; ++n)
        filters[n]->prepare(spec);

    dryWet.prepare(spec);
    feedbackVolume.resize(spec.numChannels);
    lastOutput.resize(spec.numChannels);

    auto specDown = spec;
    specDown.sampleRate /= (double)my_maxUpdateCounter;
    specDown.maximumBlockSize = (specDown.maximumBlockSize / (juce::uint32)my_maxUpdateCounter + 1);

    osc.prepare(specDown);
    bufferFrequency.setSize(1, (int)specDown.maximumBlockSize, false, false, true);

    update();
    reset();
}


void MyPhaser::reset()
{
    std::fill(lastOutput.begin(), lastOutput.end(), static_cast<float> (0));

    for (auto n = 0; n < numStages; ++n)
        filters[n]->reset();

    osc.reset();
    dryWet.reset();

    oscVolume.reset(sampleRate / (double)my_maxUpdateCounter, 0.05);

    for (auto& vol : feedbackVolume)
        vol.reset(sampleRate, 0.05);

    updateCounter = 0;
}


void MyPhaser::update()
{
    osc.setFrequency(rate);
    oscVolume.setTargetValue(depth * (float)0.5);
    dryWet.setWetMixProportion(mix);

    for (auto& vol : feedbackVolume)
        vol.setTargetValue(feedback);
}


void MyPhaser::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    jassert(inputBlock.getNumChannels() == numChannels);
    jassert(inputBlock.getNumChannels() == lastOutput.size());
    jassert(inputBlock.getNumSamples() == numSamples);

    if (context.isBypassed)
    {
        outputBlock.copyFrom(inputBlock);
        return;
    }

    int numSamplesDown = 0;
    auto counter = updateCounter;

    for (size_t i = 0; i < numSamples; ++i)
    {
        if (counter == 0)
            numSamplesDown++;

        counter++;

        if (counter == my_maxUpdateCounter)
            counter = 0;
    }

    if (numSamplesDown > 0)
    {
        auto freqBlock = juce::dsp::AudioBlock<float>(bufferFrequency).getSubBlock(0, (size_t)numSamplesDown);
        auto contextFreq = juce::dsp::ProcessContextReplacing<float>(freqBlock);
        freqBlock.clear();

        osc.process(contextFreq);
        freqBlock.multiplyBy(oscVolume);
    }

    auto* freqSamples = bufferFrequency.getWritePointer(0);

    for (int i = 0; i < numSamplesDown; ++i)
    {
        auto lfo = juce::jlimit(static_cast<float>(0.0),
            static_cast<float>(1.0),
            freqSamples[i] + normCentreFrequency);

        freqSamples[i] = juce::mapToLog10(lfo, static_cast<float>(20.0),
            static_cast<float>(juce::jmin(20000.0, 0.49 * sampleRate)));
    }

    auto currentFrequency = filters[0]->getCutoffFrequency();
    dryWet.pushDrySamples(inputBlock);

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        counter = updateCounter;
        int k = 0;

        auto* inputSamples = inputBlock.getChannelPointer(channel);
        auto* outputSamples = outputBlock.getChannelPointer(channel);

        for (size_t i = 0; i < numSamples; ++i)
        {
            auto input = inputSamples[i];
            auto output = input - lastOutput[channel];

            if (i == 0 && counter != 0)
                for (int n = 0; n < numStages; ++n)
                    filters[n]->setCutoffFrequency(currentFrequency);

            if (counter == 0)
            {
                for (int n = 0; n < numStages; ++n)
                    filters[n]->setCutoffFrequency(freqSamples[k]);

                k++;
            }

            for (int n = 0; n < numStages; ++n)
                output = filters[n]->processSample((int)channel, output);

            outputSamples[i] = output;
            lastOutput[channel] = output * feedbackVolume[channel].getNextValue();

            counter++;

            if (counter == my_maxUpdateCounter)
                counter = 0;
        }
    }

    dryWet.mixWetSamples(outputBlock);
    updateCounter = (updateCounter + (int)numSamples) % my_maxUpdateCounter;
}