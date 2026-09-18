#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"
#include <Windows.h>

namespace Triggerbot {
    struct Settings {
        bool enabled = false;
        int key = 'Q';
        int activationMode = 0;
        bool teamCheck = true;
        bool visibleCheck = true;
        float hitboxMul = 1.0f;
        int delayMs = 1;
        int releaseMs = 10;
        float maxDistance = 300.f;
    };

    inline Settings settings;

    void Update(Memory& mem);
}
