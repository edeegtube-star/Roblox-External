#include "menu.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "../features/aimbot.h"
#include "../features/silentaim.h"
#include "../features/triggerbot.h"
#include "../features/esp.h"
#include "../stealth/streamproof.h"

#include <cstdio>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif

namespace Menu {

static bool g_open = true;
static bool g_initialized = false;
static HWND g_hwnd = nullptr;

static const char* kSmoothTypes[] = { "Linear", "Ease", "Humanized" };
static const char* kHitParts[] = { "Head", "Torso" };

static void StyleUberDelivery() {
    ImGuiStyle& s = ImGui::GetStyle();
    ImVec4* c = s.Colors;

    s.WindowRounding = 8.0f;
    s.ChildRounding = 6.0f;
    s.FrameRounding = 5.0f;
    s.PopupRounding = 5.0f;
    s.ScrollbarRounding = 6.0f;
    s.GrabRounding = 4.0f;
    s.TabRounding = 5.0f;

    s.WindowBorderSize = 1.0f;
    s.FrameBorderSize = 0.0f;
    s.WindowPadding = ImVec2(12, 12);
    s.FramePadding = ImVec2(8, 5);
    s.ItemSpacing = ImVec2(8, 7);
    s.ItemInnerSpacing = ImVec2(6, 5);
    s.IndentSpacing = 18.0f;
    s.ScrollbarSize = 12.0f;
    s.GrabMinSize = 10.0f;

    const ImVec4 bg = ImVec4(0.055f, 0.058f, 0.070f, 0.97f);
    const ImVec4 bg_dark = ImVec4(0.035f, 0.038f, 0.048f, 1.00f);
    const ImVec4 bg_med = ImVec4(0.085f, 0.090f, 0.110f, 1.00f);
    const ImVec4 accent = ImVec4(0.180f, 0.640f, 0.960f, 1.00f);
    const ImVec4 accent2 = ImVec4(0.130f, 0.480f, 0.820f, 1.00f);
    const ImVec4 text = ImVec4(0.880f, 0.900f, 0.940f, 1.00f);
    const ImVec4 dim = ImVec4(0.480f, 0.510f, 0.580f, 1.00f);

    c[ImGuiCol_Text] = text;
    c[ImGuiCol_TextDisabled] = dim;
    c[ImGuiCol_WindowBg] = bg;
    c[ImGuiCol_ChildBg] = bg_dark;
    c[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.075f, 0.09f, 0.98f);
    c[ImGuiCol_Border] = ImVec4(0.14f, 0.16f, 0.20f, 1.00f);
    c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_FrameBg] = bg_med;
    c[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.14f, 0.18f, 1.00f);
    c[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.18f, 0.24f, 1.00f);
    c[ImGuiCol_TitleBg] = bg_dark;
    c[ImGuiCol_TitleBgActive] = bg_dark;
    c[ImGuiCol_TitleBgCollapsed] = bg_dark;
    c[ImGuiCol_MenuBarBg] = bg_dark;
    c[ImGuiCol_ScrollbarBg] = bg_dark;
    c[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.20f, 0.26f, 1.00f);
    c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.27f, 0.34f, 1.00f);
    c[ImGuiCol_ScrollbarGrabActive] = accent;
    c[ImGuiCol_CheckMark] = accent;
    c[ImGuiCol_SliderGrab] = accent;
    c[ImGuiCol_SliderGrabActive] = accent;
    c[ImGuiCol_Button] = ImVec4(0.11f, 0.13f, 0.17f, 1.00f);
    c[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.20f, 0.26f, 1.00f);
    c[ImGuiCol_ButtonActive] = accent2;
    c[ImGuiCol_Header] = ImVec4(0.12f, 0.15f, 0.20f, 1.00f);
    c[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.20f, 0.26f, 1.00f);
    c[ImGuiCol_HeaderActive] = accent2;
    c[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.20f, 1.00f);
    c[ImGuiCol_SeparatorHovered] = accent2;
    c[ImGuiCol_SeparatorActive] = accent;
    c[ImGuiCol_ResizeGrip] = ImVec4(0.18f, 0.20f, 0.26f, 1.00f);
    c[ImGuiCol_ResizeGripHovered] = accent2;
    c[ImGuiCol_ResizeGripActive] = accent;
    c[ImGuiCol_Tab] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
    c[ImGuiCol_TabHovered] = ImVec4(0.16f, 0.20f, 0.26f, 1.00f);
    c[ImGuiCol_TabActive] = ImVec4(0.12f, 0.15f, 0.20f, 1.00f);
    c[ImGuiCol_TabUnfocused] = ImVec4(0.06f, 0.07f, 0.09f, 1.00f);
    c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.09f, 0.11f, 0.14f, 1.00f);
    c[ImGuiCol_PlotLines] = accent;
    c[ImGuiCol_PlotLinesHovered] = accent;
    c[ImGuiCol_PlotHistogram] = accent;
    c[ImGuiCol_PlotHistogramHovered] = accent;
    c[ImGuiCol_TextSelectedBg] = ImVec4(0.18f, 0.64f, 0.96f, 0.35f);
    c[ImGuiCol_NavHighlight] = accent;
    c[ImGuiCol_NavWindowingHighlight] = accent;
    c[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.45f);
    c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.55f);
}

