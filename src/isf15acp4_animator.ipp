#ifndef ISF15ACP4_ANIMATOR_HEADER_INCLUDED
#error "isf15acp4_animator.ipp must be included from isf15acp4_animator.hpp"
#endif

#include <cmath>

namespace isf15acp4 {

inline Animator::Animator(GraphicsContext& gfx, AnimationFrameFn fn, void* user_data,
                          uint32_t fps) noexcept
    : gfx_(gfx), frame_fn_(fn), user_data_(user_data) {
    SetFps(fps);
}

inline bool Animator::Tick(uint32_t elapsed_ms) noexcept {
    elapsed_accum_ += elapsed_ms;
    if (elapsed_accum_ < frame_ms_) {
        return false;
    }
    elapsed_accum_ -= frame_ms_;
    RenderNow();
    return true;
}

inline void Animator::RenderNow() noexcept {
    if (frame_fn_ != nullptr) {
        frame_fn_(gfx_, frame_, user_data_);
    }
    ++frame_;
}

namespace animations {

inline void ColorWheel(GraphicsContext& gfx, uint32_t frame, void* /*user_data*/) noexcept {
    const uint8_t w = gfx.Width();
    const uint8_t h = gfx.Height();
    const uint8_t cx = static_cast<uint8_t>(w / 2);
    const uint8_t cy = static_cast<uint8_t>(h / 2);
    const float phase = static_cast<float>(frame) * 0.08f;

    for (uint8_t y = 0; y < h; ++y) {
        for (uint8_t x = 0; x < w; ++x) {
            const float dx = static_cast<float>(x - cx);
            const float dy = static_cast<float>(y - cy);
            const float angle = std::atan2(dy, dx) + phase;
            const uint8_t r = static_cast<uint8_t>((std::sin(angle) * 0.5f + 0.5f) * 255.0f);
            const uint8_t g =
                static_cast<uint8_t>((std::sin(angle + 2.094f) * 0.5f + 0.5f) * 255.0f);
            const uint8_t b =
                static_cast<uint8_t>((std::sin(angle + 4.188f) * 0.5f + 0.5f) * 255.0f);
            gfx.SetPixel(x, y, MakeColor565(r, g, b));
        }
    }
    gfx.DrawString(2, 2, "WHEEL", colors::White, colors::Black);
}

inline void Plasma(GraphicsContext& gfx, uint32_t frame, void* /*user_data*/) noexcept {
    const uint8_t w = gfx.Width();
    const uint8_t h = gfx.Height();
    for (uint8_t y = 0; y < h; ++y) {
        for (uint8_t x = 0; x < w; ++x) {
            const float v = std::sin((x + frame) * 0.15f) + std::sin((y + frame) * 0.21f) +
                            std::sin((x + y + frame) * 0.11f);
            const uint8_t c = static_cast<uint8_t>((v + 3.0f) / 6.0f * 255.0f);
            gfx.SetPixel(x, y, MakeColor565(c, c / 2, 255 - c));
        }
    }
    gfx.DrawString(2, h - 9, "PLASMA", colors::Cyan, colors::Black);
}

struct ScrollTextState {
    const char* text;
};

inline void ScrollingText(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept {
    auto* state = static_cast<ScrollTextState*>(user_data);
    gfx.Clear(colors::Black);
    gfx.DrawString(2, 2, "SmartDisplay", colors::White);
    const int offset = static_cast<int>(frame % 120);
    gfx.DrawString(static_cast<uint8_t>(gfx.Width() - offset), static_cast<uint8_t>(gfx.Height() / 2),
                   state != nullptr ? state->text : "ISF15ACP4", colors::Orange);
}

struct PulseButtonState {
    bool pressed;
};

inline void PulseButton(GraphicsContext& gfx, uint32_t frame, void* user_data) noexcept {
    auto* state = static_cast<PulseButtonState*>(user_data);
    const bool pressed = state != nullptr && state->pressed;
    gfx.Clear(pressed ? MakeColor565(0, 48, 0) : colors::Black);
    gfx.DrawRect({0, 0, static_cast<uint8_t>(gfx.Width() - 1), static_cast<uint8_t>(gfx.Height() - 1)},
                 pressed ? colors::Green : colors::Gray);

    const uint8_t pulse = static_cast<uint8_t>((frame % 20) + (pressed ? 20 : 0));
    gfx.FillRect({40, 24, static_cast<uint8_t>(40 + pulse / 4), 40},
                 pressed ? colors::Green : colors::Blue);
    gfx.DrawString(18, 48, pressed ? "ACTIVE" : "IDLE", colors::White);
}

}  // namespace animations

}  // namespace isf15acp4
