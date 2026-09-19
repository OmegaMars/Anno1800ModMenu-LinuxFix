#pragma once

/**
 * @file GraphicsBackendSelector.h
 * @brief Pure selection logic for which graphics menu DLL to load.
 */

class GraphicsBackendSelector {
public:
    enum class Backend {
        None = 0,
        Dx11 = 1,
        Dx12 = 2,
    };

    /**
     * Returns whether graphics modules are ready for menu injection.
     *
     * Args:
     *     has_d3d11: True if d3d11.dll is loaded in the process.
     *     has_d3d12: True if d3d12.dll is loaded in the process.
     *
     * Returns:
     *     True when at least one Direct3D runtime module is present.
     */
    static bool IsReady(bool has_d3d11, bool has_d3d12);

    /**
     * Chooses the menu backend DLL based on loaded modules.
     *
     * Args:
     *     has_d3d11: True if d3d11.dll is loaded.
     *     has_d3d12: True if d3d12.dll is loaded.
     *     has_d3d11_on_12: True if d3d11on12.dll is loaded.
     *
     * Returns:
     *     Dx12 when d3d11on12 is present, otherwise Dx11 when ready, else None.
     */
    static Backend Select(bool has_d3d11, bool has_d3d12, bool has_d3d11_on_12);
};