static void SectionHeader(const char* label) {
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.18f, 0.64f, 0.96f, 1.00f));
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();
}

static const char* VkName(int vk) {
    switch (vk) {
    case 0: return "None";
    case VK_LBUTTON: return "LMB";
    case VK_RBUTTON: return "RMB";
    case VK_MBUTTON: return "MMB";
    case VK_XBUTTON1: return "Mouse4";
    case VK_XBUTTON2: return "Mouse5";
    case VK_SHIFT: return "Shift";
    case VK_CONTROL: return "Ctrl";
    case VK_MENU: return "Alt";
    case VK_SPACE: return "Space";
    case VK_INSERT: return "Insert";
    case VK_DELETE: return "Delete";
    case VK_HOME: return "Home";
    case VK_END: return "End";
    case VK_PRIOR: return "PgUp";
    case VK_NEXT: return "PgDn";
    default: break;
    }
    static char buf[32];
    UINT sc = MapVirtualKeyA((UINT)vk, MAPVK_VK_TO_VSC);
    if (GetKeyNameTextA((LONG)(sc << 16), buf, sizeof(buf)) > 0)
        return buf;
    snprintf(buf, sizeof(buf), "0x%02X", vk);
    return buf;
}

static void Keybind(const char* label, int* key) {
    static int waitingId = 0;

    ImGui::TextUnformatted(label);
    ImGui::SameLine(ImGui::GetWindowWidth() - 140.0f);

    ImGui::PushID(label);
    int id = ImGui::GetID("kb");

    char buf[64];
    if (waitingId == id)
        snprintf(buf, sizeof(buf), "[ press key ]");
    else
        snprintf(buf, sizeof(buf), "%s", VkName(*key));

    if (ImGui::Button(buf, ImVec2(120, 0))) {
        waitingId = id;
    }

    if (waitingId == id) {
        for (int vk = 1; vk <= 0xFE; ++vk) {
            if (vk == VK_INSERT || vk == VK_END) continue; // reserved
            if (GetAsyncKeyState(vk) & 0x1) {
                if (vk == VK_ESCAPE)
                    *key = 0;
                else
                    *key = vk;
                waitingId = 0;
                break;
            }
        }
    }
    ImGui::PopID();
}

