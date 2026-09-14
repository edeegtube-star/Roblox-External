#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"
#include <vector>

namespace ESP {
    struct Settings {
        bool enabled = true;
        bool box2D = true;
        bool box3D = false;
        bool boxCorner = false;
        bool name = true;
        bool distance = true;
        bool skeleton = false;
        bool teamCheck = true;
        bool healthBar = true;
        float maxDistance = 1000.0f;
    };

    inline Settings settings;
    inline std::vector<PlayerData> players;

    void Update(Memory& mem);
    void Render();
}
