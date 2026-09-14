#include "menu.h"
#include "../features/aimbot.h"
#include "../features/silentaim.h"
#include "../features/triggerbot.h"
#include "../features/esp.h"

// Dear ImGui - place imgui sources under third_party/imgui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <Windows.h>
#include <d3d11.h>
#include <string>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Menu {

static bool open = true;
static bool initialized = false;

static void StyleUberDelivery() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 8.f;
    s.ChildRounding = 6.f;
    s.FrameRounding = 4.f;
    s.PopupRounding = 4.f;
    s.ScrollbarRounding = 4.f;
    s.GrabRounding = 3.f;
    s.TabRounding = 4.f;
    s.WindowPadding = ImVec2(12, 12);
    s.FramePadding = ImVec2(8, 4);
    s.ItemSpacing = ImVec2(8, 6);
    s.ItemInnerSpacing = ImVec2(6, 4);
    s.WindowBorderSize = 1.f;
    s.FrameBorderSize = 0.f;

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg]        = ImVec4(0.07f, 0.07f, 0.09f, 0.96f);
    c[ImGuiCol_ChildBg]         = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
    c[ImGuiCol_PopupBg]         = ImVec4(0.08f, 0.08f, 0.10f, 0.98f);
    c[ImGuiCol_Border]          = ImVec4(0.25f, 0.25f, 0.30f, 0.60f);
    c[ImGuiCol_FrameBg]         = ImVec4(0.14f, 0.14f, 0.17f, 1.00f);
    c[ImGuiCol_FrameBgHovered]  = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
    c[ImGuiCol_FrameBgActive]   = ImVec4(0.24f, 0.24f, 0.30f, 1.00f);
    c[ImGuiCol_TitleBg]         = ImVec4(0.10f, 0.10f, 0.13f, 1.00f);
    c[ImGuiCol_TitleBgActive]   = ImVec4(0.12f, 0.12f, 0.16f, 1.00f);
    c[ImGuiCol_CheckMark]       = ImVec4(0.35f, 0.70f, 1.00f, 1.00f);
    c[ImGuiCol_SliderGrab]      = ImVec4(0.35f, 0.70f, 1.00f, 1.00f);
    c[ImGuiCol_SliderGrabActive]= ImVec4(0.45f, 0.80f, 1.00f, 1.00f);
    c[ImGuiCol_Button]          = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    c[ImGuiCol_ButtonHovered]   = ImVec4(0.28f, 0.40f, 0.60f, 1.00f);
    c[ImGuiCol_ButtonActive]    = ImVec4(0.35f, 0.55f, 0.85f, 1.00f);
    c[ImGuiCol_Header]          = ImVec4(0.20f, 0.35f, 0.55f, 0.80f);
    c[ImGuiCol_HeaderHovered]   = ImVec4(0.28f, 0.45f, 0.70f, 0.90f);
    c[ImGuiCol_HeaderActive]    = ImVec4(0.35f, 0.55f, 0.85f, 1.00f);
    c[ImGuiCol_Tab]             = ImVec4(0.12f, 0.12f, 0.16f, 1.00f);
    c[ImGuiCol_TabHovered]      = ImVec4(0.28f, 0.45f, 0.70f, 0.90f);
    c[ImGuiCol_TabActive]       = ImVec4(0.22f, 0.40f, 0.65f, 1.00f);
    c[ImGuiCol_Text]            = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    c[ImGuiCol_TextDisabled]    = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    c[ImGuiCol_Separator]       = ImVec4(0.25f, 0.25f, 0.30f, 0.60f);
}

bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
    if (initialized) return true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr; // no imgui.ini on disk

    StyleUberDelivery();

    if (!ImGui_ImplWin32_Init(hwnd)) return false;
    if (!ImGui_ImplDX11_Init(device, context)) return false;

    initialized = true;
    return true;
}

void Shutdown() {
    if (!initialized) return;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    initialized = false;
}

void Toggle() { open = !open; }
bool IsOpen() { return open; }

void NotifyWndProc(HWND hwnd, unsigned int msg, unsigned long long wParam, long long lParam) {
    if (initialized)
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, (WPARAM)wParam, (LPARAM)lParam);
}

