#include <windows.h>

#include "../MenuInjection/InjectionScheduler.h"

/**
 * @file VersionExports.cpp
 * @brief Forwards version.dll API exports to the real system library.
 */

extern HMODULE g_real_version_module;

static FARPROC ResolveExport(const char* name) {
    // First export use happens after DllMain — safe place to start injection.
    InjectionScheduler::RequestStart();

    if (g_real_version_module == nullptr) {
        OutputDebugStringA("VersionExports: real module is null\n");
        return nullptr;
    }
    FARPROC proc = GetProcAddress(g_real_version_module, name);
    if (proc == nullptr) {
        OutputDebugStringA("VersionExports: GetProcAddress failed\n");
        OutputDebugStringA(name);
        OutputDebugStringA("\n");
    }
    return proc;
}

template <typename Fn>
static Fn LoadFn(const char* name) {
    return reinterpret_cast<Fn>(reinterpret_cast<void*>(ResolveExport(name)));
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoA(
    LPCSTR a, DWORD b, DWORD c, LPVOID d
) {
    using Fn = BOOL(WINAPI*)(LPCSTR, DWORD, DWORD, LPVOID);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoA");
    }
    return fn ? fn(a, b, c, d) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoW(
    LPCWSTR a, DWORD b, DWORD c, LPVOID d
) {
    using Fn = BOOL(WINAPI*)(LPCWSTR, DWORD, DWORD, LPVOID);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoW");
    }
    return fn ? fn(a, b, c, d) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoExA(
    DWORD a, LPCSTR b, DWORD c, DWORD d, LPVOID e
) {
    using Fn = BOOL(WINAPI*)(DWORD, LPCSTR, DWORD, DWORD, LPVOID);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoExA");
    }
    return fn ? fn(a, b, c, d, e) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoExW(
    DWORD a, LPCWSTR b, DWORD c, DWORD d, LPVOID e
) {
    using Fn = BOOL(WINAPI*)(DWORD, LPCWSTR, DWORD, DWORD, LPVOID);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoExW");
    }
    return fn ? fn(a, b, c, d, e) : FALSE;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeA(
    LPCSTR a, LPDWORD b
) {
    using Fn = DWORD(WINAPI*)(LPCSTR, LPDWORD);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoSizeA");
    }
    return fn ? fn(a, b) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeW(
    LPCWSTR a, LPDWORD b
) {
    using Fn = DWORD(WINAPI*)(LPCWSTR, LPDWORD);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoSizeW");
    }
    return fn ? fn(a, b) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExA(
    DWORD a, LPCSTR b, LPDWORD c
) {
    using Fn = DWORD(WINAPI*)(DWORD, LPCSTR, LPDWORD);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoSizeExA");
    }
    return fn ? fn(a, b, c) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExW(
    DWORD a, LPCWSTR b, LPDWORD c
) {
    using Fn = DWORD(WINAPI*)(DWORD, LPCWSTR, LPDWORD);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("GetFileVersionInfoSizeExW");
    }
    return fn ? fn(a, b, c) : 0;
}

extern "C" __declspec(dllexport) BOOL WINAPI VerQueryValueA(
    LPCVOID a, LPCSTR b, LPVOID* c, PUINT d
) {
    using Fn = BOOL(WINAPI*)(LPCVOID, LPCSTR, LPVOID*, PUINT);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerQueryValueA");
    }
    return fn ? fn(a, b, c, d) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI VerQueryValueW(
    LPCVOID a, LPCWSTR b, LPVOID* c, PUINT d
) {
    using Fn = BOOL(WINAPI*)(LPCVOID, LPCWSTR, LPVOID*, PUINT);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerQueryValueW");
    }
    return fn ? fn(a, b, c, d) : FALSE;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerLanguageNameA(
    DWORD a, LPSTR b, DWORD c
) {
    using Fn = DWORD(WINAPI*)(DWORD, LPSTR, DWORD);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerLanguageNameA");
    }
    return fn ? fn(a, b, c) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerLanguageNameW(
    DWORD a, LPWSTR b, DWORD c
) {
    using Fn = DWORD(WINAPI*)(DWORD, LPWSTR, DWORD);
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerLanguageNameW");
    }
    return fn ? fn(a, b, c) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerFindFileA(
    DWORD uFlags,
    LPSTR szFileName,
    LPSTR szWinDir,
    LPSTR szAppDir,
    LPSTR szCurDir,
    PUINT puCurDirLen,
    LPSTR szDestDir,
    PUINT puDestDirLen
) {
    using Fn = DWORD(WINAPI*)(
        DWORD, LPSTR, LPSTR, LPSTR, LPSTR, PUINT, LPSTR, PUINT
    );
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerFindFileA");
    }
    if (fn == nullptr) {
        return 0;
    }
    return fn(
        uFlags,
        szFileName,
        szWinDir,
        szAppDir,
        szCurDir,
        puCurDirLen,
        szDestDir,
        puDestDirLen
    );
}

extern "C" __declspec(dllexport) DWORD WINAPI VerFindFileW(
    DWORD uFlags,
    LPWSTR szFileName,
    LPWSTR szWinDir,
    LPWSTR szAppDir,
    LPWSTR szCurDir,
    PUINT puCurDirLen,
    LPWSTR szDestDir,
    PUINT puDestDirLen
) {
    using Fn = DWORD(WINAPI*)(
        DWORD, LPWSTR, LPWSTR, LPWSTR, LPWSTR, PUINT, LPWSTR, PUINT
    );
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerFindFileW");
    }
    if (fn == nullptr) {
        return 0;
    }
    return fn(
        uFlags,
        szFileName,
        szWinDir,
        szAppDir,
        szCurDir,
        puCurDirLen,
        szDestDir,
        puDestDirLen
    );
}

extern "C" __declspec(dllexport) DWORD WINAPI VerInstallFileA(
    DWORD uFlags,
    LPSTR szSrcFileName,
    LPSTR szDestFileName,
    LPSTR szSrcDir,
    LPSTR szDestDir,
    LPSTR szCurDir,
    LPSTR szTmpFile,
    PUINT puTmpFileLen
) {
    using Fn = DWORD(WINAPI*)(
        DWORD, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, PUINT
    );
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerInstallFileA");
    }
    if (fn == nullptr) {
        return 0;
    }
    return fn(
        uFlags,
        szSrcFileName,
        szDestFileName,
        szSrcDir,
        szDestDir,
        szCurDir,
        szTmpFile,
        puTmpFileLen
    );
}

extern "C" __declspec(dllexport) DWORD WINAPI VerInstallFileW(
    DWORD uFlags,
    LPWSTR szSrcFileName,
    LPWSTR szDestFileName,
    LPWSTR szSrcDir,
    LPWSTR szDestDir,
    LPWSTR szCurDir,
    LPWSTR szTmpFile,
    PUINT puTmpFileLen
) {
    using Fn = DWORD(WINAPI*)(
        DWORD, LPWSTR, LPWSTR, LPWSTR, LPWSTR, LPWSTR, LPWSTR, PUINT
    );
    static Fn fn = nullptr;
    if (fn == nullptr) {
        fn = LoadFn<Fn>("VerInstallFileW");
    }
    if (fn == nullptr) {
        return 0;
    }
    return fn(
        uFlags,
        szSrcFileName,
        szDestFileName,
        szSrcDir,
        szDestDir,
        szCurDir,
        szTmpFile,
        puTmpFileLen
    );
}
