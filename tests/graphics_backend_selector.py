from __future__ import annotations

from enum import Enum


class Backend(Enum):
    """Graphics backend chosen for menu DLL loading."""

    NONE = 0
    DX11 = 1
    DX12 = 2


class GraphicsBackendSelector:
    """Selects which Direct3D menu DLL should be injected."""

    def is_ready(self, has_d3d11: bool, has_d3d12: bool) -> bool:
        """
        Return whether graphics modules are ready for menu injection.

        Args:
            has_d3d11: True if d3d11.dll is loaded.
            has_d3d12: True if d3d12.dll is loaded.

        Returns:
            True when at least one Direct3D runtime module is present.
        """
        return bool(has_d3d11 or has_d3d12)

    def select(
        self,
        has_d3d11: bool,
        has_d3d12: bool,
        has_d3d11_on_12: bool,
    ) -> Backend:
        """
        Choose the menu backend based on loaded modules.

        Args:
            has_d3d11: True if d3d11.dll is loaded.
            has_d3d12: True if d3d12.dll is loaded.
            has_d3d11_on_12: True if d3d11on12.dll is loaded.

        Returns:
            DX12 when d3d11on12 is present, otherwise DX11 when ready, else NONE.
        """
        if not self.is_ready(has_d3d11, has_d3d12):
            return Backend.NONE
        if has_d3d11_on_12:
            return Backend.DX12
        return Backend.DX11

    def resolve_menu_path(self, backend: Backend) -> str | None:
        """
        Resolve the menu DLL file name for a backend.

        Args:
            backend: Selected graphics backend.

        Returns:
            Menu DLL file name, or None if no backend.
        """
        if backend is Backend.DX12:
            return "Anno1800ModMenuDX12.dll"
        if backend is Backend.DX11:
            return "Anno1800ModMenuDX11.dll"
        return None
