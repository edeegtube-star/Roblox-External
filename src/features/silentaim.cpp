#include "silentaim.h"
#include "../sdk/sdk.h"
#include "esp.h"
#include "aimbot.h"
#include <cmath>
#include <Windows.h>

extern SDK::Roblox g_SDK;

namespace SilentAim {

static bool KeyActive() {
    if (settings.key == 0) return settings.activationMode == 1 ? toggleState : false;
    bool down = (GetAsyncKeyState(settings.key) & 0x8000) != 0;
    if (settings.activationMode == 1) {
        static bool was = false;
        if (down && !was) toggleState = !toggleState;
        was = down;
        return toggleState;
    }
    return down;
}

void Update(Memory& mem) {
    (void)mem;
    if (!settings.enabled) return;
    if (!KeyActive()) return;

    float cx = g_SDK.screenW * 0.5f;
    float cy = g_SDK.screenH * 0.5f;
    float bestDist = settings.fov;
    Vector2 bestScreen{};
    bool found = false;
    static uintptr_t sticky = 0;
    if (!settings.stickyAim) sticky = 0;

    for (const auto& p : ESP::players) {
        if (!p.isValid || !p.onScreen) continue;
        if (settings.teamCheck && p.isTeammate) continue;
        if (settings.healthCheck && p.health < settings.minHealth) continue;
        if (settings.maxDistance > 0.f && p.distance > settings.maxDistance) continue;
        if (settings.stickyAim && sticky && p.address != sticky) continue;

        Vector3 aimWorld = p.position;
        if (settings.hitPart == 0 && p.head) aimWorld = g_SDK.GetPartPosition(p.head);
        else if (p.torso) aimWorld = g_SDK.GetPartPosition(p.torso);
        else aimWorld.y += 1.5f;

        if (settings.prediction)
            aimWorld = Aimbot::Predict(aimWorld, p.velocity, settings.predictionScale);

        Vector2 scr{};
        if (!g_SDK.WorldToScreen(aimWorld, scr)) continue;

        float dx = scr.x - cx, dy = scr.y - cy;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist > settings.fov) continue;
        if (dist < bestDist) {
            bestDist = dist; bestScreen = scr; found = true;
            if (settings.stickyAim) sticky = p.address;
        }
    }

    if (!found) return;

    float dx = bestScreen.x - cx;
    float dy = bestScreen.y - cy;
    if (fabsf(dx) < 0.5f && fabsf(dy) < 0.5f) return;

    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.dx = (LONG)(dx * 0.35f);
    in.mi.dy = (LONG)(dy * 0.35f);
    in.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &in, sizeof(INPUT));
}
}
