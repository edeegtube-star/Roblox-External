#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"

namespace SilentAim {
    struct Settings {
        bool enabled = false;
        float sensitivity = 1.0f;
        bool prediction = true;
        float predictionScale = 1.0f;
        float fov = 80.0f;
        bool teamCheck = true;
        int hitPart = 0; // 0 = head, 1 = torso
    };

    inline Settings settings;

    void Update(Memory& mem);
}
