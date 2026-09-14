#pragma once
#include "../memory/memory.h"

namespace Triggerbot {
    struct Settings {
        bool enabled = false;
        int reactionMs = 40;
        bool antiDoubleShot = true;
        int key = VK_XBUTTON1;
        bool teamCheck = true;
    };

    inline Settings settings;

    void Update(Memory& mem);
}
