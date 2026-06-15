/**
 * @file isf15acp4_canvas.hpp
 * @brief Software framebuffer and 2D drawing primitives for SmartDisplay OLEDs.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "isf15acp4_types.hpp"

namespace isf15acp4 {

/**
 * @class Canvas
 * @brief CPU-side RGB565 framebuffer with basic drawing operations.
 *
 * The canvas stores pixels in row-major RGB565 order matching the SSD1331
 * download format for the selected product variant.
 */
class Canvas {
public:
    /**
     * @brief Construct a canvas for the given product geometry.
     * @param variant Target SmartDisplay variant.
     */
    explicit Canvas(ProductVariant variant = ProductVariant::Isf15acp4) noexcept;

    /** @brief Clear the framebuffer to a solid color. */
    void Clear(Color565 color) noexcept;

    /** @brief Set one pixel if in bounds. */
    void SetPixel(uint8_t x, uint8_t y, Color565 color) noexcept;

    /** @brief Get one pixel; returns black when out of bounds. */
    [[nodiscard]] Color565 GetPixel(uint8_t x, uint8_t y) const noexcept;

    /** @brief Draw a horizontal line inclusive of both endpoints. */
    void DrawHLine(uint8_t x0, uint8_t x1, uint8_t y, Color565 color) noexcept;

    /** @brief Draw a vertical line inclusive of both endpoints. */
    void DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, Color565 color) noexcept;

    /** @brief Draw rectangle outline. */
    void DrawRect(const Rect& rect, Color565 color) noexcept;

    /** @brief Fill a solid rectangle. */
    void FillRect(const Rect& rect, Color565 color) noexcept;

    /** @brief Draw a single character using the built-in 5x7 font. */
    void DrawChar(uint8_t x, uint8_t y, char ch, Color565 color, Color565 bg = colors::Black) noexcept;

    /** @brief Draw a null-terminated string. */
    void DrawString(uint8_t x, uint8_t y, const char* text, Color565 color,
                    Color565 bg = colors::Black) noexcept;

    /**
     * @brief Blit a contiguous RGB565 buffer into the canvas.
     * @param x Destination X.
     * @param y Destination Y.
     * @param src_width Source image width in pixels.
     * @param src_height Source image height in pixels.
     * @param src Source RGB565 pixels (row-major).
     */
    void BlitRgb565(uint8_t x, uint8_t y, uint8_t src_width, uint8_t src_height,
                    const Color565* src) noexcept;

    [[nodiscard]] uint8_t Width() const noexcept { return geometry_.width; }
    [[nodiscard]] uint8_t Height() const noexcept { return geometry_.height; }
    [[nodiscard]] std::size_t ByteSize() const noexcept { return geometry_.framebuffer_bytes; }
    [[nodiscard]] const Color565* Data() const noexcept { return buffer_; }
    [[nodiscard]] Color565* Data() noexcept { return buffer_; }

private:
    DisplayGeometry geometry_;
    Color565 buffer_[96 * 64];
};

}  // namespace isf15acp4

#define ISF15ACP4_CANVAS_HEADER_INCLUDED
#include "../src/isf15acp4_canvas.ipp"
#undef ISF15ACP4_CANVAS_HEADER_INCLUDED
