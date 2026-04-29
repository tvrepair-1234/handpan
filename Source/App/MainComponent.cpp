#include "MainComponent.h"

MainComponent::MainComponent()
{
    setOpaque(true);
    addKeyListener(this);
    setWantsKeyboardFocus(true);

    handpanPad.setListener(this);
    addAndMakeVisible(handpanPad);

    auto setupSlider = [this](juce::Slider& slider, const juce::String& name, double initial)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
        slider.setName(name);
        slider.setRange(0.0, 1.0, 0.001);
        slider.setValue(initial);
        addAndMakeVisible(slider);
    };

    setupSlider(volumeSlider, "Volume", 0.85);
    setupSlider(reverbSlider, "Reverb", 0.25);
    setupSlider(toneSlider, "Tone", 0.55);

    volumeSlider.onValueChange = [this] { engine.setMasterVolume((float) volumeSlider.getValue()); };
    reverbSlider.onValueChange = [this] { engine.setReverbAmount((float) reverbSlider.getValue()); };
    toneSlider.onValueChange = [this] { engine.setTone((float) toneSlider.getValue()); };

    addAndMakeVisible(scaleCombo);
    scaleCombo.onChange = [this]
    {
        presetManager.setActivePresetIndex(scaleCombo.getSelectedItemIndex());
        updateScale();
    };

    int id = 1;
    for (const auto& preset : presetManager.getPresets())
        scaleCombo.addItem(preset.name, id++);

    scaleCombo.setSelectedItemIndex(0);

    addAndMakeVisible(midiStatus);
    midiStatus.setText("MIDI: searching...", juce::dontSendNotification);
    midiStatus.setColour(juce::Label::textColourId, juce::Colours::lightgreen);

    addAndMakeVisible(loadSamplesButton);
    loadSamplesButton.onClick = [this] { openSampleFolder(); };

    updateScale();
    refreshMidiDevices();

    setSize(1100, 760);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    midiInput.reset();
}

void MainComponent::prepareToPlay(const int samplesPerBlockExpected, const double sampleRate)
{
    engine.prepare(sampleRate, samplesPerBlockExpected, 2);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::MidiBuffer midiCopy;
    {
        const juce::ScopedLock lock(midiLock);
        midiCopy.swapWith(liveMidi);
    }

    engine.render(*bufferToFill.buffer, midiCopy);
}

void MainComponent::releaseResources() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff090b10));
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(18);
    auto top = bounds.removeFromTop(120);

    loadSamplesButton.setBounds(top.removeFromLeft(130).reduced(8));
    scaleCombo.setBounds(top.removeFromLeft(180).reduced(8));
    midiStatus.setBounds(top.removeFromLeft(250).reduced(8));

    auto controls = top;
    const int knobW = 120;
    volumeSlider.setBounds(controls.removeFromLeft(knobW).reduced(6));
    reverbSlider.setBounds(controls.removeFromLeft(knobW).reduced(6));
    toneSlider.setBounds(controls.removeFromLeft(knobW).reduced(6));

    handpanPad.setBounds(bounds.reduced(20));
}

void MainComponent::handleIncomingMidiMessage(juce::MidiInput*, const juce::MidiMessage& message)
{
    const juce::ScopedLock lock(midiLock);
    liveMidi.addEvent(message, 0);
}

bool MainComponent::keyPressed(const juce::KeyPress& key, juce::Component*)
{
    static const std::map<juce::juce_wchar, int> keyboardMap {
        { 'a', 0 }, { 's', 1 }, { 'd', 2 }, { 'f', 3 }, { 'g', 4 },
        { 'h', 5 }, { 'j', 6 }, { 'k', 7 }, { 'l', 8 }
    };

    const auto lower = juce::CharacterFunctions::toLowerCase(key.getTextCharacter());
    const auto it = keyboardMap.find(lower);
    if (it == keyboardMap.end())
        return false;

    const auto notes = presetManager.getActivePreset().toMidiNotes();
    if (it->second >= 0 && it->second < static_cast<int>(notes.size()))
        engine.triggerNote(notes[(size_t) it->second], 0.85f);

    return true;
}

void MainComponent::handpanPadTriggered(const int midiNote, const float velocity)
{
    engine.triggerNote(midiNote, velocity);
}

void MainComponent::updateScale()
{
    handpanPad.setNotes(presetManager.getActivePreset().toMidiNotes());
}

void MainComponent::openSampleFolder()
{
    auto chooser = std::make_shared<juce::FileChooser>(
        "Select scale sample folder",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory)
    );

    chooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories,
        [this, chooser](const juce::FileChooser& fc)
        {
            auto folder = fc.getResult();

            if (folder == juce::File{})
                return;

            juce::String errorMessage;
            if (! engine.loadScale(folder, errorMessage))
            {
                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::WarningIcon,
                    "Sample loading failed",
                    errorMessage
                );
            }
        }
    );
}

void MainComponent::refreshMidiDevices()
{
    const auto devices = juce::MidiInput::getAvailableDevices();
    if (devices.isEmpty())
    {
        midiStatus.setText("MIDI: no device", juce::dontSendNotification);
        return;
    }

    auto device = devices[0];
    midiInput = juce::MidiInput::openDevice(device.identifier, this);
    if (midiInput != nullptr)
    {
        midiInput->start();
        midiStatus.setText("MIDI: " + device.name, juce::dontSendNotification);
    }
    else
    {
        midiStatus.setText("MIDI: failed to open", juce::dontSendNotification);
    }
}
