from __future__ import annotations

import re
import subprocess
from pathlib import Path

import hypothesis.strategies as st
import pytest
from hypothesis import given

from graphics_backend_selector import Backend, GraphicsBackendSelector


@pytest.fixture
def selector() -> GraphicsBackendSelector:
    """Provide a GraphicsBackendSelector instance."""
    return GraphicsBackendSelector()


@pytest.mark.parametrize(
    ("has_d3d11", "has_d3d12", "expected"),
    [
        (False, False, False),
        (True, False, True),
        (False, True, True),
        (True, True, True),
    ],
)
def test_is_ready(
    selector: GraphicsBackendSelector,
    has_d3d11: bool,
    has_d3d12: bool,
    expected: bool,
) -> None:
    """is_ready mirrors presence of any Direct3D module."""
    assert selector.is_ready(has_d3d11, has_d3d12) is expected


@pytest.mark.parametrize(
    ("has_d3d11", "has_d3d12", "has_d3d11_on_12", "expected"),
    [
        (False, False, False, Backend.NONE),
        (False, False, True, Backend.NONE),
        (True, False, False, Backend.DX11),
        (False, True, False, Backend.DX11),
        (True, True, False, Backend.DX11),
        (True, False, True, Backend.DX12),
        (False, True, True, Backend.DX12),
        (True, True, True, Backend.DX12),
    ],
)
def test_select(
    selector: GraphicsBackendSelector,
    has_d3d11: bool,
    has_d3d12: bool,
    has_d3d11_on_12: bool,
    expected: Backend,
) -> None:
    """select prefers DX12 only when d3d11on12 is loaded."""
    assert (
        selector.select(has_d3d11, has_d3d12, has_d3d11_on_12) is expected
    )


@pytest.mark.parametrize(
    ("backend", "expected"),
    [
        (Backend.NONE, None),
        (Backend.DX11, "Anno1800ModMenuDX11.dll"),
        (Backend.DX12, "Anno1800ModMenuDX12.dll"),
    ],
)
def test_resolve_menu_path(
    selector: GraphicsBackendSelector,
    backend: Backend,
    expected: str | None,
) -> None:
    """resolve_menu_path maps backend enum to DLL file names."""
    assert selector.resolve_menu_path(backend) == expected


@given(
    has_d3d11=st.booleans(),
    has_d3d12=st.booleans(),
    has_d3d11_on_12=st.booleans(),
)
def test_select_never_returns_dx12_without_bridge(
    has_d3d11: bool,
    has_d3d12: bool,
    has_d3d11_on_12: bool,
) -> None:
    """DX12 backend requires the d3d11on12 bridge module."""
    selector = GraphicsBackendSelector()
    backend = selector.select(has_d3d11, has_d3d12, has_d3d11_on_12)
    if backend is Backend.DX12:
        assert has_d3d11_on_12
        assert has_d3d11 or has_d3d12


def _pe_export_names(path: Path) -> list[str]:
    """
    Read exported symbol names from a PE DLL via mingw objdump.

    Args:
        path: Path to the PE DLL file.

    Returns:
        List of exported symbol names.
    """
    result = subprocess.run(
        ["x86_64-w64-mingw32-objdump", "-p", str(path)],
        check=True,
        capture_output=True,
        text=True,
    )
    names: list[str] = []
    in_export_names = False
    for line in result.stdout.splitlines():
        if "[Ordinal/Name Pointer] Table" in line:
            in_export_names = True
            continue
        if in_export_names:
            if not line.strip():
                break
            match = re.search(r"\]\s+([A-Za-z0-9_]+)\s*$", line)
            if match:
                names.append(match.group(1))
    return names


REQUIRED_EXPORTS = {
    "GetFileVersionInfoA",
    "GetFileVersionInfoW",
    "GetFileVersionInfoExA",
    "GetFileVersionInfoExW",
    "GetFileVersionInfoSizeA",
    "GetFileVersionInfoSizeW",
    "GetFileVersionInfoSizeExA",
    "GetFileVersionInfoSizeExW",
    "VerQueryValueA",
    "VerQueryValueW",
    "VerFindFileA",
    "VerFindFileW",
    "VerInstallFileA",
    "VerInstallFileW",
    "VerLanguageNameA",
    "VerLanguageNameW",
}


def test_built_version_dll_exports() -> None:
    """Built version.dll must export the full version API set."""
    dll_path = Path(__file__).resolve().parents[1] / "Build" / "version.dll"
    assert dll_path.is_file(), f"missing built DLL: {dll_path}"
    exports = set(_pe_export_names(dll_path))
    missing = REQUIRED_EXPORTS - exports
    assert not missing, f"missing exports: {sorted(missing)}"
