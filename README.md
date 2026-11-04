# smvk

Vulkan-first C++20 graphics API and render graph, being extracted from [Tourmaline](https://github.com/striker-samuel/tourmaline).

> **Extraction in progress — full RHI is NOT standalone or build-verified yet.**
> The current CMake build exercises only the independent portable headers. The
> Vulkan backend and triangle/blur/fuzz source are staged for ongoing isolation.

## Layout

```text
include/smvk/
  core/                 handles, sharded pools, string views
  reflection/           SPIR-V reflection declarations
  rhi/                  descriptors, resource management, swapchain
  diagnostics/          optional GPU-memory pressure heuristics
  render_graph/         render graph API
  smvk.h                primary graphics API types
compat/
  Engine/Memory/        transitional Tourmaline memory.h copy (see
                       docs/extraction.md blocker 3; remove before vendoring)
src/
  reflection/           SPIR-V reflection implementation (needs SPIRV-Reflect)
  rhi/
  render_graph/
  vulkan/               upstream backend implementation, pending decoupling
samples/
  01_triangle/
  02_blur/
  03_fuzz/
shaders/
  descriptor_limits.hlsli
```

All source directories and filenames use lowercase `snake_case`. Public
headers are under `include/smvk/` rather than `include/Engine/`.
Existing C++ API symbols remain in `namespace smvk`.

## What builds today

```sh
cmake -S . -B build -DSMVK_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

These commands configure and test **smvk_portable**, an interface-only
target. The smoke step compiles every public header standalone — including
the render graph, which resolves `Engine/Memory/memory.h` through the
transitional copy in `compat/`. They do **not** compile the Vulkan
implementation or run the graphics examples.
The portability checks are configured for Windows and Linux in GitHub Actions.
Check CI status before relying on the tests.

## Next steps

See [extraction audit](docs/extraction.md) for the precise remaining
dependencies and acceptance checklist. In particular, the source still uses
some Tourmaline-owned memory, app-host, profiler and renderer functionality.
Do not vendor this branch as a replacement for Tourmaline's working graphics
implementation until the full RHI target and samples pass clean-clone builds.

## License

MIT, Copyright (c) 2026 Samuel Martin. The source was originally developed
in Tourmaline and retains upstream headers. Additional third-party dependencies
will need separate notices as they are wired into the standalone build.
