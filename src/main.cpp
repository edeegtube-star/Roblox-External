#include <Windows.h>
#include <thread>
#include <chrono>

#include "memory/memory.h"
#include "features/aimbot.h"
#include "features/esp.h"
#include "features/triggerbot.h"
#include "features/silentaim.h"
#include "render/overlay.h"
#include "menu/menu.h"
#include "stealth/streamproof.h"
#include "stealth/selfdestruct.h"

Memory g_Memory;
bool g_Running = true;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    if (!g_Memory.Attach(L"RobloxPlayerBeta.exe")) {
        MessageBoxA(nullptr, "Failed to attach to Roblox", "UberDelivery", MB_ICONERROR);
        return 1;
    }

    if (!Overlay::Initialize()) {
        MessageBoxA(nullptr, "Failed to create overlay", "UberDelivery", MB_ICONERROR);
        g_Memory.Detach();
        return 1;
    }

    Streamproof::Enable(Overlay::GetHwnd());

    if (!Menu::Initialize(Overlay::GetHwnd(), Overlay::GetDevice(), Overlay::GetContext())) {
        MessageBoxA(nullptr, "Failed to init ImGui menu", "UberDelivery", MB_ICONERROR);
        Overlay::Shutdown();
        g_Memory.Detach();
        return 1;
    }

    while (g_Running) {
        if (GetAsyncKeyState(VK_END) & 1) {
            SelfDestruct::Execute();
            break;
        }

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            Menu::Toggle();
        }

        Aimbot::Update(g_Memory);
        SilentAim::Update(g_Memory);
        Triggerbot::Update(g_Memory);
        ESP::Update(g_Memory);

        Overlay::BeginFrame();

        Menu::BeginFrame();
        ESP::Render();
        Menu::Render();
        Menu::EndFrame();

        Overlay::EndFrame();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    Menu::Shutdown();
    Overlay::Shutdown();
    g_Memory.Detach();
    return 0;
}
