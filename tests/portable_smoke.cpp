// Copyright (c) 2026 Samuel Martin
// SPDX-License-Identifier: MIT

#include <smvk/core/handle.h>
#include <smvk/core/string_view.h>
#include <smvk/rhi/descriptor_limits.h>
#include <smvk/rhi/frame_config.h>
#include <smvk/rhi/gpu_memory_spill.h>
#include <cassert>
#include <cstdint>

int main() {
    static_assert(smvk::limits::MaxTextures == 8192);
    static_assert(smvk::limits::MaxVolumeTextures == 8192);
    static_assert(smvk::limits::MaxSamplers == 8);
    static_assert(smvk::limits::MaxFramesInFlight == 2);

    auto h = smvk::Handle<int>::Mint(12);
    assert(h.valid() && h.index() == 12 && h.gen() == 1);

    smvk::StringViewT view("smvk");
    assert(view.size() == 4 && view.substr(1, 2) == "mv");

    smvk::GpuMemorySpillDetector detector;
    const smvk::GpuMemoryPressureSample state{50, 100, 0, false};
    auto warning = detector.Update(state, 1.0);
    assert(warning.state == smvk::GpuMemoryWarning::Healthy);
    return 0;
}
