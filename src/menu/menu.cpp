#include "menu.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../features/aimbot.h"
#include "../features/silentaim.h"
#include "../features/triggerbot.h"
#include "../features/esp.h"
#include "../features/movement/movement.h"
#include "../stealth/streamproof.h"
#include "../stealth/selfdestruct.h"
#include "../sdk/structures.h"
#include <cstdio>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif

namespace Menu {

static bool g_open = true;
static bool g_initialized = false;
static HWND g_hwnd = nullptr;
static int g_tab = 0;

static const char* kTabs[] = { "Combat", "Visuals", "Movement", "Stealth", "Config" };
static const char* kSmooth[] = { "Linear", "Ease", "Humanized" };
static const char* kHit[] = { "Head", "Torso", "Closest" };
static const char* kBox[] = { "Normal", "Corner" };
static const char* kAct[] = { "Hold", "Toggle" };

static void Style() {
    ImGuiStyle& s = ImGui::GetStyle();
    ImVec4* c = s.Colors;
    s.WindowRounding = 6.f; s.ChildRounding = 4.f; s.FrameRounding = 3.f;
    s.WindowPadding = ImVec2(10, 10); s.FramePadding = ImVec2(6, 4);
    const ImVec4 bg(0.07f,0.07f,0.09f,0.98f);
    const ImVec4 bgd(0.05f,0.05f,0.07f,1.f);
    const ImVec4 bgm(0.10f,0.10f,0.13f,1.f);
    const ImVec4 acc(0.18f,0.64f,0.96f,1.f);
    c[ImGuiCol_WindowBg]=bg; c[ImGuiCol_ChildBg]=bgd; c[ImGuiCol_FrameBg]=bgm;
    c[ImGuiCol_CheckMark]=acc; c[ImGuiCol_SliderGrab]=acc; c[ImGuiCol_ButtonActive]=acc;
    c[ImGuiCol_HeaderActive]=acc; c[ImGuiCol_TabActive]=ImVec4(0.14f,0.16f,0.22f,1.f);
}

static const char* VkName(int vk) {
    if (vk == 0) return "None";
    if (vk == VK_LBUTTON) return "LMB";
    if (vk == VK_RBUTTON) return "RMB";
    if (vk == VK_XBUTTON1) return "X1";
    if (vk == VK_XBUTTON2) return "X2";
    if (vk >= 'A' && vk <= 'Z') { static char b[2]; b[0]=(char)vk; b[1]=0; return b; }
    return "?";
}

static void KeyBind(const char* label, int& key) {
    ImGui::Text("%s", label);
    ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.55f);
    char buf[32];
    snprintf(buf, sizeof(buf), "[ %s ]##%s", VkName(key), label);
    if (ImGui::Button(buf, ImVec2(90, 0))) {
        for (int i = 1; i < 256; ++i)
            if (GetAsyncKeyState(i) & 0x8000) { key = i; break; }
    }
}

bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* ctx) {
    if (g_initialized) return true;
    g_hwnd = hwnd;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    Style();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, ctx);
    g_initialized = true;
    return true;
}

void Shutdown() {
    if (!g_initialized) return;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    g_initialized = false;
}

void BeginFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void EndFrame() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Toggle() { g_open = !g_open; }
bool IsOpen() { return g_open; }

bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (g_initialized && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;
    return false;
}

