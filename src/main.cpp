#include <Windows.h>
#include <thread>
#include <chrono>

#include "memory/memory.h"
#include "sdk/sdk.h"
#include "features/aimbot.h"
#include "features/esp.h"
#include "features/triggerbot.h"
#include "features/silentaim.h"
#include "features/movement/movement.h"
#include "render/overlay.h"
#include "menu/menu.h"
#include "stealth/streamproof.h"
#include "stealth/selfdestruct.h"
#include "sdk/structures.h"

Memory g_Memory;
SDK::Roblox g_SDK;
bool g_Running = true;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    while (!g_Memory.Attach(L"RobloxPlayerBeta.exe")) {
        if (GetAsyncKeyState(VK_END) & 1) return 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    g_SDK.Init(&g_Memory);

    if (!Overlay::Initialize()) {
        g_Memory.Detach();
        return 1;
    }

    Streamproof::Enable(Overlay::GetHwnd());
    Globals::streamproof = true;

    if (!Menu::Initialize(Overlay::GetHwnd(), Overlay::GetDevice(), Overlay::GetContext())) {
        Overlay::Shutdown();
        g_Memory.Detach();
        return 1;
    }

    while (g_Running) {
        if (GetAsyncKeyState(VK_END) & 1) {
            SelfDestruct::Execute();
            break;
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
            Menu::Toggle();

        Overlay::SetClickThrough(!Menu::IsOpen());

        if (!g_Memory.process) {
            g_Memory.Attach(L"RobloxPlayerBeta.exe");
            if (g_Memory.process) g_SDK.Init(&g_Memory);
        }

        g_SDK.Refresh();

        ESP::Update(g_Memory);
        Aimbot::Update(g_Memory);
        SilentAim::Update(g_Memory);
        Triggerbot::Update(g_Memory);
        Movement::Update(g_Memory);

        Overlay::BeginFrame();
        Menu::BeginFrame();
        ESP::Render();
        Menu::Render();
        Menu::EndFrame();
        Overlay::EndFrame();

        int sleepMs = Globals::performanceMode ? 2 : 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
    }

    Menu::Shutdown();
    Overlay::Shutdown();
    g_Memory.Detach();
    return 0;
}
