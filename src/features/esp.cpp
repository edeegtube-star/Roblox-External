#include "esp.h"
#include "../sdk/sdk.h"
#include "../render/overlay.h"
#include <cmath>

// Defined in main.cpp
extern SDK::Roblox g_SDK;

namespace ESP {

void Update(Memory& mem) {
    players.clear();
    if (!settings.enabled) return;

    auto list = g_SDK.GetPlayers();
    for (auto& p : list) {
        if (!p.isValid) continue;

        float dist = (p.position - Vector3{}).Length(); // replace with local pos distance later
        if (settings.maxDistance > 0.f && dist > settings.maxDistance)
            continue;

        players.push_back(p);
    }

    (void)mem;
}

void Render() {
    if (!settings.enabled) return;

    for (const auto& p : players) {
        if (!p.isValid) continue;
        if (settings.teamCheck && p.isTeammate) continue;

        Vector2 screen{};
        if (!g_SDK.WorldToScreen(p.position, screen))
            continue;

        float boxH = 60.f; // approximate until head/foot bones used
        float boxW = boxH / 2.f;
        float x = screen.x - boxW * 0.5f;
        float y = screen.y - boxH * 0.5f;

        unsigned int col = p.isTeammate ? 0xFF00FF00 : 0xFF0000FF;

        if (settings.box2D)
            Overlay::DrawBox(x, y, boxW, boxH, col, 1.5f);
        if (settings.boxCorner)
            Overlay::DrawCornerBox(x, y, boxW, boxH, col, 1.5f);

        if (settings.name && !p.name.empty())
            Overlay::DrawText(screen.x, y - 14.f, p.name.c_str(), 0xFFFFFFFF);

        if (settings.distance) {
            char buf[32];
            // distance placeholder
            snprintf(buf, sizeof(buf), "%.0fm", 0.f);
            Overlay::DrawText(screen.x, y + boxH + 2.f, buf, 0xFFCCCCCC);
        }
    }
}

} // namespace ESP
