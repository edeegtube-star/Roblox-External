#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace Menu {
    bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* ctx);
    void Shutdown();
    void BeginFrame();
    void Render();
    void EndFrame();
    void Toggle();
    bool IsOpen();
    // Returns true if ImGui consumed the message
    bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}
