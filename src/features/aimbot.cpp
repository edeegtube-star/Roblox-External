#include "aimbot.h"
#include "../sdk/sdk.h"
#include "../sdk/offsets.h"
#include "esp.h"
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <Windows.h>

extern SDK::Roblox g_SDK;

namespace Aimbot {

Vector3 Predict(const Vector3& pos, const Vector3& vel, float scale) {
    if (!settings.prediction) return pos;
    float t = 0.05f * scale;
    return pos + vel * t;
}

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

static void MoveMouseToward(float targetX, float targetY, float screenCX, float screenCY) {
    float dx = targetX - screenCX;
    float dy = targetY - screenCY;
    float smooth = settings.smoothing;
    if (smooth < 1.f) smooth = 1.f;
    switch (settings.smoothType) {
    case 1: dx *= (1.f / (smooth * 0.85f)); dy *= (1.f / (smooth * 0.85f)); break;
    case 2:
        dx *= (1.f / smooth); dy *= (1.f / smooth);
        dx += ((rand() % 5) - 2) * 0.15f; dy += ((rand() % 5) - 2) * 0.15f;
        break;
    default: dx *= (1.f / smooth); dy *= (1.f / smooth); break;
    }
    dx *= settings.sensitivity; dy *= settings.sensitivity;
    if (fabsf(dx) < 0.05f && fabsf(dy) < 0.05f) return;
    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.dx = (LONG)dx; in.mi.dy = (LONG)dy;
    in.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &in, sizeof(INPUT));
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
    static uintptr_t stickyTarget = 0;
    if (!settings.stickyAim) stickyTarget = 0;
    for (const auto& p : ESP::players) {
        if (!p.isValid || !p.onScreen) continue;
        if (settings.teamCheck && p.isTeammate) continue;
        if (settings.healthCheck && p.health < settings.minHealth) continue;
        if (settings.maxDistance > 0.f && p.distance > settings.maxDistance) continue;
        if (settings.stickyAim && stickyTarget && p.address != stickyTarget) continue;
        Vector3 aimWorld = p.position;
        if (settings.hitPart == 0 && p.head) aimWorld = g_SDK.GetPartPosition(p.head);
        else if (settings.hitPart == 1 && p.torso) aimWorld = g_SDK.GetPartPosition(p.torso);
        else aimWorld.y += 1.5f;
        aimWorld = Predict(aimWorld, p.velocity, settings.predictionScale);
        Vector2 scr{};
        if (!g_SDK.WorldToScreen(aimWorld, scr)) continue;
        float dx = scr.x - cx, dy = scr.y - cy;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist > settings.fov) continue;
        if (dist < bestDist) {
            bestDist = dist; bestScreen = scr; found = true;
            if (settings.stickyAim) stickyTarget = p.address;
        }
    }
    if (found) MoveMouseToward(bestScreen.x, bestScreen.y, cx, cy);
}
}
