#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"

namespace Aimbot {
    struct Settings {
        bool enabled = false;
        float sensitivity = 1.0f;
        float smoothing = 5.0f;
        int smoothType = 0; // 0 = linear, 1 = ease, 2 = humanized
        bool prediction = true;
        float predictionScale = 1.0f;
        float fov = 120.0f;
        int key = VK_RBUTTON;
        bool teamCheck = true;
        bool visibleCheck = false;
    };

    inline Settings settings;

    void Update(Memory& mem);
    Vector3 Predict(const Vector3& pos, const Vector3& vel, float pingMs);
}
