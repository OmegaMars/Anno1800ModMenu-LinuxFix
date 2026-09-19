from __future__ import annotations

import os
import shutil
from pathlib import Path


class ProtonVersionLocator:
    """Locates Proton/Wine PE version.dll for use as version_orig.dll."""

    DEFAULT_STEAM_ROOTS = (
        Path.home() / ".steam" / "steam",
        Path.home() / ".steam" / "debian-installation",
        Path.home() / ".local" / "share" / "Steam",
        Path.home() / ".var" / "app" / "com.valvesoftware.Steam" / "data" / "Steam",
    )

    CANDIDATE_RELATIVE_PATHS = (
        Path("files/lib/wine/x86_64-windows/version.dll"),
        Path("files/lib64/wine/x86_64-windows/version.dll"),
        Path("lib/wine/x86_64-windows/version.dll"),
        Path("lib64/wine/x86_64-windows/version.dll"),
    )

    def resolve_version_dll(
        self,
        proton_root: Path | None = None,
        steam_roots: list[Path] | None = None,
    ) -> Path | None:
        """
        Resolve a usable Wine/Proton version.dll PE path.

        Args:
            proton_root: Explicit Proton or Wine prefix tools root.
            steam_roots: Optional Steam library roots to search.

        Returns:
            Path to version.dll, or None if not found.
        """
        if proton_root is not None:
            found = self._find_under_proton_root(proton_root.expanduser())
            if found is not None:
                return found

        roots = steam_roots if steam_roots is not None else list(self.DEFAULT_STEAM_ROOTS)
        for steam_root in roots:
            steam_root = steam_root.expanduser()
            common = steam_root / "steamapps" / "common"
            if not common.is_dir():
                continue
            for entry in sorted(common.iterdir(), key=lambda p: p.name.lower()):
                if not entry.is_dir():
                    continue
                name = entry.name.lower()
                if "proton" not in name:
                    continue
                found = self._find_under_proton_root(entry)
                if found is not None:
                    return found
        return None

    def _find_under_proton_root(self, proton_root: Path) -> Path | None:
        """
        Search known relative locations under a Proton directory.

        Args:
            proton_root: Proton installation directory.

        Returns:
            Path to version.dll, or None.
        """
        if not proton_root.is_dir():
            return None
        for relative in self.CANDIDATE_RELATIVE_PATHS:
            candidate = proton_root / relative
            if candidate.is_file():
                return candidate.resolve()
        # Lutris/wine-staging style
        for pattern in (
            "**/x86_64-windows/version.dll",
            "**/x86_64-wine/version.dll",
        ):
            matches = sorted(proton_root.glob(pattern))
            for match in matches:
                if match.is_file():
                    return match.resolve()
        return None

    def validate_game_dir(self, game_dir: Path) -> Path:
        """
        Validate and normalize the Anno Bin/Win64 directory.

        Args:
            game_dir: Path that should contain Anno1800.exe.

        Returns:
            Resolved game directory path.

        Raises:
            FileNotFoundError: If the directory or Anno executable is missing.
        """
        resolved = game_dir.expanduser().resolve()
        if not resolved.is_dir():
            raise FileNotFoundError(f"game directory not found: {resolved}")
        exe = resolved / "Anno1800.exe"
        if not exe.is_file():
            raise FileNotFoundError(
                f"Anno1800.exe not found in game directory: {resolved}"
            )
        return resolved

    def install(
        self,
        game_dir: Path,
        proxy_dll: Path,
        proton_root: Path | None = None,
        dry_run: bool = False,
    ) -> dict[str, str]:
        """
        Install proxy version.dll and version_orig.dll into the game directory.

        Args:
            game_dir: Anno Bin/Win64 directory.
            proxy_dll: Built proxy version.dll path.
            proton_root: Optional explicit Proton root.
            dry_run: If True, do not copy files.

        Returns:
            Dictionary with installed paths and status messages.
        """
        target_dir = self.validate_game_dir(game_dir)
        if not proxy_dll.is_file():
            raise FileNotFoundError(f"proxy DLL not found: {proxy_dll}")

        source_version = self.resolve_version_dll(proton_root=proton_root)
        if source_version is None:
            raise FileNotFoundError(
                "Could not find Proton/Wine version.dll. "
                "Pass --proton /path/to/Proton*"
            )

        target_proxy = target_dir / "version.dll"
        target_orig = target_dir / "version_orig.dll"
        result = {
            "game_dir": str(target_dir),
            "source_version": str(source_version),
            "target_proxy": str(target_proxy),
            "target_orig": str(target_orig),
            "menu_dx11": str(target_dir / "Anno1800ModMenuDX11.dll"),
            "menu_dx12": str(target_dir / "Anno1800ModMenuDX12.dll"),
        }

        if not dry_run:
            shutil.copy2(proxy_dll, target_proxy)
            if target_orig.exists():
                try:
                    target_orig.chmod(target_orig.stat().st_mode | 0o200)
                except OSError as exc:
                    print(f"WARNING: could not chmod version_orig.dll: {exc}")
            shutil.copy2(source_version, target_orig)

        result["menu_dx11_present"] = str(
            (target_dir / "Anno1800ModMenuDX11.dll").is_file()
        )
        result["menu_dx12_present"] = str(
            (target_dir / "Anno1800ModMenuDX12.dll").is_file()
        )
        result["dry_run"] = str(dry_run)
        result["override"] = "WINEDLLOVERRIDES=version=n,b"
        return result


def default_proxy_path() -> Path:
    """
    Return the default built proxy path relative to this repository.

    Returns:
        Path to Build/version.dll.
    """
    return Path(__file__).resolve().parents[1] / "Build" / "version.dll"
