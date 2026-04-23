#pragma once

#include <JuceHeader.h>

struct ScalePreset
{
    juce::String name;
    int rootMidiNote = 50;
    std::vector<int> intervals;

    std::vector<int> toMidiNotes() const
    {
        std::vector<int> notes;
        notes.reserve(intervals.size());
        for (const auto interval : intervals)
            notes.push_back(rootMidiNote + interval);
        return notes;
    }
};

class ScalePresetManager
{
public:
    ScalePresetManager();

    const std::vector<ScalePreset>& getPresets() const { return presets; }
    const ScalePreset& getActivePreset() const;
    int getActivePresetIndex() const { return activePresetIndex; }
    bool setActivePresetIndex(int index);

private:
    std::vector<ScalePreset> presets;
    int activePresetIndex = 0;
};
