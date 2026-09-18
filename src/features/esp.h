#pragma once
#include "../memory/memory.h"
#include "../sdk/structures.h"
#include <vector>

namespace ESP {
    struct Settings {
        bool enabled = true;
        bool boxes = true;
        int boxType = 0;
        bool boxOutline = true;
        bool boxFill = false;
        float boxColour[4] = { 1.f, 1.f, 1.f, 1.f };
        float boxOutlineColour[4] = { 0.f, 0.f, 0.f, 1.f };
        float boxFillColour[4] = { 1.f, 1.f, 1.f, 0.15f };
        bool healthbar = true;
        float healthbarColour[4] = { 0.f, 1.f, 0.f, 1.f };
        float healthbarBg[4] = { 0.f, 0.f, 0.f, 0.6f };
        bool skeleton = false;
        float skeletonColour[4] = { 1.f, 1.f, 1.f, 1.f };
        float skeletonOutline[4] = { 0.f, 0.f, 0.f, 1.f };
        bool name = true;
        int nameType = 0;
        float nameColour[4] = { 1.f, 1.f, 1.f, 1.f };
        bool distance = true;
        float distanceColour[4] = { 0.8f, 0.8f, 0.8f, 1.f };
        bool tool = false;
        float toolColour[4] = { 1.f, 0.85f, 0.2f, 1.f };
        bool tracers = false;
        int tracerOrigin = 1;
        float tracersColour[4] = { 1.f, 1.f, 1.f, 0.7f };
        bool snaplines = false;
        float snaplinesColour[4] = { 1.f, 0.3f, 0.3f, 0.8f };
        bool chams = false;
        int chamsType = 1;
        float chamsColour[4] = { 0.2f, 0.6f, 1.f, 0.35f };
        float chamsOutline[4] = { 0.f, 0.f, 0.f, 0.8f };
        bool teamCheck = true;
        bool knockCheck = false;
        bool healthCheck = false;
        float minHealth = 1.f;
        float maxDistance = 1000.f;
    };

    inline Settings settings;
    inline std::vector<PlayerData> players;

    void Update(Memory& mem);
    void Render();
}
