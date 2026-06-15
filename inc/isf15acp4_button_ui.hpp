/**
 * @file isf15acp4_button_ui.hpp
 * @brief High-level button + display interaction patterns for SmartDisplay.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstdint>

#include "isf15acp4_graphics_context.hpp"

namespace isf15acp4 {

/**
 * @brief Configuration for SmartDisplay construction.
 */
struct SmartDisplayConfig {
    ProductVariant variant{ProductVariant::Isf15acp4};
    GraphicsBackend graphics{GraphicsBackend::BuiltinCanvas};
    bool enable_vcc_on_init{true};
};

/**
 * @enum ButtonEvent
 * @brief Edge-qualified button events from debounced polling.
 */
enum class ButtonEvent : uint8_t {
    None = 0,
    Pressed,
    Released,
    Click,
    LongPress
};

/**
 * @class ButtonUI
 * @brief Debounced switch handling with visual feedback on the OLED.
 */
class ButtonUI {
public:
    ButtonUI(uint32_t debounce_ms = 25, uint32_t long_press_ms = 800) noexcept;

    /**
     * @brief Poll raw pressed state and return edge/event classification.
     * @param pressed Current momentary switch state.
     * @param elapsed_ms Milliseconds since last poll.
     */
    ButtonEvent Update(bool pressed, uint32_t elapsed_ms) noexcept;

    /** @brief Draw idle/active visual using graphics context. */
    void DrawStatus(GraphicsContext& gfx, const char* label, bool active) const noexcept;

    /** @brief Draw a compact gauge arc and numeric value. */
    static void DrawGauge(GraphicsContext& gfx, uint8_t cx, uint8_t cy, uint8_t radius,
                          float value_0_to_1, Color565 fg, Color565 bg) noexcept;

    [[nodiscard]] uint32_t PressCount() const noexcept { return press_count_; }
    [[nodiscard]] bool IsDown() const noexcept { return is_down_; }

private:
    uint32_t debounce_ms_;
    uint32_t long_press_ms_;
    uint32_t stable_ms_{0};
    uint32_t hold_ms_{0};
    uint32_t press_count_{0};
    bool is_down_{false};
    bool long_fired_{false};
};

}  // namespace isf15acp4

#define ISF15ACP4_BUTTON_UI_HEADER_INCLUDED
#include "../src/isf15acp4_button_ui.ipp"
#undef ISF15ACP4_BUTTON_UI_HEADER_INCLUDED
