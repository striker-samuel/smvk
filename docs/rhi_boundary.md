# RHI ownership boundary

smvk is a Vulkan-first GPU control layer, not an alternate scene renderer.

## Keep in the RHI

- The **reserved bindless descriptor bindings** are intentionally RHI-owned. They form a stable shader ABI and simplify descriptor indexing.
- **Vertex pulling and static GPU vertex-buffer support** remain RHI responsibilities. Do not remove them as part of policy cleanup.
- GPU capabilities, queues, resource creation, memory management, synchronization, pipeline creation, command submission, and presentation.
- Optional utilities such as SPIR-V reflection, a render graph, and GPU-memory diagnostics can build above the core.

## Removed in this change

- `DebugImageDumpArmed`: reads Tourmaline-specific SSR, EVSM, fog and contact-shadow environment switches. The renderer should choose image usage flags and capture policy.
- `ResourceManager::SupportsEvsm`: an EVSM-specific aggregate of hardware checks; the shadow renderer should evaluate device format/sample/filtering capabilities. Generic format-query support can be added to the RHI separately.
- `ResourceManager::PollGpuMemoryWarning`: opinionated threshold and hysteresis policy does not belong in resource management. Continue to use `GetGpuMemoryInfo` for raw heap telemetry, and optionally use `smvk/diagnostics/gpu_memory_spill.h` from the host.
- `TextureStreamMeta`: a cooked-pak tail-mip marker, rather than a GPU resource contract. Define it with the asset streamer.

## Remaining staged extraction (not yet removed)

- `TargetPhase` and target-heap claiming still assume Tourmaline's render pass order. Replace with generic validated lifetime/interval data, including cross-frame and async queue lifetime constraints, **before** deleting the enum.
- `EngineGlobals` and named PCF/EVSM `GpuStageTimer` slots remain deeply wired to the Vulkan backend. Make their shader data/timestamp labels opt-in and client-defined in a follow-up.
- `RayTracingInstance`/`BuildRayTracingWorld` still model static scene geometry and material cutout. Introduce generic BLAS/TLAS primitives before removing them.
- KTX2 streaming and fixed texture-residency policy still live in the resource manager; extract into an asset/streaming adapter.

This is an incremental API-breaking cleanup. Tourmaline callers of removed symbols must migrate when updating their vendored smvk snapshot. The current portable build does **not** compile the staged Vulkan backend.
