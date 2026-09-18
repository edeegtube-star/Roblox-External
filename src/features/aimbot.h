#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"
#include <Windows.h>

namespace Aimbot {
    struct Settings {
        bool enabled = false;
        float sensitivity = 1.0f;
        float smoothing = 5.0f;
        int smoothType = 0;
        bool prediction = true;
        float predictionScale = 1.0f;
        float fov = 120.0f;
        int key = VK_RBUTTON;
        int activationMode = 0;
        bool teamCheck = true;
        bool visibleCheck = false;
        bool healthCheck = false;
        float minHealth = 1.f;
        int hitPart = 0;
        bool stickyAim = false;
        bool drawFov = true;
        float fovColour[4] = { 1.f, 1.f, 1.f, 0.35f };
        float fovOutline[4] = { 0.f, 0.f, 0.f, 0.7f };
        float maxDistance = 2000.f;
        bool resolver = false;
    };
    inline Settings settings;
    inline bool toggleState = false;
    void Update(Memory& mem);
    Vector3 Predict(const Vector3& pos, const Vector3& vel, float scale);
}
