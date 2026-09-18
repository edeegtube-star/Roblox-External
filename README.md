# UberDelivery

Closet Matcha-style Roblox external (Windows 10/11).

## Features

### Combat
- Aimbot — FOV, smoothing (Linear/Ease/Humanized), prediction, sticky, hitpart, team/health checks
- Silent Aim — FOV, prediction, sticky, discrete mouse correction
- Triggerbot — delay/release, hitbox multiplier, team/visible checks

### Visuals
- Box (normal / corner), fill, outline
- Healthbar, skeleton, name (username/display), distance, tool
- Tracers, snaplines, chams
- Aimbot + Silent FOV circles

### Movement
- Speed (Velocity / WalkSpeed)
- Fly
- Desync toggle (game-dependent)
- Noclip, inf jump, anti-fling, 360 spin, custom gravity

### Stealth
- Streamproof (`WDA_EXCLUDEFROMCAPTURE`) — OBS / Discord / Game Bar
- Self-destruct (delayed batch wipe + exit)
- Click-through overlay when menu closed
- Minimal strings / fake product name

## Controls
| Key | Action |
|-----|--------|
| INSERT | Toggle menu |
| END | Self-destruct |
| Configurable | Aimbot / Silent / Trigger / Movement |

## Build
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Offsets
Update `src/sdk/offsets.h` from RbxDumperV2 after every client update.
See `tools/README_DUMPER.md`.

## Notes
- Pure external — no injection into Roblox
- Offsets go stale; keep them current
- Server-sided desync is experience-dependent
