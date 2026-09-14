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

    // Menu needs device/context from overlay - expose them next pass if needed
    // For now Initialize is called with nulls until overlay exposes D3D objects
    // Menu::Initialize(Overlay::GetHwnd(), device, context);

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
