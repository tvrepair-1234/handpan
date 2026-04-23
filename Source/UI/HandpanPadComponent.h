#pragma once

#include <JuceHeader.h>

class HandpanPadComponent : public juce::Component,
                            private juce::Timer
{
public:
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void handpanPadTriggered(int midiNote, float velocity) = 0;
    };

    HandpanPadComponent();

    void setNotes(const std::vector<int>& midiNotes);
    void setListener(Listener* newListener) { listener = newListener; }

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;

private:
    struct Zone
    {
        int midiNote = 60;
        juce::Point<float> center;
        float radius = 30.0f;
        float glow = 0.0f;
    };

    void resized() override;
    void timerCallback() override;
    void triggerZone(Zone& zone, float velocity);

    std::vector<Zone> zones;
    Listener* listener = nullptr;
};
