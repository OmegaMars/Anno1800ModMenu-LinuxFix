# MenuLoader – Programming Notes

## Usage

```cpp
#include "MenuInjection/MenuLoader.h"

MenuLoader loader;
HMODULE menu = loader.LoadWhenReady(50, 1000);
```

## Important Methods

| Method | Usage |
|--------|--------|
| `LoadWhenReady(poll_ms, settle_ms)` | Blocks until DX is ready, then loads the menu DLL |
| `ResolveMenuPath(backend_value)` | private: file name for the selected backend |

Typically called from a worker thread after `DllMain` `PROCESS_ATTACH`.
