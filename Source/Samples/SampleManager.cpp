#include "SampleManager.h"

namespace
{
int extractNumberAfter(const juce::String& source, const juce::String& key)
{
    const auto idx = source.indexOfIgnoreCase(key);
    if (idx < 0)
        return -1;

    juce::String number;
    for (int i = idx + key.length(); i < source.length(); ++i)
    {
        const auto c = source[i];
        if (juce::CharacterFunctions::isDigit(c))
            number += c;
        else
            break;
    }

    return number.isNotEmpty() ? number.getIntValue() : -1;
}
}

bool SampleManager::loadScaleFolder(const juce::File& folder, juce::String& errorMessage)
{
    samples.clear();
    rrCountByNoteAndLayer.clear();

    formatManager.clearFormats();
    formatManager.registerBasicFormats();

    if (! folder.exists() || ! folder.isDirectory())
    {
        errorMessage = "Sample folder does not exist: " + folder.getFullPathName();
        return false;
    }

    juce::Array<juce::File> noteFolders;
    folder.findChildFiles(noteFolders, juce::File::findDirectories, false);

    for (const auto& noteFolder : noteFolders)
    {
        const int midiNote = parseMidiNoteFromFolderName(noteFolder.getFileName());
        if (midiNote < 0)
            continue;

        juce::Array<juce::File> wavFiles;
        noteFolder.findChildFiles(wavFiles, juce::File::findFiles, false, "*.wav");

        for (const auto& wavFile : wavFiles)
        {
            const auto baseName = wavFile.getFileNameWithoutExtension();
            const int layer = juce::jmax(1, extractNumberAfter(baseName, "vel"));
            const int rr = juce::jmax(1, extractNumberAfter(baseName, "rr"));

            std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(wavFile));
            if (reader == nullptr)
                continue;

            SampleData data;
            data.buffer.setSize(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
            reader->read(&data.buffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
            data.sourceSampleRate = reader->sampleRate;

            samples[{ midiNote, layer, rr }] = std::move(data);
            auto& rrCount = rrCountByNoteAndLayer[{ midiNote, layer }];
            rrCount = juce::jmax(rrCount, rr);
        }
    }

    if (samples.empty())
    {
        errorMessage = "No valid WAV files found under " + folder.getFullPathName();
        return false;
    }

    errorMessage.clear();
    return true;
}

const SampleManager::SampleData* SampleManager::getSample(const NoteMapping::Selection& selection) const
{
    const auto it = samples.find({ selection.midiNote, selection.velocityLayer, selection.rrIndex });
    if (it != samples.end())
        return &it->second;

    const auto fallbackIt = samples.find({ selection.midiNote, selection.velocityLayer, 1 });
    if (fallbackIt != samples.end())
        return &fallbackIt->second;

    return nullptr;
}

int SampleManager::getRoundRobinCount(const int midiNote, const int velocityLayer) const
{
    const auto it = rrCountByNoteAndLayer.find({ midiNote, velocityLayer });
    return it != rrCountByNoteAndLayer.end() ? it->second : 0;
}

int SampleManager::parseMidiNoteFromFolderName(const juce::String& name)
{
    if (name.isEmpty())
        return -1;

    static const juce::String noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

    juce::String noteName;
    noteName += juce::CharacterFunctions::toUpperCase(name[0]);

    int cursor = 1;
    if (name.length() > 1 && name[1] == '#')
    {
        noteName += '#';
        cursor = 2;
    }

    const int octave = name.substring(cursor).getIntValue();

    int noteIndex = -1;
    for (int i = 0; i < 12; ++i)
    {
        if (noteName == noteNames[i])
        {
            noteIndex = i;
            break;
        }
    }

    if (noteIndex < 0)
        return -1;

    return (octave + 1) * 12 + noteIndex;
}
