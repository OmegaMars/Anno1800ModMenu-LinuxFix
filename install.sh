#!/usr/bin/env bash
# Install Linux/Proton version.dll proxy for Anno 1800 Mod Menu.
# Pure bash — no Python required.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"

GAME_DIR=""
PROTON=""
PROXY=""
DRY_RUN=0

usage() {
  cat <<'EOF'
Install Linux/Proton version.dll proxy for Anno 1800 Mod Menu.

Usage:
  ./install.sh --game-dir "/path/to/Anno 1800/Bin/Win64" [options]

Options:
  --game-dir DIR   Anno Bin/Win64 (must contain Anno1800.exe)
  --proton DIR     Proton/Wine root with x86_64-windows/version.dll
  --proxy FILE     Proxy DLL (default: ./version.dll or ./Build/version.dll)
  --dry-run        Print actions without copying
  -h, --help       Show this help

After install set:
  WINEDLLOVERRIDES=version=n,b

Steam: game launch options.
Lutris: Ubisoft Connect / Proton runner environment (process that starts Anno).

Menu DLLs (Anno1800ModMenuDX11.dll / DX12.dll) are NOT installed by this script.
EOF
}

die() {
  echo "ERROR: $*" >&2
  exit 1
}

log() {
  echo "$*"
}

resolve_proxy() {
  if [[ -n "${PROXY}" ]]; then
    [[ -f "${PROXY}" ]] || die "proxy DLL not found: ${PROXY}"
    printf '%s\n' "${PROXY}"
    return
  fi
  if [[ -f "${ROOT}/version.dll" ]]; then
    printf '%s\n' "${ROOT}/version.dll"
    return
  fi
  if [[ -f "${ROOT}/Build/version.dll" ]]; then
    printf '%s\n' "${ROOT}/Build/version.dll"
    return
  fi
  die "proxy DLL not found (tried ${ROOT}/version.dll and ${ROOT}/Build/version.dll)"
}

version_under_proton() {
  local root="$1"
  local candidate
  for candidate in \
    "${root}/files/lib/wine/x86_64-windows/version.dll" \
    "${root}/files/lib64/wine/x86_64-windows/version.dll" \
    "${root}/lib/wine/x86_64-windows/version.dll" \
    "${root}/lib64/wine/x86_64-windows/version.dll"
  do
    if [[ -f "${candidate}" ]]; then
      printf '%s\n' "${candidate}"
      return 0
    fi
  done
  return 1
}

resolve_proton_version() {
  local found=""
  local steam_root common entry name

  if [[ -n "${PROTON}" ]]; then
    if found="$(version_under_proton "${PROTON}")"; then
      printf '%s\n' "${found}"
      return 0
    fi
    die "version.dll not found under --proton: ${PROTON}"
  fi

  local -a steam_roots=(
    "${HOME}/.steam/steam"
    "${HOME}/.steam/debian-installation"
    "${HOME}/.local/share/Steam"
    "${HOME}/.var/app/com.valvesoftware.Steam/data/Steam"
  )

  for steam_root in "${steam_roots[@]}"; do
    common="${steam_root}/steamapps/common"
    [[ -d "${common}" ]] || continue
    shopt -s nullglob
    for entry in "${common}"/*; do
      [[ -d "${entry}" ]] || continue
      name="$(basename "${entry}")"
      name_lc="$(printf '%s' "${name}" | tr '[:upper:]' '[:lower:]')"
      case "${name_lc}" in
        *proton*)
          if found="$(version_under_proton "${entry}")"; then
            shopt -u nullglob
            printf '%s\n' "${found}"
            return 0
          fi
          ;;
      esac
    done
    shopt -u nullglob
  done

  return 1
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --game-dir)
      GAME_DIR="${2:-}"
      shift 2
      ;;
    --proton)
      PROTON="${2:-}"
      shift 2
      ;;
    --proxy)
      PROXY="${2:-}"
      shift 2
      ;;
    --dry-run)
      DRY_RUN=1
      shift
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      usage >&2
      exit 1
      ;;
  esac
done

[[ -n "${GAME_DIR}" ]] || die "--game-dir is required (see --help)"

GAME_DIR="$(cd "${GAME_DIR}" && pwd)" || die "game directory not found: ${GAME_DIR}"
[[ -f "${GAME_DIR}/Anno1800.exe" ]] || die "Anno1800.exe not found in: ${GAME_DIR}"

if [[ -n "${PROTON}" ]]; then
  PROTON="$(cd "${PROTON}" && pwd)" || die "proton directory not found: ${PROTON}"
fi

PROXY_PATH="$(resolve_proxy)"
SOURCE_VERSION="$(resolve_proton_version)" || die \
  "Could not find Proton/Wine version.dll. Pass --proton /path/to/Proton*"

TARGET_PROXY="${GAME_DIR}/version.dll"
TARGET_ORIG="${GAME_DIR}/version_orig.dll"

log "Install summary"
log "  game_dir:       ${GAME_DIR}"
log "  source_version: ${SOURCE_VERSION}"
log "  proxy_source:   ${PROXY_PATH}"
log "  target_proxy:   ${TARGET_PROXY}"
log "  target_orig:    ${TARGET_ORIG}"
log "  dry_run:        ${DRY_RUN}"
log ""

if [[ "${DRY_RUN}" -eq 0 ]]; then
  cp -f "${PROXY_PATH}" "${TARGET_PROXY}"
  if [[ -e "${TARGET_ORIG}" ]]; then
    chmod u+w "${TARGET_ORIG}" 2>/dev/null || \
      log "WARNING: could not chmod version_orig.dll"
  fi
  cp -f "${SOURCE_VERSION}" "${TARGET_ORIG}"
  chmod u+rw "${TARGET_PROXY}" "${TARGET_ORIG}" 2>/dev/null || true
fi

if [[ ! -f "${GAME_DIR}/Anno1800ModMenuDX11.dll" \
   && ! -f "${GAME_DIR}/Anno1800ModMenuDX12.dll" ]]; then
  log "WARNING: Anno1800ModMenuDX11.dll / DX12.dll not found in game dir."
  log "         Install them from the original Mod Menu release."
  log ""
fi

log "Required environment / Lutris override:"
log "  WINEDLLOVERRIDES=version=n,b"
log ""
log "Then start the game and press F7."
