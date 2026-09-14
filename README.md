# UberDelivery

Advanced Roblox External

## Features

### Combat
- **Aimbot** - Sensitivity, Smoothing (Linear / Ease / Humanized), Prediction
- **Silent Aim** - Sensitivity, Prediction
- **Triggerbot** - Reaction time (ms), Anti-double-shot lock

### Visuals
- ESP: 2D Box, 3D Box, Corner Box
- Team Check
- Name
- Distance
- Skeleton

### Stealth
- True Streamproof (OBS / NVIDIA / AMD)
- Echo.ac & Detect.ac resistance layer
- Self-Destruct (full trace wipe)
- Boot persistence + config auto-load (phase 2)

## Build

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Structure

```
src/
  main.cpp
  memory/
  sdk/
  features/
  render/
  menu/
  stealth/
```

Offsets are placeholders. Dump live offsets before use.
