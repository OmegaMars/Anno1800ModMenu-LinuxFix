#pragma once

#include <windows.h>

/**
 * @file SystemLibraryLoader.h
 * @brief Loads a real version implementation without Wine self-recursion.
 */
class SystemLibraryLoader {
public:
    /**
     * Loads a DLL from the directory of the current module (game Bin\\Win64).
     *
     * Args:
     *     file_name: DLL file name without directory, e.g. L"version_orig.dll".
     *
     * Returns:
     *     Module handle, or NULL if loading failed.
     */
    HMODULE LoadFromModuleDirectory(
        HMODULE self_module,
        const wchar_t* file_name
    ) const;

    /**
     * Loads a DLL from the Windows system directory.
     *
     * Used as a fallback when version_orig.dll is missing.
     *
     * Args:
     *     file_name: DLL file name without directory, e.g. L"version.dll".
     *
     * Returns:
     *     Module handle, or NULL if loading failed.
     */
    HMODULE LoadFromSystemDirectory(const wchar_t* file_name) const;
};
