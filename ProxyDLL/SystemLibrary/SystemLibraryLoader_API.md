# SystemLibraryLoader – Programming Notes

## Usage

```cpp
#include "SystemLibrary/SystemLibraryLoader.h"

SystemLibraryLoader loader;
HMODULE real = loader.LoadFromModuleDirectory(hSelf, L"version_orig.dll");
if (real == nullptr) {
    real = loader.LoadFromSystemDirectory(L"version.dll");
}
```

## Important Methods

| Method | Usage |
|--------|--------|
| `LoadFromModuleDirectory(self, file_name)` | Load `version_orig.dll` beside the proxy |
| `LoadFromSystemDirectory(file_name)` | Fallback via system32 / `LOAD_LIBRARY_SEARCH_SYSTEM32` |

On failure: `OutputDebugStringW` + `NULL`.
