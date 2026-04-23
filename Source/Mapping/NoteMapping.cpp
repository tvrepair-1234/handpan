#include "NoteMapping.h"

NoteMapping::Selection NoteMapping::select(const int midiNote, const float velocity, const int rrCount)
{
    Selection selection;
    selection.midiNote = midiNote;

    if (velocity < 0.4f)
        selection.velocityLayer = 1;
    else if (velocity < 0.75f)
        selection.velocityLayer = 2;
    else
        selection.velocityLayer = 3;

    auto& cursor = rrCursorByNote[midiNote];
    if (rrCount <= 0)
    {
        selection.rrIndex = 1;
    }
    else
    {
        cursor = (cursor % rrCount) + 1;
        selection.rrIndex = cursor;
    }

    return selection;
}
