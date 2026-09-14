# UberDelivery

Advanced Roblox External

## Features

### Combat
- **Aimbot** — Sensitivity, Smoothing (Linear / Ease / Humanized / Snap), Prediction, multi-hitbox
- **Silent Aim** — Sensitivity, Prediction, FOV, hitbox select
- **Triggerbot** — Reaction time (ms), Anti-double-shot lock (single-bullet weapons)

### Visuals
- ESP: 2D Box, 3D Box, Corner Box
- Team Check, Name, Distance, Skeleton, Health bar

### Stealth
- True Streamproof (`WDA_EXCLUDEFROMCAPTURE` — OBS / Discord / Game Bar)
- Self-Destruct (trace wipe + process exit)
- Echo.ac / Detect.ac resistance (usermode external, no inject)

## Build

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Offsets

Live offsets are in `src/sdk/offsets.h`.

Update path:
1. Use [RbxDumperV2](https://git.imtheo.lol/theo/RbxDumperV2)
2. Paste new values into `offsets.h`
3. Rebuild

See `tools/README_DUMPER.md`.

## Controls (default)

| Key | Action |
|-----|--------|
| INSERT | Toggle menu |
| END | Self-destruct |
| RMB | Aimbot hold |
| Configurable | Silent / Trigger |

## Structure

```
src/
  main.cpp
  memory/
  sdk/          offsets + structures
  features/     aimbot, silentaim, triggerbot, esp
  render/       streamproof overlay
  menu/
  stealth/      streamproof + selfdestruct
tools/
  README_DUMPER.md
```

## Status

- Skeleton + offsets + feature shells: **done**
- Entity list / WorldToScreen / full draw: **next**
- Silent aim method for your specific game: **needs target experience details**