static void CombatTab() {
    SectionHeader("Aimbot");
    ImGui::Checkbox("Enabled##aim", &Aimbot::settings.enabled);
    ImGui::SliderFloat("Sensitivity##aim", &Aimbot::settings.sensitivity, 0.1f, 5.0f, "%.2f");
    ImGui::SliderFloat("Smoothing##aim", &Aimbot::settings.smoothing, 1.0f, 30.0f, "%.1f");
    ImGui::Combo("Smooth Type##aim", &Aimbot::settings.smoothType, kSmoothTypes, IM_ARRAYSIZE(kSmoothTypes));
    ImGui::Checkbox("Prediction##aim", &Aimbot::settings.prediction);
    if (Aimbot::settings.prediction)
        ImGui::SliderFloat("Prediction Scale##aim", &Aimbot::settings.predictionScale, 0.1f, 3.0f, "%.2f");
    ImGui::SliderFloat("FOV##aim", &Aimbot::settings.fov, 10.0f, 300.0f, "%.0f");
    ImGui::Checkbox("Team Check##aim", &Aimbot::settings.teamCheck);
    ImGui::Checkbox("Visible Check##aim", &Aimbot::settings.visibleCheck);
    Keybind("Aim Key", &Aimbot::settings.key);

    SectionHeader("Silent Aim");
    ImGui::Checkbox("Enabled##sa", &SilentAim::settings.enabled);
    ImGui::SliderFloat("Sensitivity##sa", &SilentAim::settings.sensitivity, 0.1f, 5.0f, "%.2f");
    ImGui::Checkbox("Prediction##sa", &SilentAim::settings.prediction);
    if (SilentAim::settings.prediction)
        ImGui::SliderFloat("Prediction Scale##sa", &SilentAim::settings.predictionScale, 0.1f, 3.0f, "%.2f");
    ImGui::SliderFloat("FOV##sa", &SilentAim::settings.fov, 10.0f, 200.0f, "%.0f");
    ImGui::Checkbox("Team Check##sa", &SilentAim::settings.teamCheck);
    ImGui::Combo("Hit Part##sa", &SilentAim::settings.hitPart, kHitParts, IM_ARRAYSIZE(kHitParts));

    SectionHeader("Triggerbot");
    ImGui::Checkbox("Enabled##tb", &Triggerbot::settings.enabled);
    ImGui::SliderInt("Reaction Time (ms)##tb", &Triggerbot::settings.reactionMs, 0, 300);
    ImGui::Checkbox("Anti Double Shot##tb", &Triggerbot::settings.antiDoubleShot);
    ImGui::Checkbox("Team Check##tb", &Triggerbot::settings.teamCheck);
    Keybind("Trigger Key", &Triggerbot::settings.key);
}

static void VisualsTab() {
    SectionHeader("ESP");
    ImGui::Checkbox("Enabled##esp", &ESP::settings.enabled);
    ImGui::Spacing();
    ImGui::Checkbox("2D Box", &ESP::settings.box2D);
    ImGui::Checkbox("3D Box", &ESP::settings.box3D);
    ImGui::Checkbox("Corner Box", &ESP::settings.boxCorner);
    ImGui::Spacing();
    ImGui::Checkbox("Name", &ESP::settings.name);
    ImGui::Checkbox("Distance", &ESP::settings.distance);
    ImGui::Checkbox("Skeleton", &ESP::settings.skeleton);
    ImGui::Checkbox("Health Bar", &ESP::settings.healthBar);
    ImGui::Spacing();
    ImGui::Checkbox("Team Check##esp", &ESP::settings.teamCheck);
    ImGui::SliderFloat("Max Distance", &ESP::settings.maxDistance, 50.0f, 2000.0f, "%.0f");
}

