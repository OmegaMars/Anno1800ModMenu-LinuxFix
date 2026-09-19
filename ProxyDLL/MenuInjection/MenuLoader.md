# MenuLoader

## Purpose

Waits in the game process for Direct3D modules, then loads the matching ImGui menu DLL.

## Flow

1. Polls `d3d11.dll` / `d3d12.dll` (does not wait for `d3d9`).
2. Short settle delay.
3. Selection via `GraphicsBackendSelector`.
4. `LoadLibraryW` of the menu DLL; on failure → debug output.
