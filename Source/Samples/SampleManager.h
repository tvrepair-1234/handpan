#pragma once

#include <JuceHeader.h>
#include "Mapping/NoteMapping.h"

class SampleManager
{
public:
    struct SampleData
    {
        juce::AudioBuffer<float> buffer;
        double sourceSampleRate = 44100.0;
    };

    bool loadScaleFolder(const juce::File& folder, juce::String& errorMessage);

    const SampleData* getSample(const NoteMapping::Selection& selection) const;
    int getRoundRobinCount(int midiNote, int velocityLayer) const;

private:
    struct Key
    {
        int midiNote = -1;
        int velocityLayer = 1;
        int rrIndex = 1;

        bool operator<(const Key& other) const
        {
            return std::tie(midiNote, velocityLayer, rrIndex)
                < std::tie(other.midiNote, other.velocityLayer, other.rrIndex);
        }
    };

    static int parseMidiNoteFromFolderName(const juce::String& name);

    juce::AudioFormatManager formatManager;
    std::map<Key, SampleData> samples;
    std::map<std::pair<int, int>, int> rrCountByNoteAndLayer;
};
