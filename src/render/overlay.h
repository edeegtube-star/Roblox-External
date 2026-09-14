#pragma once
#include <Windows.h>
#include <d3d11.h>

namespace Overlay {
    bool Initialize();
    void Shutdown();
    void BeginFrame();
    void EndFrame();
    HWND GetHwnd();
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();

    void DrawLine(float x1, float y1, float x2, float y2, unsigned int color, float thickness = 1.0f);
    void DrawBox(float x, float y, float w, float h, unsigned int color, float thickness = 1.0f);
    void DrawCornerBox(float x, float y, float w, float h, unsigned int color, float thickness = 1.0f);
    void DrawText(float x, float y, const char* text, unsigned int color);
}
