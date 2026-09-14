#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <cstdint>

class Memory {
public:
    HANDLE process = nullptr;
    uintptr_t base = 0;
    DWORD pid = 0;

    bool Attach(const wchar_t* processName);
    void Detach();

    template<typename T>
    T Read(uintptr_t address) {
        T buffer{};
        if (process && address)
            ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr);
        return buffer;
    }

    template<typename T>
    bool Write(uintptr_t address, const T& value) {
        if (!process || !address) return false;
        return WriteProcessMemory(process, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
    }

    std::string ReadString(uintptr_t address, size_t maxLen = 64);
    uintptr_t GetModuleBase(const wchar_t* moduleName);
};
