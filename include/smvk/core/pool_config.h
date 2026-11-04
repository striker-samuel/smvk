// Copyright (c) 2026 Samuel Martin
// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
namespace smvk::mem::config {
inline constexpr uint32_t kNumPoolShards = 16;
inline constexpr uint32_t kShardCapacity = 512;
inline constexpr uint32_t kShardedMaxSize = kNumPoolShards * kShardCapacity;
}
