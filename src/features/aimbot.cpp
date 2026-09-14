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

static Vector3 PredictPos(const Vector3& pos, const Vector3& vel) {
    if (!settings.prediction) return pos;
    float t = 0.05f * settings.predictionScale;
    return pos + vel * t;
}

static void MoveMouseToward(float targetX, float targetY, float screenCX, float screenCY) {
    float dx = targetX - screenCX;
    float dy = targetY - screenCY;

    float smooth = settings.smoothing;
    if (smooth < 1.f) smooth = 1.f;

    switch (settings.smoothType) {
    case 1: // Ease
        dx *= (1.f / (smooth * 0.85f));
        dy *= (1.f / (smooth * 0.85f));
        break;
    case 2: // Humanized
        dx *= (1.f / smooth);
        dy *= (1.f / smooth);
        dx += ((rand() % 5) - 2) * 0.15f;
        dy += ((rand() % 5) - 2) * 0.15f;
        break;
    default: // Linear
        dx *= (1.f / smooth);
        dy *= (1.f / smooth);
        break;
    }

    dx *= settings.sensitivity;
    dy *= settings.sensitivity;

    if (fabsf(dx) < 0.1f && fabsf(dy) < 0.1f) return;

    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.dx = (LONG)dx;
    in.mi.dy = (LONG)dy;
    in.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &in, sizeof(INPUT));
}

void Update(Memory& mem) {
    if (!settings.enabled) return;
    if (!(GetAsyncKeyState(settings.key) & 0x8000)) return;

    float cx = g_SDK.screenW * 0.5f;
    float cy = g_SDK.screenH * 0.5f;

    float bestDist = settings.fov;
    Vector2 bestScreen{};
    bool found = false;

    for (const auto& p : ESP::players) {
        if (!p.isValid) continue;
        if (settings.teamCheck && p.isTeammate) continue;

        Vector3 aimWorld = p.position;
        if (p.head)
            aimWorld = g_SDK.GetPartPosition(p.head);
        else
            aimWorld.y += 2.0f;

        aimWorld = PredictPos(aimWorld, p.velocity);

        Vector2 scr{};
        if (!g_SDK.WorldToScreen(aimWorld, scr)) continue;

        float d = sqrtf((scr.x - cx) * (scr.x - cx) + (scr.y - cy) * (scr.y - cy));
        if (d < bestDist) {
            bestDist = d;
            bestScreen = scr;
            found = true;
        }
    }

    if (found)
        MoveMouseToward(bestScreen.x, bestScreen.y, cx, cy);

    (void)mem;
}

} // namespace Aimbot
