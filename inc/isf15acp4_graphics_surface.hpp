/**
 * @file isf15acp4_graphics_surface.hpp
 * @brief Pluggable graphics surface interface for interchangeable render backends.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "isf15acp4_types.hpp"

namespace isf15acp4 {

/**
 * @enum GraphicsBackend
 * @brief Selectable graphics engine used by SmartDisplay and UI helpers.
 *
 * Choose at construction time via SmartDisplayConfig::backend or
 * GraphicsContext::Create().
 */
enum class GraphicsBackend : uint8_t {
    /** CPU RGB565 framebuffer with 5x7 font and software primitives. */
    BuiltinCanvas = 0,
    /** No framebuffer — uses SSD1331 hardware draw commands only. */
    HardwareDirect = 1,
    /** Builtin canvas plus frame sequencer for timed animations. */
    Animator = 2
};

/**
 * @brief CRTP interface every graphics backend must implement.
 *
 * Backends share a common drawing vocabulary so application code can target
 * SmartDisplay with any engine selected at construction time.
 *
 * @tparam Derived Concrete backend (Canvas, HardwareDirectSurface, AnimatorSurface).
 */
template <typename Derived>
class GraphicsSurface {
public:
    void Clear(Color565 color) noexcept { static_cast<Derived*>(this)->Clear(color); }
    void SetPixel(uint8_t x, uint8_t y, Color565 color) noexcept {
        static_cast<Derived*>(this)->SetPixel(x, y, color);
    }
    [[nodiscard]] Color565 GetPixel(uint8_t x, uint8_t y) const noexcept {
        return static_cast<const Derived*>(this)->GetPixel(x, y);
    }
    void DrawHLine(uint8_t x0, uint8_t x1, uint8_t y, Color565 color) noexcept {
        static_cast<Derived*>(this)->DrawHLine(x0, x1, y, color);
    }
    void DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, Color565 color) noexcept {
        static_cast<Derived*>(this)->DrawVLine(x, y0, y1, color);
    }
    void DrawRect(const Rect& rect, Color565 color) noexcept {
        static_cast<Derived*>(this)->DrawRect(rect, color);
    }
    void FillRect(const Rect& rect, Color565 color) noexcept {
        static_cast<Derived*>(this)->FillRect(rect, color);
    }
    void DrawChar(uint8_t x, uint8_t y, char ch, Color565 color, Color565 bg = colors::Black) noexcept {
        static_cast<Derived*>(this)->DrawChar(x, y, ch, color, bg);
    }
    void DrawString(uint8_t x, uint8_t y, const char* text, Color565 color,
                    Color565 bg = colors::Black) noexcept {
        static_cast<Derived*>(this)->DrawString(x, y, text, color, bg);
    }
    void BlitRgb565(uint8_t x, uint8_t y, uint8_t src_width, uint8_t src_height,
                    const Color565* src) noexcept {
        static_cast<Derived*>(this)->BlitRgb565(x, y, src_width, src_height, src);
    }

    [[nodiscard]] uint8_t Width() const noexcept { return static_cast<const Derived*>(this)->Width(); }
    [[nodiscard]] uint8_t Height() const noexcept { return static_cast<const Derived*>(this)->Height(); }
    [[nodiscard]] std::size_t ByteSize() const noexcept {
        return static_cast<const Derived*>(this)->ByteSize();
    }
    [[nodiscard]] const Color565* Data() const noexcept {
        return static_cast<const Derived*>(this)->Data();
    }
    [[nodiscard]] Color565* Data() noexcept { return static_cast<Derived*>(this)->Data(); }
    [[nodiscard]] GraphicsBackend Backend() const noexcept {
        return static_cast<const Derived*>(this)->Backend();
    }

protected:
    GraphicsSurface() = default;
    ~GraphicsSurface() = default;
};

}  // namespace isf15acp4
