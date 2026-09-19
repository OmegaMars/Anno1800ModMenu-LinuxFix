# GraphicsBackendSelector – Programming Notes

## Usage

```cpp
#include "GraphicsBackend/GraphicsBackendSelector.h"

bool ready = GraphicsBackendSelector::IsReady(has_d3d11, has_d3d12);
auto backend = GraphicsBackendSelector::Select(has_d3d11, has_d3d12, has_d3d11_on_12);
```

## Important Methods

| Method | Usage |
|--------|--------|
| `IsReady(has_d3d11, has_d3d12)` | Poll loop: wait until true |
| `Select(...)` | After ready: choose DX11 vs DX12 |

No Windows dependency in this class — easy to unit-test / mirror in Python.
