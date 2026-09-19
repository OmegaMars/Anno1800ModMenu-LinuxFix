# GraphicsBackendSelector

## Purpose

Pure decision logic: when graphics runtimes are ready, and which menu DLL to load.

## Behavior

- Ready as soon as `d3d11.dll` **or** `d3d12.dll` is loaded.
- Backend **DX12** only if `d3d11on12.dll` is also present.
- Otherwise **DX11** when ready.
- Otherwise no backend.
