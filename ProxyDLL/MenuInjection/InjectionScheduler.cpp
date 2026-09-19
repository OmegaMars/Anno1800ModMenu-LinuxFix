#include "InjectionScheduler.h"

#include "MenuLoader.h"

#include <stdio.h>

void InjectionScheduler::AppendLog(const char* message) {
    if (message == nullptr) {
        return;
    }

    HMODULE self = nullptr;
    if (!GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
                | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&InjectionScheduler::AppendLog),
            &self
        )) {
        return;
    }

    wchar_t module_path[MAX_PATH];
    const DWORD length = GetModuleFileNameW(self, module_path, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return;
    }

    wchar_t* slash = wcsrchr(module_path, L'\\');
    if (slash == nullptr) {
        slash = wcsrchr(module_path, L'/');
    }
    if (slash == nullptr) {
        return;
    }
    *(slash + 1) = L'\0';

    wchar_t log_path[MAX_PATH];
    if (swprintf(log_path, MAX_PATH, L"%sinject.log", module_path) < 0) {
        return;
    }

    FILE* file = _wfopen(log_path, L"a");
    if (file == nullptr) {
        return;
    }
    fputs(message, file);
    fputc('\n', file);
    fclose(file);
}

DWORD WINAPI InjectionScheduler::Worker(LPVOID lpParameter) {
    (void)lpParameter;
    AppendLog("InjectionScheduler: worker started");
    MenuLoader loader;
    HMODULE menu = loader.LoadWhenReady();
    if (menu == nullptr) {
        AppendLog("InjectionScheduler: menu LoadLibrary failed");
        OutputDebugStringA("InjectionScheduler: menu LoadLibrary failed\n");
    } else {
        AppendLog("InjectionScheduler: menu DLL loaded");
    }
    return 0;
}

void InjectionScheduler::RequestStart() {
    static LONG started = 0;
    if (InterlockedCompareExchange(&started, 1, 0) != 0) {
        return;
    }

    HANDLE thread = CreateThread(nullptr, 0, Worker, nullptr, 0, nullptr);
    if (thread == nullptr) {
        AppendLog("InjectionScheduler: CreateThread failed");
        OutputDebugStringA("InjectionScheduler: CreateThread failed\n");
        InterlockedExchange(&started, 0);
        return;
    }
    CloseHandle(thread);
}
