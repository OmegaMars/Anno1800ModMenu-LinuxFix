# InjectionScheduler – Programming Notes

## Usage

```cpp
#include "MenuInjection/InjectionScheduler.h"

InjectionScheduler::RequestStart();
```

Call from exported functions (or any code path after `PROCESS_ATTACH`), never from `DllMain`.

## Important Methods

| Method | Usage |
|--------|--------|
| `RequestStart()` | One-shot: spawns worker that calls `MenuLoader::LoadWhenReady` |
| `Worker` | private thread entry |
| `AppendLog` | writes `inject.log` beside the proxy DLL |

Failures are logged to `inject.log` and `OutputDebugStringA`.
