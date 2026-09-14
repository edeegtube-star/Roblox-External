#include "triggerbot.h"
#include "../sdk/offsets.h"
#include <chrono>

namespace Triggerbot {

static std::chrono::steady_clock::time_point lastShot{};
static bool locked = false;
static bool shooting = false;

void Update(Memory& mem) {
    if (!settings.enabled) return;

    bool keyDown = (GetAsyncKeyState(settings.key) & 0x8000) != 0;

    if (!keyDown) {
        if (shooting) {
            INPUT up{};
            up.type = INPUT_MOUSE;
            up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &up, sizeof(INPUT));
            shooting = false;
        }
        return;
    }

    auto now = std::chrono::steady_clock::now();

    if (settings.antiDoubleShot && locked) {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShot).count();
        // Placeholder: real implementation reads tool cooldown / ammo / animation state
        if (elapsed < 700) return;
        locked = false;
    }

    // Crosshair entity check goes here (GetAimingEntity equivalent for Roblox)
    bool canShoot = false; // set true when valid enemy under crosshair

    if (canShoot && !shooting) {
        std::this_thread::sleep_for(std::chrono::milliseconds(settings.reactionMs));

        INPUT down{};
        down.type = INPUT_MOUSE;
        down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &down, sizeof(INPUT));

        shooting = true;
        lastShot = now;
        if (settings.antiDoubleShot) locked = true;
    }
    else if (shooting && !canShoot) {
        INPUT up{};
        up.type = INPUT_MOUSE;
        up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &up, sizeof(INPUT));
        shooting = false;
    }

    (void)mem;
}

} // namespace Triggerbot