static void StealthTab() {
    SectionHeader("Status");
    ImGui::TextColored(ImVec4(0.18f, 0.64f, 0.96f, 1.0f), "Streamproof active");
    ImGui::TextDisabled("OBS / Discord / Game Bar");
    ImGui::Spacing();
    ImGui::TextDisabled("NVIDIA / AMD GPU capture needs kernel / DMA");
    ImGui::Spacing();
    ImGui::TextDisabled("Self-destruct = END key");
    ImGui::TextDisabled("Usermode external, no inject");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if (ImGui::Button("Force Streamproof Re-Apply", ImVec2(-1, 0))) {
        if (g_hwnd)
            SetWindowDisplayAffinity(g_hwnd, WDA_EXCLUDEFROMCAPTURE);
    }
}

static void ConfigTab() {
    SectionHeader("Config");
    if (ImGui::Button("Save Config", ImVec2(-1, 0))) { /* phase 2 */ }
    if (ImGui::Button("Load Config", ImVec2(-1, 0))) { /* phase 2 */ }
    if (ImGui::Button("Reset Defaults", ImVec2(-1, 0))) {
        Aimbot::settings = {};
        Aimbot::settings.sensitivity = 1.0f;
        Aimbot::settings.smoothing = 5.0f;
        Aimbot::settings.prediction = true;
        Aimbot::settings.predictionScale = 1.0f;
        Aimbot::settings.fov = 120.0f;
        Aimbot::settings.key = VK_RBUTTON;
        Aimbot::settings.teamCheck = true;

        SilentAim::settings = {};
        SilentAim::settings.sensitivity = 1.0f;
        SilentAim::settings.prediction = true;
        SilentAim::settings.predictionScale = 1.0f;
        SilentAim::settings.fov = 80.0f;
        SilentAim::settings.teamCheck = true;

        Triggerbot::settings = {};
        Triggerbot::settings.reactionMs = 40;
        Triggerbot::settings.antiDoubleShot = true;
        Triggerbot::settings.key = VK_XBUTTON1;
        Triggerbot::settings.teamCheck = true;

        ESP::settings = {};
        ESP::settings.enabled = true;
        ESP::settings.box2D = true;
        ESP::settings.name = true;
        ESP::settings.distance = true;
        ESP::settings.teamCheck = true;
        ESP::settings.healthBar = true;
        ESP::settings.maxDistance = 1000.0f;
    }

    ImGui::Spacing();
    ImGui::TextDisabled("%APPDATA%\\UberDelivery\\config.json");

    SectionHeader("Session");
    if (ImGui::Button("Close Menu", ImVec2(-1, 0)))
        g_open = false;

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextDisabled("INSERT = menu    END = self-destruct");
}

bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* ctx) {
    if (g_initialized) return true;
    g_hwnd = hwnd;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    StyleUberDelivery();

    if (!ImGui_ImplWin32_Init(hwnd)) return false;
    if (!ImGui_ImplDX11_Init(device, ctx)) return false;

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
    if (!g_initialized) return;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Render() {
    if (!g_initialized || !g_open) return;

    ImGui::SetNextWindowSize(ImVec2(480, 420), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(420, 360), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("UberDelivery", &g_open, ImGuiWindowFlags_NoCollapse);

    if (ImGui::BeginTabBar("##uber_tabs")) {
        if (ImGui::BeginTabItem("Combat")) { CombatTab(); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Visuals")) { VisualsTab(); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Stealth")) { StealthTab(); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Config")) { ConfigTab(); ImGui::EndTabItem(); }
        ImGui::EndTabBar();
    }

    // Watermark
    {
        ImVec2 win = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        const char* wm = "UberDelivery";
        ImVec2 ts = ImGui::CalcTextSize(wm);
        ImGui::SetCursorScreenPos(ImVec2(win.x + size.x - ts.x - 12.0f, win.y + size.y - ts.y - 10.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.5f, 0.65f, 0.35f));
        ImGui::TextUnformatted(wm);
        ImGui::PopStyleColor();
    }

    ImGui::End();
}

void EndFrame() {
    if (!g_initialized) return;
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Toggle() { g_open = !g_open; }
bool IsOpen() { return g_open; }

bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (!g_initialized) return false;
    return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam) != 0;
}

} // namespace Menu
