#include "SystemLibraryLoader.h"

#include <stdio.h>

HMODULE SystemLibraryLoader::LoadFromModuleDirectory(
    HMODULE self_module,
    const wchar_t* file_name
) const {
    if (file_name == nullptr || file_name[0] == L'\0') {
        OutputDebugStringW(L"SystemLibraryLoader: empty file name\n");
        return nullptr;
    }

    wchar_t module_path[MAX_PATH];
    const DWORD module_length = GetModuleFileNameW(
        self_module,
        module_path,
        MAX_PATH
    );
    if (module_length == 0 || module_length >= MAX_PATH) {
        OutputDebugStringW(L"SystemLibraryLoader: GetModuleFileNameW failed\n");
        return nullptr;
    }

    wchar_t* last_slash = wcsrchr(module_path, L'\\');
    if (last_slash == nullptr) {
        last_slash = wcsrchr(module_path, L'/');
    }
    if (last_slash == nullptr) {
        OutputDebugStringW(L"SystemLibraryLoader: module path has no directory\n");
        return nullptr;
    }
    *(last_slash + 1) = L'\0';

    wchar_t path[MAX_PATH];
    if (swprintf(path, MAX_PATH, L"%s%s", module_path, file_name) < 0) {
        OutputDebugStringW(L"SystemLibraryLoader: path formatting failed\n");
        return nullptr;
    }

    HMODULE module = LoadLibraryW(path);
    if (module == nullptr) {
        OutputDebugStringW(L"SystemLibraryLoader: LoadLibraryW (module dir) failed\n");
    }
    return module;
}

HMODULE SystemLibraryLoader::LoadFromSystemDirectory(const wchar_t* file_name) const {
    if (file_name == nullptr || file_name[0] == L'\0') {
        OutputDebugStringW(L"SystemLibraryLoader: empty file name\n");
        return nullptr;
    }

    wchar_t system_dir[MAX_PATH];
    const UINT length = GetSystemDirectoryW(system_dir, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        OutputDebugStringW(L"SystemLibraryLoader: GetSystemDirectoryW failed\n");
        return nullptr;
    }

    wchar_t path[MAX_PATH];
    if (swprintf(path, MAX_PATH, L"%s\\%s", system_dir, file_name) < 0) {
        OutputDebugStringW(L"SystemLibraryLoader: path formatting failed\n");
        return nullptr;
    }

    HMODULE module = LoadLibraryExW(
        path,
        nullptr,
        LOAD_LIBRARY_SEARCH_SYSTEM32
    );
    if (module == nullptr) {
        module = LoadLibraryW(path);
    }
    if (module == nullptr) {
        OutputDebugStringW(L"SystemLibraryLoader: LoadLibraryW (system) failed\n");
    }
    return module;
}
