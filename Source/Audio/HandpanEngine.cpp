#include "HandpanEngine.h"

void HandpanEngine::prepare(const double sampleRate, const int blockSize, const int outputChannels)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(blockSize), static_cast<juce::uint32>(outputChannels) };
    reverb.prepare(spec);
    toneFilter.prepare(spec);

    setReverbAmount(0.25f);
    setTone(0.5f);
}

void HandpanEngine::render(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();

    for (const auto metadata : midi)
    {
        const auto message = metadata.getMessage();
        if (message.isNoteOn())
            triggerNote(message.getNoteNumber(), message.getFloatVelocity());
    }

    for (auto& voice : voices)
        voice.render(buffer, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    toneFilter.process(context);
    reverb.process(context);

    buffer.applyGain(masterVolume);
}

void HandpanEngine::triggerNote(const int midiNote, const float velocity)
{
    auto selection = mapping.select(midiNote, velocity, sampleManager.getRoundRobinCount(midiNote, 1));
    selection.rrIndex = juce::jmax(1, selection.rrIndex);

    if (const auto* sample = sampleManager.getSample(selection))
    {
        for (auto& voice : voices)
        {
            if (! voice.isActive())
            {
                voice.start(sample, midiNote, velocity, currentSampleRate, randomPitchAmount);
                return;
            }
        }

        voices.front().start(sample, midiNote, velocity, currentSampleRate, randomPitchAmount);
    }
}

bool HandpanEngine::loadScale(const juce::File& scaleFolder, juce::String& errorMessage)
{
    return sampleManager.loadScaleFolder(scaleFolder, errorMessage);
}

void HandpanEngine::setReverbAmount(const float amount)
{
    juce::dsp::Reverb::Parameters p;
    p.roomSize = 0.82f;
    p.damping = 0.38f;
    p.wetLevel = juce::jlimit(0.0f, 1.0f, amount) * 0.45f;
    p.dryLevel = 1.0f - (amount * 0.15f);
    p.width = 1.0f;
    p.freezeMode = 0.0f;
    reverb.setParameters(p);
}

void HandpanEngine::setTone(const float normalizedTone)
{
    const auto cutoff = juce::jmap(normalizedTone, 0.0f, 1.0f, 900.0f, 12000.0f);
    toneFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, cutoff, 0.707f);
}
