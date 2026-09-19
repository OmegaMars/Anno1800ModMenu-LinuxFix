#pragma once

#include <windows.h>

/**
 * @file InjectionScheduler.h
 * @brief Starts menu injection after DllMain has finished (Wine-safe).
 */
class InjectionScheduler {
public:
    /**
     * Requests a one-shot background injection if not already started.
     *
     * Safe to call from exported DLL functions after PROCESS_ATTACH.
     * Must not be called from DllMain.
     */
    static void RequestStart();

private:
    /**
     * Thread entry that loads the ImGui menu DLL.
     *
     * Args:
     *     lpParameter: Unused.
     *
     * Returns:
     *     Always 0.
     */
    static DWORD WINAPI Worker(LPVOID lpParameter);

    /**
     * Appends a line to inject.log next to this module.
     *
     * Args:
     *     message: ASCII message without trailing newline.
     */
    static void AppendLog(const char* message);
};
