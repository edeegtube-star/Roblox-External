#pragma once
#include "../../memory/memory.h"
#include "../../sdk/structures.h"
#include <Windows.h>

namespace Movement {
    struct SpeedSettings {
        bool enabled = false;
        int key = 0;
        int activationMode = 0;
        int mode = 0;
        float speed = 50.f;
    };
    struct FlySettings {
        bool enabled = false;
        int key = 0;
        int activationMode = 0;
        int mode = 0;
        float speed = 50.f;
    };
    struct DesyncSettings {
        bool enabled = false;
        int key = 0;
        bool visualize = true;
        float chamColour[4] = { 1.f, 0.2f, 0.2f, 0.4f };
        float chamOutline[4] = { 1.f, 0.f, 0.f, 0.8f };
        float textColour[4] = { 1.f, 1.f, 1.f, 1.f };
    };
    struct MiscSettings {
        bool noclip = false;
        bool infJump = false;
        bool antiFling = false;
        bool spin360 = false;
        float spinSpeed = 1000.f;
        float gravity = 196.2f;
        bool gravityEnabled = false;
    };
    inline SpeedSettings speed;
    inline FlySettings fly;
    inline DesyncSettings desync;
    inline MiscSettings misc;
    inline bool speedToggle = false;
    inline bool flyToggle = false;
    inline bool desyncActive = false;
    void Update(Memory& mem);
}
