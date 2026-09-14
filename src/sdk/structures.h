#pragma once
#include <cstdint>
#include <string>

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& o) const { return { x + o.x, y + o.y, z + o.z }; }
    Vector3 operator-(const Vector3& o) const { return { x - o.x, y - o.y, z - o.z }; }
    Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }

    float Length() const { return sqrtf(x * x + y * y + z * z); }
    Vector3 Normalized() const {
        float len = Length();
        if (len < 0.0001f) return {};
        return { x / len, y / len, z / len };
    }
};

struct Vector2 {
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

struct PlayerData {
    uintptr_t address = 0;
    uintptr_t character = 0;
    uintptr_t humanoid = 0;
    uintptr_t rootPart = 0;
    uintptr_t head = 0;
    std::string name;
    float health = 0.f;
    float maxHealth = 0.f;
    Vector3 position;
    Vector3 velocity;
    bool isTeammate = false;
    bool isValid = false;
};
