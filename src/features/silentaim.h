#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"
#include <Windows.h>

namespace SilentAim {
    struct Settings {
        bool enabled = false;
        float fov = 80.0f;
        int key = 'E';
        int activationMode = 0;
        bool teamCheck = true;
        bool visibleCheck = false;
        bool healthCheck = false;
        float minHealth = 1.f;
        int hitPart = 0;
        bool stickyAim = false;
        bool prediction = true;
        float predictionScale = 1.0f;
        int method = 0;
        bool drawFov = true;
        float fovColour[4] = { 0.2f, 0.8f, 1.f, 0.4f };
        float fovOutline[4] = { 0.f, 0.f, 0.f, 0.7f };
        float maxDistance = 500.f;
    };

    inline Settings settings;
    inline bool toggleState = false;

    void Update(Memory& mem);
}
