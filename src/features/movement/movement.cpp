#include "movement.h"
#include "../../sdk/sdk.h"
#include "../../sdk/offsets.h"
#include <Windows.h>
#include <cmath>

extern SDK::Roblox g_SDK;

namespace Movement {

static bool KeyActive(int key, int mode, bool& toggleState) {
    if (key == 0) return mode == 1 ? toggleState : false;
    bool down = (GetAsyncKeyState(key) & 0x8000) != 0;
    if (mode == 1) {
        static bool was = false;
        if (down && !was) toggleState = !toggleState;
        was = down;
        return toggleState;
    }
    return down;
}

void Update(Memory& mem) {
    if (!g_SDK.localPlayer || !g_SDK.mem) return;

    uintptr_t character = g_SDK.mem->Read<uintptr_t>(g_SDK.localPlayer + Offsets::Player_ModelInstance);
    if (!character) return;

    uintptr_t humanoid = g_SDK.FindFirstChildOfClass(character, "Humanoid");
    uintptr_t hrp = g_SDK.FindFirstChild(character, "HumanoidRootPart");
    if (!humanoid || !hrp) return;

    if (speed.enabled && KeyActive(speed.key, speed.activationMode, speedToggle)) {
        if (speed.mode == 1) {
            g_SDK.mem->Write<float>(humanoid + Offsets::Humanoid_Walkspeed, speed.speed);
        }
    }

    if (fly.enabled && KeyActive(fly.key, fly.activationMode, flyToggle)) {
    }

    if (desync.enabled) {
        static bool was = false;
        bool down = desync.key && (GetAsyncKeyState(desync.key) & 0x8000);
        if (down && !was) desyncActive = !desyncActive;
        was = down;
    }

    (void)mem;
}
}
