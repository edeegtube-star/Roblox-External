#include "esp.h"
#include "../sdk/sdk.h"
#include "../features/aimbot.h"
#include "imgui.h"
#include <cmath>
#include <cstdio>

extern SDK::Roblox g_SDK;

namespace ESP {

static Vector3 g_localPos{};

void Update(Memory& mem) {
    players.clear();
    if (!settings.enabled && !Aimbot::settings.enabled) return;

    // Local character position for distance
    g_localPos = {};
    if (g_SDK.localPlayer) {
        uintptr_t charModel = g_SDK.mem->Read<uintptr_t>(g_SDK.localPlayer + Offsets::Player_ModelInstance);
        if (charModel) {
            uintptr_t hrp = g_SDK.FindFirstChild(charModel, "HumanoidRootPart");
            if (hrp) g_localPos = g_SDK.GetPartPosition(hrp);
        }
    }

    auto list = g_SDK.GetPlayers();
    for (auto& p : list) {
        if (!p.isValid) continue;
        if (p.address == g_SDK.localPlayer) continue;

        float dx = p.position.x - g_localPos.x;
        float dy = p.position.y - g_localPos.y;
        float dz = p.position.z - g_localPos.z;
        float dist = sqrtf(dx * dx + dy * dy + dz * dz);

        if (settings.maxDistance > 0.f && dist > settings.maxDistance)
            continue;

        players.push_back(p);
    }

    (void)mem;
}

static void DrawCornerBox(ImDrawList* dl, float x, float y, float w, float h, ImU32 col, float thickness) {
    float cl = w * 0.25f;
    float ct = h * 0.25f;
    // TL
    dl->AddLine(ImVec2(x, y), ImVec2(x + cl, y), col, thickness);
    dl->AddLine(ImVec2(x, y), ImVec2(x, y + ct), col, thickness);
    // TR
    dl->AddLine(ImVec2(x + w, y), ImVec2(x + w - cl, y), col, thickness);
    dl->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + ct), col, thickness);
    // BL
    dl->AddLine(ImVec2(x, y + h), ImVec2(x + cl, y + h), col, thickness);
    dl->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - ct), col, thickness);
    // BR
    dl->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - cl, y + h), col, thickness);
    dl->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - ct), col, thickness);
}

void Render() {
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    if (!dl) return;

    // FOV circle for aimbot
    if (Aimbot::settings.enabled && Aimbot::settings.fov > 0.f) {
        ImVec2 center(g_SDK.screenW * 0.5f, g_SDK.screenH * 0.5f);
        dl->AddCircle(center, Aimbot::settings.fov, IM_COL32(255, 255, 255, 90), 64, 1.5f);
    }

    if (!settings.enabled) return;

    for (const auto& p : players) {
        if (!p.isValid) continue;
        if (settings.teamCheck && p.isTeammate) continue;

        Vector2 feet{}, head{};
        bool feetOk = g_SDK.WorldToScreen(p.position, feet);

        Vector3 headWorld = p.position;
        if (p.head) {
            headWorld = g_SDK.GetPartPosition(p.head);
        } else {
            headWorld.y += 2.5f; // approx
        }
        bool headOk = g_SDK.WorldToScreen(headWorld, head);

        if (!feetOk && !headOk) continue;

        float top = headOk ? head.y : feet.y - 60.f;
        float bottom = feetOk ? feet.y : head.y + 60.f;
        if (bottom < top) { float t = top; top = bottom; bottom = t; }

        float h = bottom - top;
        if (h < 8.f) h = 40.f;
        float w = h * 0.45f;
        float cx = feetOk ? feet.x : head.x;
        float x = cx - w * 0.5f;

        ImU32 boxCol = p.isTeammate ? IM_COL32(80, 220, 80, 220) : IM_COL32(220, 60, 60, 220);
        ImU32 outline = IM_COL32(0, 0, 0, 200);

        if (settings.box2D) {
            dl->AddRect(ImVec2(x - 1, top - 1), ImVec2(x + w + 1, top + h + 1), outline, 0.f, 0, 2.0f);
            dl->AddRect(ImVec2(x, top), ImVec2(x + w, top + h), boxCol, 0.f, 0, 1.5f);
        }

        if (settings.boxCorner) {
            DrawCornerBox(dl, x, top, w, h, outline, 2.5f);
            DrawCornerBox(dl, x, top, w, h, boxCol, 1.5f);
        }

        // Health bar
        if (settings.healthBar && p.maxHealth > 0.f) {
            float pct = p.health / p.maxHealth;
            if (pct < 0.f) pct = 0.f;
            if (pct > 1.f) pct = 1.f;
            float barX = x - 6.f;
            dl->AddRectFilled(ImVec2(barX - 1, top - 1), ImVec2(barX + 3, top + h + 1), outline);
            ImU32 hpCol = IM_COL32((int)((1.f - pct) * 255), (int)(pct * 255), 40, 255);
            dl->AddRectFilled(ImVec2(barX, top + h * (1.f - pct)), ImVec2(barX + 2, top + h), hpCol);
        }

        float textY = top - 16.f;

        if (settings.name && !p.name.empty()) {
            ImVec2 ts = ImGui::CalcTextSize(p.name.c_str());
            dl->AddText(ImVec2(cx - ts.x * 0.5f + 1, textY + 1), IM_COL32(0, 0, 0, 200), p.name.c_str());
            dl->AddText(ImVec2(cx - ts.x * 0.5f, textY), IM_COL32(255, 255, 255, 255), p.name.c_str());
            textY -= 14.f;
        }

        if (settings.distance) {
            float dx = p.position.x - g_localPos.x;
            float dy = p.position.y - g_localPos.y;
            float dz = p.position.z - g_localPos.z;
            float dist = sqrtf(dx * dx + dy * dy + dz * dz);
            char buf[32];
            snprintf(buf, sizeof(buf), "%.0fm", dist);
            ImVec2 ts = ImGui::CalcTextSize(buf);
            dl->AddText(ImVec2(cx - ts.x * 0.5f + 1, bottom + 3), IM_COL32(0, 0, 0, 200), buf);
            dl->AddText(ImVec2(cx - ts.x * 0.5f, bottom + 2), IM_COL32(200, 200, 200, 255), buf);
        }

        // Skeleton placeholder: head to root line
        if (settings.skeleton && headOk && feetOk) {
            dl->AddLine(ImVec2(head.x, head.y), ImVec2(feet.x, feet.y), boxCol, 1.2f);
        }
    }
}

} // namespace ESP