void Render() {
    if (!g_open) return;
    ImGui::SetNextWindowSize(ImVec2(720, 480), ImGuiCond_FirstUseEver);
    ImGui::Begin("UberDelivery", &g_open, ImGuiWindowFlags_NoCollapse);

    for (int i = 0; i < 5; ++i) {
        if (i) ImGui::SameLine();
        bool sel = (g_tab == i);
        if (sel) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f,0.64f,0.96f,0.45f));
        if (ImGui::Button(kTabs[i], ImVec2(120, 28))) g_tab = i;
        if (sel) ImGui::PopStyleColor();
    }
    ImGui::Separator();

    if (g_tab == 0) {
        ImGui::Checkbox("Aimbot", &Aimbot::settings.enabled);
        KeyBind("Aim key", Aimbot::settings.key);
        ImGui::SliderFloat("FOV##ab", &Aimbot::settings.fov, 1.f, 600.f, "%.0f");
        ImGui::SliderFloat("Smooth", &Aimbot::settings.smoothing, 1.f, 30.f, "%.1f");
        ImGui::Combo("Smooth type", &Aimbot::settings.smoothType, kSmooth, 3);
        ImGui::Checkbox("Prediction##ab", &Aimbot::settings.prediction);
        ImGui::Checkbox("Sticky##ab", &Aimbot::settings.stickyAim);
        ImGui::Checkbox("Team check##ab", &Aimbot::settings.teamCheck);
        ImGui::Checkbox("Draw FOV##ab", &Aimbot::settings.drawFov);
        ImGui::Separator();
        ImGui::Checkbox("Silent Aim", &SilentAim::settings.enabled);
        KeyBind("Silent key", SilentAim::settings.key);
        ImGui::SliderFloat("FOV##sa", &SilentAim::settings.fov, 1.f, 400.f, "%.0f");
        ImGui::Checkbox("Team check##sa", &SilentAim::settings.teamCheck);
        ImGui::Checkbox("Draw FOV##sa", &SilentAim::settings.drawFov);
        ImGui::Separator();
        ImGui::Checkbox("Triggerbot", &Triggerbot::settings.enabled);
        KeyBind("Trigger key", Triggerbot::settings.key);
        ImGui::SliderInt("Delay ms", &Triggerbot::settings.delayMs, 0, 200);
        ImGui::SliderInt("Release ms", &Triggerbot::settings.releaseMs, 0, 200);
    }
    else if (g_tab == 1) {
        ImGui::Checkbox("ESP master", &ESP::settings.enabled);
        ImGui::Checkbox("Boxes", &ESP::settings.boxes);
        ImGui::Combo("Box type", &ESP::settings.boxType, kBox, 2);
        ImGui::Checkbox("Box outline", &ESP::settings.boxOutline);
        ImGui::Checkbox("Healthbar", &ESP::settings.healthbar);
        ImGui::Checkbox("Skeleton", &ESP::settings.skeleton);
        ImGui::Checkbox("Name", &ESP::settings.name);
        ImGui::Checkbox("Distance", &ESP::settings.distance);
        ImGui::Checkbox("Tool", &ESP::settings.tool);
        ImGui::Checkbox("Tracers", &ESP::settings.tracers);
        ImGui::Checkbox("Snaplines", &ESP::settings.snaplines);
        ImGui::Checkbox("Chams", &ESP::settings.chams);
        ImGui::Checkbox("Team check##esp", &ESP::settings.teamCheck);
        ImGui::SliderFloat("Max distance", &ESP::settings.maxDistance, 0.f, 3000.f, "%.0f");
    }
    else if (g_tab == 2) {
        ImGui::Checkbox("Speed", &Movement::speed.enabled);
        KeyBind("Speed key", Movement::speed.key);
        ImGui::SliderFloat("Speed value", &Movement::speed.speed, 1.f, 500.f, "%.1f");
        ImGui::Checkbox("Fly", &Movement::fly.enabled);
        KeyBind("Fly key", Movement::fly.key);
        ImGui::Checkbox("Desync", &Movement::desync.enabled);
        KeyBind("Desync key", Movement::desync.key);
        ImGui::Checkbox("Noclip", &Movement::misc.noclip);
        ImGui::Checkbox("Inf jump", &Movement::misc.infJump);
    }
    else if (g_tab == 3) {
        ImGui::Checkbox("Streamproof", &Globals::streamproof);
        if (g_hwnd) {
            if (Globals::streamproof) Streamproof::Enable(g_hwnd);
            else Streamproof::Disable(g_hwnd);
        }
        ImGui::Checkbox("Performance mode", &Globals::performanceMode);
        ImGui::Checkbox("Watermark", &Globals::showWatermark);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f,0.12f,0.12f,1.f));
        if (ImGui::Button("Self-Destruct", ImVec2(-1, 0)))
            SelfDestruct::Execute();
        ImGui::PopStyleColor();
        ImGui::TextDisabled("END key also self-destructs");
    }
    else {
        ImGui::Text("UberDelivery closet external");
        ImGui::Text("INSERT = menu | END = self-destruct");
        if (ImGui::Button("Reset Combat", ImVec2(-1, 0))) {
            Aimbot::settings = {}; SilentAim::settings = {}; Triggerbot::settings = {};
        }
        if (ImGui::Button("Reset Visuals", ImVec2(-1, 0)))
            ESP::settings = {};
    }

    ImGui::End();

    if (Globals::showWatermark) {
        ImDrawList* dl = ImGui::GetBackgroundDrawList();
        dl->AddText(ImVec2(12, 10), IM_COL32(180, 200, 255, 200), "UberDelivery");
    }
}

} // namespace Menu
