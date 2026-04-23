# Handpan Desktop App (JUCE MVP)

This repository contains an MVP for a premium-style digital handpan desktop app built with **C++17 + JUCE**.

## MVP highlights
- Standalone desktop architecture that can be migrated into a plugin processor/editor split.
- Sample-based engine with:
  - multi-note folder scanning
  - velocity layers (`velX`)
  - round robin (`rrX`)
- Polyphonic overlapping voices with natural decays.
- MIDI input support + computer keyboard triggering + mouse pad triggering.
- Circular handpan-inspired UI with visual strike feedback.
- Master volume, reverb amount, tone shaping.
- Scale preset switching: D Kurd, Celtic, Integral, Hijaz.

## Expected sample layout

```text
Samples/
  D_Kurd/
    D3/
      vel1_rr1.wav
      vel1_rr2.wav
      vel2_rr1.wav
    F3/
      vel1_rr1.wav
```

Note folders are parsed by musical note name (`D3`, `F#3`, etc.).

## Build

```bash
cmake -S . -B build
cmake --build build -j
```

The first configure downloads JUCE through `FetchContent`.
