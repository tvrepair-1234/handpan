#include "ScalePresetManager.h"

ScalePresetManager::ScalePresetManager()
{
    presets = {
        { "D Kurd", 50, { 0, 3, 5, 7, 10, 12, 15, 17, 19 } },
        { "Celtic", 50, { 0, 2, 5, 7, 9, 12, 14, 17, 19 } },
        { "Integral", 50, { 0, 3, 5, 7, 9, 12, 15, 17, 21 } },
        { "Hijaz", 50, { 0, 1, 4, 5, 7, 8, 12, 13, 16 } }
    };
}

const ScalePreset& ScalePresetManager::getActivePreset() const
{
    jassert(! presets.empty());
    return presets[static_cast<size_t>(activePresetIndex)];
}

bool ScalePresetManager::setActivePresetIndex(const int index)
{
    if (index < 0 || index >= static_cast<int>(presets.size()))
        return false;

    activePresetIndex = index;
    return true;
}
