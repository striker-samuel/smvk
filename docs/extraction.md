# smvk extraction audit (work in progress)

## Ownership boundary

- **smvk** owns the public RHI types, GPU resource management, Vulkan backend,
  swapchain, reusable render graph, SPIR-V reflection, and low-level samples.
- **Tourmaline** owns the world renderer, meshes/assets, scene pipeline,
  materials, shader-library overlays, editor, physics/audio, profiling UI and
  application host. It will consume a pinned vendored smvk snapshot.
- Keep `namespace smvk` and existing public semantics when moving code.
  Do not introduce a multi-backend abstraction just for this extraction.

## Landed in this branch

- Moved public headers to `include/smvk/` with lowercase filenames.
- Staged existing Vulkan implementation and both Vulkan .inl companions under
  `src/vulkan/`. This is SOURCE IMPORT, not a standalone-ready backend.
- Staged the RHI resource manager, swapchain, render graph and three samples.
- Moved the generic handle, string view, pool/shard config and SPIR-V
  reflection components to smvk ownership.
- Separated descriptor constants from Tourmaline's engine shader limits.
  When vendoring, ensure shaders share the same descriptor budget contract.
- Removed the Tourmaline shader-library hot-reload helper from the public
  smvk render graph header. That helper belongs on the consumer side.
- Vendored Core headers and generic allocator implementations from Tourmaline\n  Core separation PR #253 (commit `8affc5dc7c6fe5083bdf7574deab0b6aca9835b0`).\n  The render graph now references the canonical `Core/Memory/memory.h` header;\n  legacy compat headers are thin forwarders instead of duplicated definitions.\n- Added a portable CMake target, portable smoke test, file-layout enforcement
  and Windows/Linux CI configuration.

## Still blocking a standalone Vulkan build

1. `src/vulkan/impl.cpp` directly includes Tourmaline App, input, world,
   renderer meshes, DLSS, profiler, ImGui and third-party dependencies.
   Separate device/backend behavior from app-host and world rendering,
   keeping engine adapters in Tourmaline. `smvk::VertexLayoutId` now
   lives in smvk.h (moved from Tourmaline mesh.h so the public graph
   header compiles standalone — Tourmaline must drop its mesh.h copy
   when vendoring); the layout table it selects (`GetVertexLayoutDesc`)
   is still consumer-side and must move with the backend.
2. `src/vulkan/detail/vulkan_impl.h` still includes Tourmaline memory
   allocators and static vertex heaps. Decide which pieces are genuinely RHI
   owned; move those, and convert world-specific heap code to consumer APIs.
3. `include/smvk/render_graph/render_graph.h` now depends on vendored\n   `Core/Memory/memory.h` for allocator ownership types. Core is compiled\n   independently of Engine, but the graph's lifetime/ownership contract\n   remains coupled to that allocator API. Retire legacy compat includes\n   as staged Vulkan sources are decoupled.
4. `src/rhi/resource_manager.cpp` and the sample sources still require
   Vulkan/SDL3/KTX/DXC and Tourmaline host/setup implementation. Define an
   independent dependency setup and minimal smvk sample host.
5. Port the generic render graph implementation from the real backend
   and eliminate duplicate compilation in Tourmaline before linking both.
6. Configure a true `smvk` static library (not just `smvk_portable`),
   add explicit public/private dependencies and CMake packaging.
7. Build and run triangle, blur and fuzz on a clean Windows and Linux clone.
   Add headless RHI tests where no GPU is required.
8. Update third-party notices and verify source licensing before release.

## PR acceptance

- [ ] No `#include <Engine/...>` in the standalone smvk library or public headers
- [ ] A single compiled `smvk` target owns the Vulkan backend and render graph
- [ ] Windows/Linux library builds pass from clean clones
- [ ] Triangle, blur and fuzz build and behave correctly
- [ ] Tests cover device-init failure, resource lifetimes, synchronization and swapchain recovery
- [ ] Shader constants and API layout remain compatible with Tourmaline
- [ ] Pin a reviewed upstream SHA for the separate **experimental, do-not-merge** Tourmaline vendor PR

Do not merge until these conditions are met.
