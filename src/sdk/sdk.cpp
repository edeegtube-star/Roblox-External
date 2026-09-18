#include "sdk.h"
#include <cmath>

namespace SDK {

bool Roblox::Init(Memory* memory) {
    mem = memory;
    if (!mem || !mem->base) return false;
    moduleBase = mem->base;
    return Refresh();
}

uintptr_t Roblox::ReadPointer(uintptr_t address) {
    return mem->Read<uintptr_t>(address);
}

bool Roblox::Refresh() {
    if (!mem) return false;
    visualEngine = ReadPointer(moduleBase + Offsets::VisualEngine_Pointer);
    if (!visualEngine)
        visualEngine = ReadPointer(Offsets::VisualEngine_Pointer);
    uintptr_t fakeDM = ReadPointer(moduleBase + Offsets::FakeDataModel_Pointer);
    if (!fakeDM)
        fakeDM = ReadPointer(Offsets::FakeDataModel_Pointer);
    if (fakeDM)
        dataModel = ReadPointer(fakeDM + Offsets::FakeDataModel_RealDataModel);
    if (!dataModel && visualEngine) {
        uintptr_t veFake = ReadPointer(visualEngine + Offsets::VisualEngine_FakeDataModel);
        if (veFake)
            dataModel = ReadPointer(veFake + Offsets::FakeDataModel_RealDataModel);
    }
    if (!dataModel) return false;
    workspace = ReadPointer(dataModel + Offsets::DataModel_Workspace);
    players = FindFirstChildOfClass(dataModel, "Players");
    if (players)
        localPlayer = ReadPointer(players + Offsets::Player_LocalPlayer);
    if (visualEngine) {
        viewMatrix = mem->Read<Matrix4>(visualEngine + Offsets::VisualEngine_ViewMatrix);
        struct { float x, y; } dims = mem->Read<decltype(dims)>(visualEngine + Offsets::VisualEngine_Dimensions);
        screenW = (int)dims.x;
        screenH = (int)dims.y;
    }
    if (screenW <= 0) screenW = GetSystemMetrics(SM_CXSCREEN);
    if (screenH <= 0) screenH = GetSystemMetrics(SM_CYSCREEN);
    return dataModel != 0;
}

std::string Roblox::ReadRobloxString(uintptr_t strStruct) {
    if (!strStruct) return {};
    int len = mem->Read<int>(strStruct + Offsets::Misc_StringLength);
    if (len <= 0 || len > 256) return {};
    if (len >= 16) {
        uintptr_t ptr = mem->Read<uintptr_t>(strStruct);
        return mem->ReadString(ptr, len + 1);
    }
    return mem->ReadString(strStruct, len + 1);
}

std::vector<uintptr_t> Roblox::GetChildren(uintptr_t instance) {
    std::vector<uintptr_t> out;
    if (!instance) return out;
    uintptr_t start = ReadPointer(instance + Offsets::Instance_ChildrenStart);
    uintptr_t end = ReadPointer(start + Offsets::Instance_ChildrenEnd);
    if (!start) return out;
    for (uintptr_t p = start; p < end && out.size() < 512; p += sizeof(uintptr_t)) {
        uintptr_t child = ReadPointer(p);
        if (child) out.push_back(child);
    }
    return out;
}

uintptr_t Roblox::FindFirstChild(uintptr_t parent, const std::string& name) {
    for (uintptr_t child : GetChildren(parent)) {
        uintptr_t nameCont = ReadPointer(child + Offsets::Instance_NameContainer);
        if (ReadRobloxString(nameCont == 0 ? child + Offsets::Instance_Name : nameCont) == name)
            return child;
        if (ReadRobloxString(child + Offsets::Instance_NameContainer) == name)
            return child;
    }
    return 0;
}

uintptr_t Roblox::FindFirstChildOfClass(uintptr_t parent, const std::string& className) {
    for (uintptr_t child : GetChildren(parent)) {
        uintptr_t desc = ReadPointer(child + Offsets::Instance_ClassDescriptor);
        if (!desc) continue;
        uintptr_t classNamePtr = ReadPointer(desc + Offsets::Instance_ClassName);
        if (ReadRobloxString(classNamePtr) == className)
            return child;
    }
    return 0;
}

Vector3 Roblox::GetPartPosition(uintptr_t part) {
    if (!part) return {};
    uintptr_t prim = ReadPointer(part + Offsets::BasePart_Primitive);
    if (!prim) return {};
    return mem->Read<Vector3>(prim + Offsets::Primitive_Position);
}

Vector3 Roblox::GetPartSize(uintptr_t part) {
    if (!part) return {};
    uintptr_t prim = ReadPointer(part + Offsets::BasePart_Primitive);
    if (!prim) return {};
    return mem->Read<Vector3>(prim + Offsets::Primitive_Size);
}

bool Roblox::WorldToScreen(const Vector3& world, Vector2& out) {
    float w = viewMatrix.m[3][0] * world.x + viewMatrix.m[3][1] * world.y + viewMatrix.m[3][2] * world.z + viewMatrix.m[3][3];
    if (w < 0.001f) return false;
    float x = viewMatrix.m[0][0] * world.x + viewMatrix.m[0][1] * world.y + viewMatrix.m[0][2] * world.z + viewMatrix.m[0][3];
    float y = viewMatrix.m[1][0] * world.x + viewMatrix.m[1][1] * world.y + viewMatrix.m[1][2] * world.z + viewMatrix.m[1][3];
    x /= w; y /= w;
    out.x = (screenW * 0.5f) * (1.0f + x);
    out.y = (screenH * 0.5f) * (1.0f - y);
    return true;
}

std::vector<PlayerData> Roblox::GetPlayers() {
    std::vector<PlayerData> list;
    if (!players) return list;
    for (uintptr_t plr : GetChildren(players)) {
        PlayerData pd{};
        pd.address = plr;
        uintptr_t nameCont = ReadPointer(plr + Offsets::Instance_NameContainer);
        pd.name = ReadRobloxString(nameCont ? nameCont : plr + Offsets::Instance_NameContainer);
        pd.character = ReadPointer(plr + Offsets::Player_ModelInstance);
        if (!pd.character) continue;
        pd.humanoid = FindFirstChildOfClass(pd.character, "Humanoid");
        pd.rootPart = FindFirstChild(pd.character, "HumanoidRootPart");
        pd.head = FindFirstChild(pd.character, "Head");
        pd.torso = FindFirstChild(pd.character, "UpperTorso");
        if (!pd.torso) pd.torso = FindFirstChild(pd.character, "Torso");
        pd.leftArm = FindFirstChild(pd.character, "LeftUpperArm");
        if (!pd.leftArm) pd.leftArm = FindFirstChild(pd.character, "Left Arm");
        pd.rightArm = FindFirstChild(pd.character, "RightUpperArm");
        if (!pd.rightArm) pd.rightArm = FindFirstChild(pd.character, "Right Arm");
        pd.leftLeg = FindFirstChild(pd.character, "LeftUpperLeg");
        if (!pd.leftLeg) pd.leftLeg = FindFirstChild(pd.character, "Left Leg");
        pd.rightLeg = FindFirstChild(pd.character, "RightUpperLeg");
        if (!pd.rightLeg) pd.rightLeg = FindFirstChild(pd.character, "Right Leg");
        uintptr_t dn = ReadPointer(plr + Offsets::Player_DisplayName);
        if (dn) pd.displayName = ReadRobloxString(dn);
        if (pd.displayName.empty()) pd.displayName = pd.name;
        for (uintptr_t ch : GetChildren(pd.character)) {
            uintptr_t desc = ReadPointer(ch + Offsets::Instance_ClassDescriptor);
            if (!desc) continue;
            uintptr_t cnPtr = ReadPointer(desc + Offsets::Instance_ClassName);
            if (ReadRobloxString(cnPtr) == "Tool") {
                uintptr_t nc = ReadPointer(ch + Offsets::Instance_NameContainer);
                pd.toolName = ReadRobloxString(nc ? nc : ch + Offsets::Instance_NameContainer);
                break;
            }
        }
        if (pd.humanoid) {
            pd.health = mem->Read<float>(pd.humanoid + Offsets::Humanoid_Health);
            pd.maxHealth = mem->Read<float>(pd.humanoid + Offsets::Humanoid_MaxHealth);
            if (pd.maxHealth <= 0.f) pd.maxHealth = 100.f;
        }
        if (pd.rootPart) {
            pd.position = GetPartPosition(pd.rootPart);
            uintptr_t prim = ReadPointer(pd.rootPart + Offsets::BasePart_Primitive);
            if (prim)
                pd.velocity = mem->Read<Vector3>(prim + Offsets::Primitive_AssemblyLinearVelocity);
        }
        uintptr_t team = ReadPointer(plr + Offsets::Player_Team);
        uintptr_t localTeam = localPlayer ? ReadPointer(localPlayer + Offsets::Player_Team) : 0;
        pd.isTeammate = (team && localTeam && team == localTeam);
        pd.isValid = pd.rootPart != 0;
        if (pd.isValid)
            list.push_back(pd);
    }
    return list;
}

} // namespace SDK
