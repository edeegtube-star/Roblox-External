#include "memory.h"
#include <psapi.h>

bool Memory::Attach(const wchar_t* processName) {
    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return false;

    if (Process32FirstW(snap, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, processName) == 0) {
                pid = entry.th32ProcessID;
                process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
                break;
            }
        } while (Process32NextW(snap, &entry));
    }
    CloseHandle(snap);

    if (!process) return false;
    base = GetModuleBase(processName);
    return base != 0;
}

void Memory::Detach() {
    if (process) {
        CloseHandle(process);
        process = nullptr;
    }
    base = 0;
    pid = 0;
}

uintptr_t Memory::GetModuleBase(const wchar_t* moduleName) {
    HMODULE mods[1024];
    DWORD needed = 0;
    if (EnumProcessModules(process, mods, sizeof(mods), &needed)) {
        for (unsigned int i = 0; i < (needed / sizeof(HMODULE)); ++i) {
            wchar_t name[MAX_PATH];
            if (GetModuleBaseNameW(process, mods[i], name, MAX_PATH)) {
                if (_wcsicmp(name, moduleName) == 0)
                    return reinterpret_cast<uintptr_t>(mods[i]);
            }
        }
    }
    return 0;
}

std::string Memory::ReadString(uintptr_t address, size_t maxLen) {
    char buffer[256]{};
    if (maxLen > 255) maxLen = 255;
    ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), buffer, maxLen, nullptr);
    return std::string(buffer);
}
