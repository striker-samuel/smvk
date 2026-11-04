// Copyright (c) 2026 Samuel Martin
// SPDX-License-Identifier: MIT

#include <smvk/smvk.h>
#include <smvk/core/pool.h>
#include <smvk/rhi/gpu_memory_spill.h>
#include <cstdint>
#include <cstdio>

namespace {
bool Check(bool condition, const char* message) {
    if (!condition) std::fprintf(stderr, "smvk portable smoke: %s\n", message);
    return condition;
}
}

int main() {
    static_assert(smvk::limits::MaxTextures == 8192);
    static_assert(smvk::limits::MaxVolumeTextures == 8192);
    static_assert(smvk::limits::MaxSamplers == 8);
    static_assert(smvk::limits::MaxFramesInFlight == 2);
    static_assert(smvk::mem::config::kShardedMaxSize == smvk::limits::MaxTextures);

    const auto h = smvk::Handle<int>::Mint(12);
    if (!Check(h.valid() && h.index() == 12 && h.gen() == 1, "handle mint"))
        return 1;

    const smvk::StringViewT view("smvk");
    if (!Check(view.size() == 4 && view.substr(1, 2) == "mv", "string view"))
        return 1;

    smvk::GpuMemorySpillDetector detector;
    const smvk::GpuMemoryPressureSample state{50, 100, 0, false};
    auto warning = detector.Update(state, 1.0);
    if (!Check(warning.state == smvk::GpuMemoryWarning::Healthy, "memory pressure"))
        return 1;
    return 0;
}
