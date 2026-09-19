#include "MenuLoader.h"

#include "../GraphicsBackend/GraphicsBackendSelector.h"

const wchar_t* MenuLoader::ResolveMenuPath(int backend_value) const {
    using Backend = GraphicsBackendSelector::Backend;
    if (backend_value == static_cast<int>(Backend::Dx12)) {
        return L"Anno1800ModMenuDX12.dll";
    }
    if (backend_value == static_cast<int>(Backend::Dx11)) {
        return L"Anno1800ModMenuDX11.dll";
    }
    return nullptr;
}

HMODULE MenuLoader::LoadWhenReady(DWORD poll_ms, DWORD settle_ms) const {
    for (;;) {
        const bool has_d3d11 = GetModuleHandleW(L"d3d11.dll") != nullptr;
        const bool has_d3d12 = GetModuleHandleW(L"d3d12.dll") != nullptr;
        if (GraphicsBackendSelector::IsReady(has_d3d11, has_d3d12)) {
            break;
        }
        Sleep(poll_ms);
    }

    Sleep(settle_ms);

    const bool has_d3d11 = GetModuleHandleW(L"d3d11.dll") != nullptr;
    const bool has_d3d12 = GetModuleHandleW(L"d3d12.dll") != nullptr;
    const bool has_d3d11_on_12 = GetModuleHandleW(L"d3d11on12.dll") != nullptr;
    const auto backend = GraphicsBackendSelector::Select(
        has_d3d11,
        has_d3d12,
        has_d3d11_on_12
    );
    const wchar_t* path = ResolveMenuPath(static_cast<int>(backend));
    if (path == nullptr) {
        OutputDebugStringW(L"MenuLoader: no graphics backend selected\n");
        return nullptr;
    }

    HMODULE module = LoadLibraryW(path);
    if (module == nullptr) {
        OutputDebugStringW(L"MenuLoader: LoadLibraryW failed for menu DLL\n");
    }
    return module;
}
