#include "HandpanPadComponent.h"

HandpanPadComponent::HandpanPadComponent()
{
    startTimerHz(30);
}

void HandpanPadComponent::setNotes(const std::vector<int>& midiNotes)
{
    zones.clear();
    for (const auto note : midiNotes)
        zones.push_back({ note, {}, 34.0f, 0.0f });

    resized();
    repaint();
}

void HandpanPadComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0c0f16));

    const auto area = getLocalBounds().toFloat().reduced(10.0f);
    g.setColour(juce::Colour(0xff111826));
    g.fillEllipse(area);

    for (const auto& zone : zones)
    {
        const auto glowColour = juce::Colour(0xff66d9ef).withAlpha(0.2f + zone.glow * 0.35f);
        g.setColour(glowColour);
        g.fillEllipse(zone.center.x - zone.radius - 8.0f, zone.center.y - zone.radius - 8.0f,
                      (zone.radius + 8.0f) * 2.0f, (zone.radius + 8.0f) * 2.0f);

        g.setColour(juce::Colour(0xff1d2636));
        g.fillEllipse(zone.center.x - zone.radius, zone.center.y - zone.radius, zone.radius * 2.0f, zone.radius * 2.0f);

        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
        g.drawFittedText(juce::MidiMessage::getMidiNoteName(zone.midiNote, true, true, 3),
                         juce::Rectangle<int>::leftTopRightBottom(
                             static_cast<int>(zone.center.x - zone.radius),
                             static_cast<int>(zone.center.y - 10.0f),
                             static_cast<int>(zone.center.x + zone.radius),
                             static_cast<int>(zone.center.y + 10.0f)),
                         juce::Justification::centred, 1);
    }
}

void HandpanPadComponent::mouseDown(const juce::MouseEvent& e)
{
    for (auto& zone : zones)
    {
        const auto dist = zone.center.getDistanceFrom(e.position);
        if (dist <= zone.radius)
        {
            const auto vel = juce::jlimit(0.2f, 1.0f, 1.0f - dist / zone.radius);
            triggerZone(zone, vel);
            return;
        }
    }
}

void HandpanPadComponent::resized()
{
    if (zones.empty())
        return;

    const auto area = getLocalBounds().toFloat().reduced(20.0f);
    const auto center = area.getCentre();
    const int total = static_cast<int>(zones.size());

    zones[0].center = center;
    zones[0].radius = 44.0f;

    const float ringRadius = juce::jmin(area.getWidth(), area.getHeight()) * 0.34f;

    for (int i = 1; i < total; ++i)
    {
        const float angle = juce::MathConstants<float>::twoPi * (static_cast<float>(i - 1) / static_cast<float>(total - 1))
                          - juce::MathConstants<float>::halfPi;

        zones[i].center = center + juce::Point<float>(std::cos(angle) * ringRadius, std::sin(angle) * ringRadius);
        zones[i].radius = 34.0f;
    }
}

void HandpanPadComponent::timerCallback()
{
    bool needsRepaint = false;
    for (auto& zone : zones)
    {
        if (zone.glow > 0.001f)
        {
            zone.glow *= 0.88f;
            needsRepaint = true;
        }
    }

    if (needsRepaint)
        repaint();
}

void HandpanPadComponent::triggerZone(Zone& zone, const float velocity)
{
    zone.glow = 1.0f;
    if (listener != nullptr)
        listener->handpanPadTriggered(zone.midiNote, velocity);

    repaint();
}
