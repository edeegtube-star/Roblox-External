#include "overlay.h"
#include <dwmapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwmapi.lib")

namespace Overlay {

static HWND hwnd = nullptr;
static ID3D11Device* device = nullptr;
static ID3D11DeviceContext* context = nullptr;
static IDXGISwapChain* swapChain = nullptr;
static ID3D11RenderTargetView* rtv = nullptr;

bool Initialize() {
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "UberDeliveryOverlay";
    RegisterClassExA(&wc);

    hwnd = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        wc.lpszClassName, "", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, wc.hInstance, nullptr
    );

    if (!hwnd) return false;

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL level;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &sd, &swapChain, &device, &level, &context
    );
    if (FAILED(hr)) return false;

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
    if (backBuffer) backBuffer->Release();

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

void Shutdown() {
    if (rtv) { rtv->Release(); rtv = nullptr; }
    if (swapChain) { swapChain->Release(); swapChain = nullptr; }
    if (context) { context->Release(); context = nullptr; }
    if (device) { device->Release(); device = nullptr; }
    if (hwnd) { DestroyWindow(hwnd); hwnd = nullptr; }
}

void BeginFrame() {
    float clear[4] = { 0.f, 0.f, 0.f, 0.f };
    context->OMSetRenderTargets(1, &rtv, nullptr);
    context->ClearRenderTargetView(rtv, clear);
}

void EndFrame() {
    swapChain->Present(0, 0);
}

HWND GetHwnd() { return hwnd; }
ID3D11Device* GetDevice() { return device; }
ID3D11DeviceContext* GetContext() { return context; }

void DrawLine(float, float, float, float, unsigned int, float) {}
void DrawBox(float, float, float, float, unsigned int, float) {}
void DrawCornerBox(float, float, float, float, unsigned int, float) {}
void DrawText(float, float, const char*, unsigned int) {}

} // namespace Overlay
