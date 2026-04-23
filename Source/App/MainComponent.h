#pragma once

#include <JuceHeader.h>
#include "Audio/HandpanEngine.h"
#include "Presets/ScalePresetManager.h"
#include "UI/HandpanPadComponent.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::MidiInputCallback,
                      public juce::KeyListener,
                      private HandpanPadComponent::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

private:
    void handpanPadTriggered(int midiNote, float velocity) override;
    void updateScale();
    void openSampleFolder();
    void refreshMidiDevices();

    HandpanEngine engine;
    ScalePresetManager presetManager;

    HandpanPadComponent handpanPad;

    juce::Slider volumeSlider;
    juce::Slider reverbSlider;
    juce::Slider toneSlider;
    juce::ComboBox scaleCombo;
    juce::Label midiStatus;
    juce::TextButton loadSamplesButton { "Load Samples" };

    juce::CriticalSection midiLock;
    juce::MidiBuffer liveMidi;
    std::unique_ptr<juce::MidiInput> midiInput;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
