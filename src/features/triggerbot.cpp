#include "triggerbot.h"
#include "../sdk/sdk.h"
#include "esp.h"
#include <cmath>
#include <chrono>
#include <Windows.h>

extern SDK::Roblox g_SDK;

namespace Triggerbot {

static auto lastShot = std::chrono::steady_clock::now();
static bool mouseDown = false;

void Update(Memory& mem) {
    (void)mem;
    if (!settings.enabled) return;

    bool keyOk = true;
    if (settings.activationMode == 0 && settings.key != 0)
        keyOk = (GetAsyncKeyState(settings.key) & 0x8000) != 0;
    if (!keyOk) {
        if (mouseDown) {
            INPUT up{};
            up.type = INPUT_MOUSE;
            up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &up, sizeof(INPUT));
            mouseDown = false;
        }
        return;
    }

    float cx = g_SDK.screenW * 0.5f;
    float cy = g_SDK.screenH * 0.5f;
    bool onTarget = false;

    for (const auto& p : ESP::players) {
        if (!p.isValid || !p.onScreen) continue;
        if (settings.teamCheck && p.isTeammate) continue;
        if (settings.maxDistance > 0.f && p.distance > settings.maxDistance) continue;

        float halfW = (p.boxW * 0.5f) * settings.hitboxMul;
        float halfH = (p.boxH * 0.5f) * settings.hitboxMul;
        float left = p.screenHRP.x - halfW;
        float right = p.screenHRP.x + halfW;
        float top = p.screenHead.y;
        float bottom = p.screenFeet.y;

        if (cx >= left && cx <= right && cy >= top && cy <= bottom) {
            onTarget = true;
            break;
        }
    }

    auto now = std::chrono::steady_clock::now();
    if (onTarget && !mouseDown) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShot).count();
        if (ms >= settings.delayMs) {
            INPUT down{};
            down.type = INPUT_MOUSE;
            down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &down, sizeof(INPUT));
            mouseDown = true;
            lastShot = now;
        }
    } else if (!onTarget && mouseDown) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShot).count();
        if (ms >= settings.releaseMs) {
            INPUT up{};
            up.type = INPUT_MOUSE;
            up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &up, sizeof(INPUT));
            mouseDown = false;
        }
    }
}
}
