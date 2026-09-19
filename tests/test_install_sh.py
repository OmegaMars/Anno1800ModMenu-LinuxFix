from __future__ import annotations

import os
import subprocess
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[1]
INSTALL_SH = REPO_ROOT / "install.sh"


def _run_install(args: list[str], env: dict[str, str] | None = None) -> subprocess.CompletedProcess[str]:
    """
    Run install.sh and return the completed process.

    Args:
        args: Arguments after install.sh.
        env: Optional environment overlay.

    Returns:
        CompletedProcess with text output.
    """
    full_env = os.environ.copy()
    if env:
        full_env.update(env)
    return subprocess.run(
        ["bash", str(INSTALL_SH), *args],
        cwd=str(REPO_ROOT),
        env=full_env,
        capture_output=True,
        text=True,
        check=False,
    )


def test_install_sh_requires_game_dir() -> None:
    """install.sh exits with error when --game-dir is missing."""
    result = _run_install([])
    assert result.returncode != 0
    assert "game-dir" in (result.stderr + result.stdout).lower()


def test_install_sh_dry_run(
    tmp_path: Path,
) -> None:
    """install.sh --dry-run copies nothing but resolves Proton version.dll."""
    game_dir = tmp_path / "Win64"
    game_dir.mkdir()
    (game_dir / "Anno1800.exe").write_bytes(b"MZ")

    proton = tmp_path / "Proton Hotfix"
    version = proton / "files" / "lib" / "wine" / "x86_64-windows" / "version.dll"
    version.parent.mkdir(parents=True)
    version.write_bytes(b"orig")

    proxy = tmp_path / "version.dll"
    proxy.write_bytes(b"proxy")

    result = _run_install(
        [
            "--game-dir",
            str(game_dir),
            "--proton",
            str(proton),
            "--proxy",
            str(proxy),
            "--dry-run",
        ]
    )
    assert result.returncode == 0, result.stderr
    assert "WINEDLLOVERRIDES=version=n,b" in result.stdout
    assert not (game_dir / "version.dll").exists()
    assert not (game_dir / "version_orig.dll").exists()


def test_install_sh_copies_files(tmp_path: Path) -> None:
    """install.sh copies proxy and version_orig into the game directory."""
    game_dir = tmp_path / "Win64"
    game_dir.mkdir()
    (game_dir / "Anno1800.exe").write_bytes(b"MZ")
    (game_dir / "Anno1800ModMenuDX11.dll").write_bytes(b"menu")

    proton = tmp_path / "Proton Hotfix"
    version = proton / "files" / "lib" / "wine" / "x86_64-windows" / "version.dll"
    version.parent.mkdir(parents=True)
    version.write_bytes(b"orig")

    proxy = tmp_path / "version.dll"
    proxy.write_bytes(b"proxy")

    result = _run_install(
        [
            "--game-dir",
            str(game_dir),
            "--proton",
            str(proton),
            "--proxy",
            str(proxy),
        ]
    )
    assert result.returncode == 0, result.stderr
    assert (game_dir / "version.dll").read_bytes() == b"proxy"
    assert (game_dir / "version_orig.dll").read_bytes() == b"orig"
