# smvk

Vulkan-first graphics API extracted from [Tourmaline](https://github.com/striker-samuel/tourmaline).

> **Work in progress.** This branch is an initial source snapshot, not yet a standalone buildable graphics library. Header paths and transitive dependencies still refer to Tourmaline's `Engine/` tree.

## Planned scope

- smvk API and resource/swapchain management
- Vulkan backend and generic render graph
- Standalone triangle, blur and fuzz samples

## Before merging

- [ ] Isolate required Vulkan backend code and any shared utility dependencies
- [ ] Add an independent CMake `smvk` library and link its dependencies explicitly
- [ ] Remove dependencies on the Tourmaline world renderer, asset pipeline and app host
- [ ] Build and run triangle/blur/fuzz from a clean clone
- [ ] Add Windows and Linux CI
- [ ] Verify licenses and third-party notices

This branch intentionally preserves existing public `smvk` names and upstream source headers while the build boundary is being extracted.
