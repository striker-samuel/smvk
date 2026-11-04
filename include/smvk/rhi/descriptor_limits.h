// Copyright (c) 2026 Samuel Martin
// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "../../../shaders/descriptor_limits.hlsli"
namespace smvk::limits {
inline constexpr uint32_t MaxTextures = SMVK_MAX_TEXTURES;
inline constexpr uint32_t MaxVolumeTextures = SMVK_MAX_VOLUME_TEXTURES;
inline constexpr uint32_t MaxSamplers = SMVK_MAX_SAMPLERS;
} // namespace smvk::limits
#undef SMVK_MAX_TEXTURES
#undef SMVK_MAX_VOLUME_TEXTURES
#undef SMVK_MAX_SAMPLERS
