#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT_DIR="${ROOT}/Build"
OUT_DLL="${OUT_DIR}/version.dll"
CXX="${CXX:-x86_64-w64-mingw32-g++}"

mkdir -p "${OUT_DIR}"

"${CXX}" \
  -shared \
  -O2 \
  -std=c++17 \
  -Wall \
  -Wextra \
  -DNDEBUG \
  -o "${OUT_DLL}" \
  "${ROOT}/ProxyDLL/dllmain.cpp" \
  "${ROOT}/ProxyDLL/GraphicsBackend/GraphicsBackendSelector.cpp" \
  "${ROOT}/ProxyDLL/MenuInjection/InjectionScheduler.cpp" \
  "${ROOT}/ProxyDLL/MenuInjection/MenuLoader.cpp" \
  "${ROOT}/ProxyDLL/SystemLibrary/SystemLibraryLoader.cpp" \
  "${ROOT}/ProxyDLL/VersionExports/VersionExports.cpp" \
  -static-libgcc \
  -static-libstdc++ \
  -Wl,--enable-stdcall-fix \
  -s

echo "Built: ${OUT_DLL}"
