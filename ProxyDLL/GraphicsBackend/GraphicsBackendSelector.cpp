#include "GraphicsBackendSelector.h"

bool GraphicsBackendSelector::IsReady(bool has_d3d11, bool has_d3d12) {
    return has_d3d11 || has_d3d12;
}

GraphicsBackendSelector::Backend GraphicsBackendSelector::Select(
    bool has_d3d11,
    bool has_d3d12,
    bool has_d3d11_on_12
) {
    if (!IsReady(has_d3d11, has_d3d12)) {
        return Backend::None;
    }
    if (has_d3d11_on_12) {
        return Backend::Dx12;
    }
    return Backend::Dx11;
}
