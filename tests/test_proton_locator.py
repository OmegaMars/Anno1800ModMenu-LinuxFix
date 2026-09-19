from __future__ import annotations

from pathlib import Path

import hypothesis.strategies as st
import pytest
from hypothesis import given

from install_support.proton_locator import ProtonVersionLocator, default_proxy_path


@pytest.fixture
def locator() -> ProtonVersionLocator:
    """Provide a ProtonVersionLocator instance."""
    return ProtonVersionLocator()


def test_default_proxy_path_points_at_build() -> None:
    """default_proxy_path ends with Build/version.dll."""
    path = default_proxy_path()
    assert path.name == "version.dll"
    assert path.parent.name == "Build"


def test_validate_game_dir_requires_exe(
    locator: ProtonVersionLocator,
    tmp_path: Path,
) -> None:
    """validate_game_dir rejects directories without Anno1800.exe."""
    with pytest.raises(FileNotFoundError):
        locator.validate_game_dir(tmp_path)


def test_validate_game_dir_ok(
    locator: ProtonVersionLocator,
    tmp_path: Path,
) -> None:
    """validate_game_dir accepts a directory that contains Anno1800.exe."""
    (tmp_path / "Anno1800.exe").write_bytes(b"MZ")
    assert locator.validate_game_dir(tmp_path) == tmp_path.resolve()


def test_resolve_version_dll_explicit_proton(
    locator: ProtonVersionLocator,
    tmp_path: Path,
) -> None:
    """resolve_version_dll finds version.dll under an explicit Proton root."""
    version = (
        tmp_path
        / "files"
        / "lib"
        / "wine"
        / "x86_64-windows"
        / "version.dll"
    )
    version.parent.mkdir(parents=True)
    version.write_bytes(b"fake")
    found = locator.resolve_version_dll(proton_root=tmp_path)
    assert found == version.resolve()


def test_resolve_version_dll_searches_steam_roots(
    locator: ProtonVersionLocator,
    tmp_path: Path,
) -> None:
    """resolve_version_dll scans Steam common for Proton* folders."""
    proton = tmp_path / "steamapps" / "common" / "Proton Hotfix"
    version = proton / "files" / "lib" / "wine" / "x86_64-windows" / "version.dll"
    version.parent.mkdir(parents=True)
    version.write_bytes(b"fake")
    found = locator.resolve_version_dll(steam_roots=[tmp_path])
    assert found == version.resolve()


def test_install_copies_files(
    locator: ProtonVersionLocator,
    tmp_path: Path,
) -> None:
    """install copies proxy and version_orig into the game directory."""
    game_dir = tmp_path / "Win64"
    game_dir.mkdir()
    (game_dir / "Anno1800.exe").write_bytes(b"MZ")
    (game_dir / "Anno1800ModMenuDX11.dll").write_bytes(b"menu")

    proton = tmp_path / "Proton"
    source = proton / "files" / "lib" / "wine" / "x86_64-windows" / "version.dll"
    source.parent.mkdir(parents=True)
    source.write_bytes(b"orig")

    proxy = tmp_path / "proxy-version.dll"
    proxy.write_bytes(b"proxy")

    result = locator.install(
        game_dir=game_dir,
        proxy_dll=proxy,
        proton_root=proton,
        dry_run=False,
    )
    assert (game_dir / "version.dll").read_bytes() == b"proxy"
    assert (game_dir / "version_orig.dll").read_bytes() == b"orig"
    assert result["menu_dx11_present"] == "True"
    assert result["override"] == "WINEDLLOVERRIDES=version=n,b"


def test_install_dry_run_does_not_copy(
    locator: ProtonVersionLocator,
    tmp_path: Path,
) -> None:
    """install dry_run leaves the game directory unchanged."""
    game_dir = tmp_path / "Win64"
    game_dir.mkdir()
    (game_dir / "Anno1800.exe").write_bytes(b"MZ")

    proton = tmp_path / "Proton"
    source = proton / "files" / "lib" / "wine" / "x86_64-windows" / "version.dll"
    source.parent.mkdir(parents=True)
    source.write_bytes(b"orig")

    proxy = tmp_path / "proxy-version.dll"
    proxy.write_bytes(b"proxy")

    locator.install(
        game_dir=game_dir,
        proxy_dll=proxy,
        proton_root=proton,
        dry_run=True,
    )
    assert not (game_dir / "version.dll").exists()
    assert not (game_dir / "version_orig.dll").exists()


@given(name=st.sampled_from(["Proton 9.0", "Proton Hotfix", "GE-Proton9-20"]))
def test_proton_name_heuristic(name: str) -> None:
    """Proton folder names used for discovery contain 'proton' case-insensitively."""
    assert "proton" in name.lower()