void BeginFrame() {
    if (!initialized) return;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void EndFrame() {
    if (!initialized) return;
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

static void TabCombat() {
    if (ImGui::BeginTabItem("Combat")) {
        // Aimbot
        if (ImGui::CollapsingHeader("Aimbot", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("Enabled##aim", &Aimbot::settings.enabled);
            ImGui::SliderFloat("Sensitivity##aim", &Aimbot::settings.sensitivity, 0.1f, 5.0f, "%.2f");
            ImGui::SliderFloat("Smoothing##aim", &Aimbot::settings.smoothing, 1.0f, 30.0f, "%.1f");

            const char* smoothTypes[] = { "Linear", "Ease", "Humanized" };
            ImGui::Combo("Smooth Type##aim", &Aimbot::settings.smoothType, smoothTypes, IM_ARRAYSIZE(smoothTypes));

            ImGui::Checkbox("Prediction##aim", &Aimbot::settings.prediction);
            if (Aimbot::settings.prediction)
                ImGui::SliderFloat("Prediction Scale##aim", &Aimbot::settings.predictionScale, 0.1f, 3.0f, "%.2f");

            ImGui::SliderFloat("FOV##aim", &Aimbot::settings.fov, 10.0f, 300.0f, "%.0f");
            ImGui::Checkbox("Team Check##aim", &Aimbot::settings.teamCheck);
            ImGui::Checkbox("Visible Check##aim", &Aimbot::settings.visibleCheck);

            ImGui::Text("Key: RMB (change in code for now)");
        }

        // Silent Aim
        if (ImGui::CollapsingHeader("Silent Aim")) {
            ImGui::Checkbox("Enabled##silent", &SilentAim::settings.enabled);
            ImGui::SliderFloat("Sensitivity##silent", &SilentAim::settings.sensitivity, 0.1f, 5.0f, "%.2f");
            ImGui::Checkbox("Prediction##silent", &SilentAim::settings.prediction);
            if (SilentAim::settings.prediction)
                ImGui::SliderFloat("Prediction Scale##silent", &SilentAim::settings.predictionScale, 0.1f, 3.0f, "%.2f");
            ImGui::SliderFloat("FOV##silent", &SilentAim::settings.fov, 10.0f, 200.0f, "%.0f");
            ImGui::Checkbox("Team Check##silent", &SilentAim::settings.teamCheck);

            const char* hitParts[] = { "Head", "Torso" };
            ImGui::Combo("Hit Part##silent", &SilentAim::settings.hitPart, hitParts, IM_ARRAYSIZE(hitParts));
        }

        // Triggerbot
        if (ImGui::CollapsingHeader("Triggerbot")) {
            ImGui::Checkbox("Enabled##trig", &Triggerbot::settings.enabled);
            ImGui::SliderInt("Reaction (ms)##trig", &Triggerbot::settings.reactionMs, 0, 300);
            ImGui::Checkbox("Anti Double Shot##trig", &Triggerbot::settings.antiDoubleShot);
            ImGui::Checkbox("Team Check##trig", &Triggerbot::settings.teamCheck);
            ImGui::Text("Key: Mouse4 (change in code for now)");
        }

        ImGui::EndTabItem();
    }
}

static void TabVisuals() {
    if (ImGui::BeginTabItem("Visuals")) {
        ImGui::Checkbox("ESP Enabled", &ESP::settings.enabled);

        if (ImGui::CollapsingHeader("Boxes", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("2D Box", &ESP::settings.box2D);
            ImGui::Checkbox("3D Box", &ESP::settings.box3D);
            ImGui::Checkbox("Corner Box", &ESP::settings.boxCorner);
        }

        if (ImGui::CollapsingHeader("Info")) {
            ImGui::Checkbox("Name", &ESP::settings.name);
            ImGui::Checkbox("Distance", &ESP::settings.distance);
            ImGui::Checkbox("Skeleton", &ESP::settings.skeleton);
            ImGui::Checkbox("Health Bar", &ESP::settings.healthBar);
            ImGui::Checkbox("Team Check##esp", &ESP::settings.teamCheck);
            ImGui::SliderFloat("Max Distance", &ESP::settings.maxDistance, 50.0f, 2000.0f, "%.0f");
        }

        ImGui::EndTabItem();
    }
}

static void TabStealth() {
    if (ImGui::BeginTabItem("Stealth")) {
        ImGui::TextWrapped("Streamproof: WDA_EXCLUDEFROMCAPTURE active on overlay.");
        ImGui::TextWrapped("Blocks OBS (WGC/DXGI), Discord, Game Bar.");
        ImGui::TextWrapped("NVIDIA Shadowplay / AMD ReLive need kernel/DMA for full coverage.");

        ImGui::Separator();
        ImGui::Text("Self Destruct: END key");
        ImGui::TextWrapped("Wipes executable via delayed batch and exits process.");

        ImGui::Separator();
        ImGui::TextDisabled("Echo.ac / Detect.ac: usermode external, no module inject.");

        ImGui::EndTabItem();
    }
}

static void TabConfig() {
    if (ImGui::BeginTabItem("Config")) {
        ImGui::Text("UberDelivery");
        ImGui::TextDisabled("Config save/load coming next.");

        ImGui::Separator();
        if (ImGui::Button("Close Menu", ImVec2(-1, 0)))
            open = false;

        ImGui::Spacing();
        ImGui::TextWrapped("INSERT = toggle menu");
        ImGui::TextWrapped("END = self destruct");

        ImGui::EndTabItem();
    }
}

void Render() {
    if (!initialized || !open) return;

    ImGui::SetNextWindowSize(ImVec2(480, 420), ImGuiCond_FirstUseEver);
    ImGui::Begin("UberDelivery", &open, ImGuiWindowFlags_NoCollapse);

    if (ImGui::BeginTabBar("##main_tabs", ImGuiTabBarFlags_None)) {
        TabCombat();
        TabVisuals();
        TabStealth();
        TabConfig();
        ImGui::EndTabBar();
    }

    ImGui::End();
}

} // namespace Menu
