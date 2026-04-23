#pragma once

#include <JuceHeader.h>
#include "Samples/SampleManager.h"

class HandpanVoice
{
public:
    void start(const SampleManager::SampleData* sampleToUse,
               int midiNoteNumber,
               float velocity,
               double outputSampleRate,
               float randomPitchAmount);

    void render(juce::AudioBuffer<float>& output, int startSample, int numSamples);
    bool isActive() const { return active; }
    int getMidiNote() const { return midiNote; }

private:
    const SampleManager::SampleData* sample = nullptr;
    juce::ADSR envelope;
    juce::ADSR::Parameters envelopeParams { 0.001f, 0.02f, 0.9f, 1.8f };

    bool active = false;
    int midiNote = -1;
    double readPosition = 0.0;
    double step = 1.0;
    float gain = 1.0f;
};
