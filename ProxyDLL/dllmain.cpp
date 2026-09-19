#include <windows.h>

#include "SystemLibrary/SystemLibraryLoader.h"

HMODULE g_real_version_module = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    (void)lpReserved;

    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        // Do not CreateThread here (Wine loader-lock). Menu injection starts
        // lazily from the first forwarded version.dll export instead.
        SystemLibraryLoader system_loader;
        g_real_version_module = system_loader.LoadFromModuleDirectory(
            hModule,
            L"version_orig.dll"
        );
        if (g_real_version_module == nullptr
            || g_real_version_module == hModule) {
            OutputDebugStringW(
                L"DllMain: version_orig.dll failed, trying system version.dll\n"
            );
            g_real_version_module = system_loader.LoadFromSystemDirectory(
                L"version.dll"
            );
        }
        if (g_real_version_module == nullptr
            || g_real_version_module == hModule) {
            OutputDebugStringW(
                L"DllMain: real version library unavailable; continuing\n"
            );
            g_real_version_module = nullptr;
        }

        // Always succeed so a failed inject cannot prevent the game from starting.
        return TRUE;
    }

    return TRUE;
}
