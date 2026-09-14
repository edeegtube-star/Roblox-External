# Offset Dumper Integration

UberDelivery uses [RbxDumperV2](https://git.imtheo.lol/theo/RbxDumperV2) for live offset recovery.

## Quick use

1. Join the dumper place (see RbxDumperV2 README).
2. Run `RbxDumperV2.exe` / `run_dumper.bat`.
3. Copy the generated offsets into `src/sdk/offsets.h`.

## Current static dump (loaded into offsets.h)

Key pointers from latest provided dump:

- TaskScheduler::Pointer = `0x8b5cee8`
- VisualEngine::Pointer  = `0x83f0a08`
- FakeDataModel::Pointer = `0x8dc2258`

Player / Humanoid / Camera / Primitive offsets are already wired in `src/sdk/offsets.h`.

## After every Roblox update

1. Re-run RbxDumperV2
2. Diff against current `offsets.h`
3. Update pointer values + any shifted struct members
4. Rebuild UberDelivery

Dumper source is intentionally kept external (GPL-3.0). Do not vendor the full dumper tree into this repo unless license compliance is handled.
