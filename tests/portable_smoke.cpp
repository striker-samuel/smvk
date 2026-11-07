// Copyright (c) 2026 Samuel Martin
// SPDX-License-Identifier: MIT

#include <smvk/smvk.h>
#include <smvk/core/handle.h>
#include <smvk/core/pool.h>
#include <smvk/core/string_view.h>
#include <smvk/reflection/spirv_reflection.h>
#include <smvk/rhi/descriptor_limits.h>
#include <smvk/rhi/frame_config.h>
#include <smvk/diagnostics/gpu_memory_spill.h>
#include <smvk/rhi/resource_manager.h>
#include <smvk/rhi/swapchain.h>
// Compile step: every public header must build standalone — including
// the render graph, which resolves <Core/Memory/memory.h> from vendored Core.
#include <smvk/render_graph/render_graph.h>
#include <cstdint>
#include <cstdio>
#include <type_traits>

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
    // The render graph header compiles against the vendored Core memory API:
    // smvk::mem::LinearPtr must resolve to the dtor-only unique_ptr.
    static_assert(std::is_same_v<
        smvk::mem::LinearPtr<int>,
        std::unique_ptr<int, smvk::mem::LinearDeleter<int>>>);

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
