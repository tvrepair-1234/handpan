#include "HandpanVoice.h"

void HandpanVoice::start(const SampleManager::SampleData* sampleToUse,
                         const int midiNoteNumber,
                         const float velocity,
                         const double outputSampleRate,
                         const float randomPitchAmount)
{
    sample = sampleToUse;
    midiNote = midiNoteNumber;
    gain = velocity;
    readPosition = 0.0;

    envelope.setParameters(envelopeParams);
    envelope.setSampleRate(outputSampleRate);
    envelope.noteOn();

    const auto randomSemitone = juce::Random::getSystemRandom().nextFloat() * randomPitchAmount - (randomPitchAmount * 0.5f);
    const auto ratio = std::pow(2.0, randomSemitone / 12.0);
    step = (sample->sourceSampleRate / outputSampleRate) * ratio;

    active = (sample != nullptr);
}

void HandpanVoice::render(juce::AudioBuffer<float>& output, const int startSample, const int numSamples)
{
    if (! active || sample == nullptr)
        return;

    const auto& source = sample->buffer;
    const int sourceNumSamples = source.getNumSamples();
    const int sourceNumChannels = source.getNumChannels();

    for (int i = 0; i < numSamples; ++i)
    {
        const int srcPos = static_cast<int>(readPosition);

        if (srcPos >= sourceNumSamples - 1)
        {
            envelope.noteOff();
            if (! envelope.isActive())
            {
                active = false;
                break;
            }
        }

        float sampleValue = 0.0f;
        for (int ch = 0; ch < sourceNumChannels; ++ch)
            sampleValue += source.getSample(ch, juce::jlimit(0, sourceNumSamples - 1, srcPos));

        sampleValue /= static_cast<float>(juce::jmax(1, sourceNumChannels));
        sampleValue *= envelope.getNextSample() * gain;

        for (int outCh = 0; outCh < output.getNumChannels(); ++outCh)
            output.addSample(outCh, startSample + i, sampleValue);

        readPosition += step;
    }

    if (! envelope.isActive())
        active = false;
}
