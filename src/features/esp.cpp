#include "esp.h"
#include "../sdk/sdk.h"
#include "../sdk/offsets.h"
#include "aimbot.h"
#include "silentaim.h"
#include "imgui.h"
#include <cmath>
#include <cstdio>
#include <algorithm>

extern SDK::Roblox g_SDK;

namespace ESP {

static Vector3 g_localPos{};

static ImU32 Col(const float c[4]) {
    return IM_COL32((int)(c[0]*255), (int)(c[1]*255), (int)(c[2]*255), (int)(c[3]*255));
}

void Update(Memory& mem) {
    (void)mem;
    players.clear();
    g_localPos = {};
    if (g_SDK.localPlayer && g_SDK.mem) {
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
        p.distance = sqrtf(dx*dx + dy*dy + dz*dz);
        if (settings.maxDistance > 0.f && p.distance > settings.maxDistance) continue;
        Vector3 headPos = p.position; headPos.y += 2.5f;
        Vector3 feetPos = p.position; feetPos.y -= 0.5f;
        if (p.head) headPos = g_SDK.GetPartPosition(p.head);
        if (p.rootPart) feetPos = g_SDK.GetPartPosition(p.rootPart);
        Vector2 sh{}, sf{}, shrp{};
        bool hOk = g_SDK.WorldToScreen(headPos, sh);
        bool fOk = g_SDK.WorldToScreen(feetPos, sf);
        bool rOk = g_SDK.WorldToScreen(p.position, shrp);
        p.onScreen = hOk || fOk;
        if (!p.onScreen) continue;
        p.screenHead = sh; p.screenFeet = sf; p.screenHRP = shrp;
        p.boxH = fabsf(sf.y - sh.y);
        if (p.boxH < 4.f) p.boxH = 40.f;
        p.boxW = p.boxH * 0.55f;
        p.isKnocked = p.health <= 0.f;
        players.push_back(p);
    }
}

static void DrawCornerBox(ImDrawList* dl, float x, float y, float w, float h, ImU32 col, float thickness) {
    float cl = w * 0.25f, ct = h * 0.25f;
    dl->AddLine({x,y},{x+cl,y},col,thickness); dl->AddLine({x,y},{x,y+ct},col,thickness);
    dl->AddLine({x+w,y},{x+w-cl,y},col,thickness); dl->AddLine({x+w,y},{x+w,y+ct},col,thickness);
    dl->AddLine({x,y+h},{x+cl,y+h},col,thickness); dl->AddLine({x,y+h},{x,y+h-ct},col,thickness);
    dl->AddLine({x+w,y+h},{x+w-cl,y+h},col,thickness); dl->AddLine({x+w,y+h},{x+w,y+h-ct},col,thickness);
}

void Render() {
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    if (!dl) return;
    if (Aimbot::settings.enabled && Aimbot::settings.drawFov && Aimbot::settings.fov > 0.f) {
        ImVec2 c(g_SDK.screenW * 0.5f, g_SDK.screenH * 0.5f);
        dl->AddCircle(c, Aimbot::settings.fov, Col(Aimbot::settings.fovOutline), 64, 2.5f);
        dl->AddCircle(c, Aimbot::settings.fov, Col(Aimbot::settings.fovColour), 64, 1.2f);
    }
    if (SilentAim::settings.enabled && SilentAim::settings.drawFov && SilentAim::settings.fov > 0.f) {
        ImVec2 c(g_SDK.screenW * 0.5f, g_SDK.screenH * 0.5f);
        dl->AddCircle(c, SilentAim::settings.fov, Col(SilentAim::settings.fovOutline), 64, 2.5f);
        dl->AddCircle(c, SilentAim::settings.fov, Col(SilentAim::settings.fovColour), 64, 1.2f);
    }
    if (!settings.enabled) return;
    for (const auto& p : players) {
        if (!p.isValid || !p.onScreen) continue;
        if (settings.teamCheck && p.isTeammate) continue;
        if (settings.knockCheck && p.isKnocked) continue;
        if (settings.healthCheck && p.health < settings.minHealth) continue;
        float x = p.screenHead.x - p.boxW * 0.5f;
        float y = p.screenHead.y;
        float w = p.boxW, h = p.boxH;
        if (settings.chams && settings.chamsType == 1)
            dl->AddRectFilled({x,y},{x+w,y+h}, Col(settings.chamsColour), 2.f);
        if (settings.boxes) {
            if (settings.boxFill)
                dl->AddRectFilled({x,y},{x+w,y+h}, Col(settings.boxFillColour));
            if (settings.boxType == 0) {
                if (settings.boxOutline)
                    dl->AddRect({x-1,y-1},{x+w+1,y+h+1}, Col(settings.boxOutlineColour), 0, 0, 2.5f);
                dl->AddRect({x,y},{x+w,y+h}, Col(settings.boxColour), 0, 0, 1.2f);
            } else {
                if (settings.boxOutline)
                    DrawCornerBox(dl, x, y, w, h, Col(settings.boxOutlineColour), 2.5f);
                DrawCornerBox(dl, x, y, w, h, Col(settings.boxColour), 1.2f);
            }
        }
        if (settings.healthbar) {
            float pct = std::clamp(p.health / (p.maxHealth > 0 ? p.maxHealth : 100.f), 0.f, 1.f);
            float bx = x - 6.f, by = y, bh = h, bw = 3.5f;
            dl->AddRectFilled({bx,by},{bx+bw,by+bh}, Col(settings.healthbarBg));
            ImU32 hpCol = IM_COL32((int)((1.f-pct)*255), (int)(pct*255), 0, 255);
            dl->AddRectFilled({bx, by+bh*(1.f-pct)},{bx+bw, by+bh}, hpCol);
            dl->AddRect({bx-1,by-1},{bx+bw+1,by+bh+1}, IM_COL32(0,0,0,180));
        }
        if (settings.skeleton && p.head && p.rootPart) {
            auto bone = [&](uintptr_t a, uintptr_t b) {
                if (!a || !b) return;
                Vector3 wa = g_SDK.GetPartPosition(a);
                Vector3 wb = g_SDK.GetPartPosition(b);
                Vector2 sa{}, sb{};
                if (!g_SDK.WorldToScreen(wa, sa) || !g_SDK.WorldToScreen(wb, sb)) return;
                dl->AddLine({sa.x,sa.y},{sb.x,sb.y}, Col(settings.skeletonOutline), 2.5f);
                dl->AddLine({sa.x,sa.y},{sb.x,sb.y}, Col(settings.skeletonColour), 1.f);
            };
            bone(p.head, p.torso ? p.torso : p.rootPart);
            bone(p.torso ? p.torso : p.rootPart, p.rootPart);
            bone(p.torso ? p.torso : p.rootPart, p.leftArm);
            bone(p.torso ? p.torso : p.rootPart, p.rightArm);
            bone(p.rootPart, p.leftLeg);
            bone(p.rootPart, p.rightLeg);
        }
        if (settings.tracers) {
            ImVec2 origin;
            if (settings.tracerOrigin == 0) origin = { g_SDK.screenW*0.5f, 0.f };
            else if (settings.tracerOrigin == 2) origin = { g_SDK.screenW*0.5f, (float)g_SDK.screenH };
            else origin = { g_SDK.screenW*0.5f, g_SDK.screenH*0.5f };
            ImVec2 tgt = { p.screenFeet.x, p.screenFeet.y };
            dl->AddLine(origin, tgt, IM_COL32(0,0,0,160), 2.f);
            dl->AddLine(origin, tgt, Col(settings.tracersColour), 1.f);
        }
        if (settings.snaplines) {
            ImVec2 origin = { g_SDK.screenW*0.5f, g_SDK.screenH*0.5f };
            ImVec2 tgt = { p.screenHRP.x, p.screenHRP.y };
            dl->AddLine(origin, tgt, IM_COL32(0,0,0,140), 2.f);
            dl->AddLine(origin, tgt, Col(settings.snaplinesColour), 1.f);
        }
        float ty = y - 14.f;
        if (settings.name) {
            const char* n = (settings.nameType == 1 && !p.displayName.empty()) ? p.displayName.c_str() : p.name.c_str();
            ImVec2 ts = ImGui::CalcTextSize(n);
            dl->AddText({ p.screenHead.x - ts.x*0.5f + 1, ty + 1 }, IM_COL32(0,0,0,200), n);
            dl->AddText({ p.screenHead.x - ts.x*0.5f, ty }, Col(settings.nameColour), n);
            ty -= 13.f;
        }
        if (settings.distance) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%.0fm", p.distance);
            ImVec2 ts = ImGui::CalcTextSize(buf);
            dl->AddText({ p.screenHead.x - ts.x*0.5f + 1, ty + 1 }, IM_COL32(0,0,0,200), buf);
            dl->AddText({ p.screenHead.x - ts.x*0.5f, ty }, Col(settings.distanceColour), buf);
            ty -= 13.f;
        }
        if (settings.tool && !p.toolName.empty()) {
            ImVec2 ts = ImGui::CalcTextSize(p.toolName.c_str());
            dl->AddText({ p.screenHead.x - ts.x*0.5f + 1, ty + 1 }, IM_COL32(0,0,0,200), p.toolName.c_str());
            dl->AddText({ p.screenHead.x - ts.x*0.5f, ty }, Col(settings.toolColour), p.toolName.c_str());
        }
    }
}
}
