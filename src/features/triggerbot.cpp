#include "triggerbot.h"
#include <chrono>

namespace Triggerbot {

static std::chrono::steady_clock::time_point lastShot;
static bool locked = false;

void Update(Memory& mem) {
    if (!settings.enabled) return;
    if (!(GetAsyncKeyState(settings.key) & 0x8000)) return;

    auto now = std::chrono::steady_clock::now();

    if (settings.antiDoubleShot && locked) {
        // Wait until weapon is ready again (animation / reload finished)
        // Read weapon state / tool cooldown from memory when offsets available
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShot).count();
        if (elapsed < 800) return; // placeholder cooldown
        locked = false;
    }

    // Crosshair target check goes here
    // If valid enemy under crosshair:
    //   Sleep(settings.reactionMs);
    //   mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    //   mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    //   lastShot = now;
    //   if (settings.antiDoubleShot) locked = true;

    (void)mem;
}

}
