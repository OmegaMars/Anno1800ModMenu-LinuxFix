#pragma once

#include <windows.h>

/**
 * @file MenuLoader.h
 * @brief Loads the ImGui menu DLL after graphics runtimes are available.
 */
class MenuLoader {
public:
    /**
     * Waits for Direct3D modules, then loads the matching menu DLL.
     *
     * Args:
     *     poll_ms: Sleep duration between readiness checks in milliseconds.
     *     settle_ms: Extra delay after ready before LoadLibrary.
     *
     * Returns:
     *     Module handle of the loaded menu DLL, or NULL on failure.
     */
    HMODULE LoadWhenReady(DWORD poll_ms = 50, DWORD settle_ms = 1000) const;

private:
    /**
     * Resolves the menu DLL file name for the selected backend.
     *
     * Args:
     *     backend_value: Integer value of GraphicsBackendSelector::Backend.
     *
     * Returns:
     *     Wide path/file name of the menu DLL, or nullptr if none.
     */
    const wchar_t* ResolveMenuPath(int backend_value) const;
};
