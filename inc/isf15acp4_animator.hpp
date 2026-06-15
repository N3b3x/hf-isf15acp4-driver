/**
 * @file isf15acp4_animator.hpp
 * @brief Frame sequencer and procedural animation helpers for SmartDisplay.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "isf15acp4_graphics_context.hpp"

namespace isf15acp4 {

/** @brief Callback that renders one animation frame into a graphics context. */
using AnimationFrameFn = void (*)(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept;

/**
 * @class Animator
 * @brief Drives timed frame updates on top of a GraphicsContext.
 */
class Animator {
public:
    Animator(GraphicsContext& gfx, AnimationFrameFn fn, void* user_data = nullptr,
             uint32_t fps = 30) noexcept;

    /** @brief Advance animation clock and invoke frame callback when due. */
    bool Tick(uint32_t elapsed_ms) noexcept;

    /** @brief Force immediate frame render. */
    void RenderNow() noexcept;

    void SetFps(uint32_t fps) noexcept { frame_ms_ = fps > 0 ? (1000u / fps) : 33u; }
    void Reset() noexcept { frame_ = 0; elapsed_accum_ = 0; }
    [[nodiscard]] uint32_t Frame() const noexcept { return frame_; }

private:
    GraphicsContext& gfx_;
    AnimationFrameFn frame_fn_;
    void* user_data_;
    uint32_t frame_{0};
    uint32_t frame_ms_{33};
    uint32_t elapsed_accum_{0};
};

namespace animations {

void ColorWheel(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept;
void Plasma(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept;
void ScrollingText(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept;
void PulseButton(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept;

}  // namespace animations

}  // namespace isf15acp4

#define ISF15ACP4_ANIMATOR_HEADER_INCLUDED
#include "../src/isf15acp4_animator.ipp"
#undef ISF15ACP4_ANIMATOR_HEADER_INCLUDED
