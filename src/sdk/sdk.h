#pragma once
#include "../memory/memory.h"
#include "offsets.h"
#include "structures.h"
#include <string>
#include <vector>

namespace SDK {

struct Matrix4 {
    float m[4][4];
};

class Roblox {
public:
    Memory* mem = nullptr;
    uintptr_t moduleBase = 0;
    uintptr_t dataModel = 0;
    uintptr_t visualEngine = 0;
    uintptr_t workspace = 0;
    uintptr_t players = 0;
    uintptr_t localPlayer = 0;
    Matrix4 viewMatrix{};
    int screenW = 0;
    int screenH = 0;

    bool Init(Memory* memory);
    bool Refresh();

    uintptr_t ReadPointer(uintptr_t address);
    std::string ReadRobloxString(uintptr_t nameContainer);
    std::vector<uintptr_t> GetChildren(uintptr_t instance);
    uintptr_t FindFirstChild(uintptr_t parent, const std::string& name);
    uintptr_t FindFirstChildOfClass(uintptr_t parent, const std::string& className);

    Vector3 GetPartPosition(uintptr_t part);
    Vector3 GetPartSize(uintptr_t part);
    bool WorldToScreen(const Vector3& world, Vector2& out);

    std::vector<PlayerData> GetPlayers();
};

} // namespace SDK
