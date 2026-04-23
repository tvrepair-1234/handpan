#pragma once

#include <JuceHeader.h>
#include "Audio/HandpanVoice.h"
#include "Mapping/NoteMapping.h"
#include "Samples/SampleManager.h"

class HandpanEngine
{
public:
    void prepare(double sampleRate, int blockSize, int outputChannels);
    void render(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);

    void triggerNote(int midiNote, float velocity);

    bool loadScale(const juce::File& scaleFolder, juce::String& errorMessage);

    void setMasterVolume(float volume) { masterVolume = volume; }
    void setReverbAmount(float amount);
    void setTone(float normalizedTone);
    void setRandomPitchAmount(float semitones) { randomPitchAmount = semitones; }

private:
    static constexpr int maxVoices = 64;

    std::array<HandpanVoice, maxVoices> voices;
    NoteMapping mapping;
    SampleManager sampleManager;

    juce::dsp::Reverb reverb;
    juce::dsp::IIR::Filter<float> toneFilter;

    float masterVolume = 0.8f;
    float randomPitchAmount = 0.06f;
    double currentSampleRate = 44100.0;
};
