#pragma once

#include <JuceHeader.h>

class NoteMapping
{
public:
    struct Selection
    {
        int midiNote = -1;
        int velocityLayer = 1;
        int rrIndex = 1;
    };

    Selection select(int midiNote, float velocity, int rrCount);

private:
    std::map<int, int> rrCursorByNote;
};
