# Extraction notes

Source snapshot copied from `striker-samuel/tourmaline` on 2026-11-04.

This PR is a **starting point**, not a completed extraction. The backend implementation in Tourmaline's `engine/src/Vulkan/impl.cpp` is large and coupled to other engine headers. Port only required implementation into `src/Vulkan/` and establish the narrow independent dependency set before compiling this target. The rendergraph header and RHI also contain Tourmaline-specific includes.

Use `smvk` as upstream after extraction; Tourmaline will vendor a pinned copy under `external/smvk/` by an explicit sync operation. Do not merge an unverified API/build change into Tourmaline.
