#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct HWND__;
typedef HWND__* HWND;

namespace Menu {
    bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();
    void Toggle();
    bool IsOpen();
    void BeginFrame();
    void Render();
    void EndFrame();
    void NotifyWndProc(HWND hwnd, unsigned int msg, unsigned long long wParam, long long lParam);
}
