#include "triggerbot.h"
#include "../sdk/sdk.h"
#include "esp.h"
#include <Windows.h>
#include <chrono>
#include <thread>
#include <cmath>

extern SDK::Roblox g_SDK;

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
        if (elapsed < 700) return;
        locked = false;
    }

    // Crosshair proximity: any enemy screen-pos within ~8px of center
    float cx = g_SDK.screenW * 0.5f;
    float cy = g_SDK.screenH * 0.5f;
    bool canShoot = false;

    for (const auto& p : ESP::players) {
        if (!p.isValid) continue;
        if (settings.teamCheck && p.isTeammate) continue;

        Vector3 aim = p.head ? g_SDK.GetPartPosition(p.head) : p.position;
        Vector2 scr{};
        if (!g_SDK.WorldToScreen(aim, scr)) continue;

        float d = sqrtf((scr.x - cx) * (scr.x - cx) + (scr.y - cy) * (scr.y - cy));
        if (d < 8.0f) {
            canShoot = true;
            break;
        }
    }

    if (canShoot && !shooting) {
        if (settings.reactionMs > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(settings.reactionMs));

        INPUT down{};
        down.type = INPUT_MOUSE;
        down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &down, sizeof(INPUT));

        shooting = true;
        lastShot = std::chrono::steady_clock::now();
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
