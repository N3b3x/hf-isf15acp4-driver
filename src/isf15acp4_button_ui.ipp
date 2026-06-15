#ifndef ISF15ACP4_BUTTON_UI_HEADER_INCLUDED
#error "isf15acp4_button_ui.ipp must be included from isf15acp4_button_ui.hpp"
#endif

#include <cmath>

namespace isf15acp4 {

inline ButtonUI::ButtonUI(uint32_t debounce_ms, uint32_t long_press_ms) noexcept
    : debounce_ms_(debounce_ms), long_press_ms_(long_press_ms) {}

inline ButtonEvent ButtonUI::Update(bool pressed, uint32_t elapsed_ms) noexcept {
    stable_ms_ += elapsed_ms;
    if (stable_ms_ < debounce_ms_) {
        return ButtonEvent::None;
    }
    stable_ms_ = 0;

    if (pressed && !is_down_) {
        is_down_ = true;
        hold_ms_ = 0;
        long_fired_ = false;
        ++press_count_;
        return ButtonEvent::Pressed;
    }

    if (!pressed && is_down_) {
        is_down_ = false;
        const auto ev = long_fired_ ? ButtonEvent::None : ButtonEvent::Click;
        hold_ms_ = 0;
        return ev == ButtonEvent::None ? ButtonEvent::Released : ev;
    }

    if (pressed && is_down_) {
        hold_ms_ += elapsed_ms;
        if (!long_fired_ && hold_ms_ >= long_press_ms_) {
            long_fired_ = true;
            return ButtonEvent::LongPress;
        }
    }

    return ButtonEvent::None;
}

inline void ButtonUI::DrawStatus(GraphicsContext& gfx, const char* label, bool active) const noexcept {
    gfx.Clear(active ? MakeColor565(0, 40, 0) : colors::Black);
    gfx.FillRect({0, 0, gfx.Width(), 11}, colors::Blue);
    gfx.DrawString(4, 2, label != nullptr ? label : "BTN", colors::White);
    gfx.DrawString(4, 20, active ? "PRESSED" : "READY", active ? colors::Green : colors::Gray);
    gfx.DrawRect({2, 14, static_cast<uint8_t>(gfx.Width() - 3), static_cast<uint8_t>(gfx.Height() - 3)},
                 active ? colors::Green : colors::White);
}

inline void ButtonUI::DrawGauge(GraphicsContext& gfx, uint8_t cx, uint8_t cy, uint8_t radius,
                                float value_0_to_1, Color565 fg, Color565 bg) noexcept {
    if (value_0_to_1 < 0.0f) {
        value_0_to_1 = 0.0f;
    }
    if (value_0_to_1 > 1.0f) {
        value_0_to_1 = 1.0f;
    }

    for (uint8_t r = 0; r <= radius; ++r) {
        for (int angle = 180; angle >= 0; --angle) {
            const float rad = static_cast<float>(angle) * 3.14159265f / 180.0f;
            const int x = static_cast<int>(cx + std::cos(rad) * static_cast<float>(r));
            const int y = static_cast<int>(cy + std::sin(rad) * static_cast<float>(r));
            const float t = static_cast<float>(angle) / 180.0f;
            const Color565 c = (t <= value_0_to_1) ? fg : bg;
            if (x >= 0 && y >= 0 && x < gfx.Width() && y < gfx.Height()) {
                gfx.SetPixel(static_cast<uint8_t>(x), static_cast<uint8_t>(y), c);
            }
        }
    }
}

}  // namespace isf15acp4
