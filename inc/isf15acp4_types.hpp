/**
 * @file isf15acp4_types.hpp
 * @brief Shared types, constants, and color utilities for NKK SmartDisplay OLED drivers.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

namespace isf15acp4 {

/**
 * @brief Supported NKK SmartDisplay OLED product variants.
 *
 * All variants share the SSD1331 controller but use different initialization
 * tables, memory windows, and visible pixel areas.
 */
enum class ProductVariant : uint8_t {
    Isf15acp4 = 0,  ///< Frameless OLED pushbutton, 96x64 (ISF15ACP4)
    Isc15anp4 = 1,  ///< Standard OLED pushbutton, 64x48 (ISC15ANP4)
    Isc01p = 2      ///< OLED display module, 52x36 (ISC01P)
};

/**
 * @brief Driver error flags (bitmask).
 */
enum class Error : uint16_t {
    None = 0,
    SpiWrite = 1 << 0,
    GpioControl = 1 << 1,
    InvalidParam = 1 << 2,
    NotInitialized = 1 << 3,
    OutOfRange = 1 << 4,
    PowerSequence = 1 << 5
};

/**
 * @brief Display power/visibility state.
 */
enum class DisplayState : uint8_t {
    Off = 0,
    On = 1,
    Dim = 2
};

/**
 * @brief 16-bit RGB565 color value (R5G6B5).
 */
using Color565 = uint16_t;

/** @brief Compile-time display geometry for a product variant. */
struct DisplayGeometry {
    uint8_t width;
    uint8_t height;
    uint8_t column_start;
    uint8_t column_end;
    uint8_t row_start;
    uint8_t row_end;
    std::size_t framebuffer_bytes;
};

// Namespace-scope so GetGeometry stays C++20 constexpr (function-local
// static constexpr is C++23).
inline constexpr DisplayGeometry kGeometryIsf15acp4{96, 64, 0x00, 0x5F, 0x00, 0x3F,
                                                    96u * 64u * 2u};
inline constexpr DisplayGeometry kGeometryIsc15anp4{64, 48, 0x10, 0x4F, 0x00, 0x2F,
                                                    64u * 48u * 2u};
inline constexpr DisplayGeometry kGeometryIsc01p{52, 36, 0x16, 0x49, 0x00, 0x23,
                                                 52u * 36u * 2u};

/**
 * @brief Get geometry metadata for a product variant.
 * @param variant Product variant identifier.
 * @return Const reference to static geometry descriptor.
 */
inline constexpr const DisplayGeometry& GetGeometry(ProductVariant variant) noexcept {
    switch (variant) {
        case ProductVariant::Isf15acp4:
            return kGeometryIsf15acp4;
        case ProductVariant::Isc15anp4:
            return kGeometryIsc15anp4;
        case ProductVariant::Isc01p:
            return kGeometryIsc01p;
    }
    return kGeometryIsf15acp4;
}

/** @brief 2D point in display coordinates. */
struct Point {
    uint8_t x;
    uint8_t y;
};

/** @brief Axis-aligned rectangle in display coordinates (inclusive bounds). */
struct Rect {
    uint8_t x0;
    uint8_t y0;
    uint8_t x1;
    uint8_t y1;
};

/**
 * @brief Build an RGB565 color from 8-bit RGB channels.
 * @param r Red channel (0-255).
 * @param g Green channel (0-255).
 * @param b Blue channel (0-255).
 * @return Packed RGB565 color.
 */
inline constexpr Color565 MakeColor565(uint8_t r, uint8_t g, uint8_t b) noexcept {
    return static_cast<Color565>(((r & 0xF8u) << 8) | ((g & 0xFCu) << 3) | (b >> 3));
}

/** @brief Common preset colors. */
namespace colors {
inline constexpr Color565 Black = MakeColor565(0, 0, 0);
inline constexpr Color565 White = MakeColor565(255, 255, 255);
inline constexpr Color565 Red = MakeColor565(255, 0, 0);
inline constexpr Color565 Green = MakeColor565(0, 255, 0);
inline constexpr Color565 Blue = MakeColor565(0, 0, 255);
inline constexpr Color565 Yellow = MakeColor565(255, 255, 0);
inline constexpr Color565 Cyan = MakeColor565(0, 255, 255);
inline constexpr Color565 Magenta = MakeColor565(255, 0, 255);
inline constexpr Color565 Orange = MakeColor565(255, 165, 0);
inline constexpr Color565 Gray = MakeColor565(128, 128, 128);
}  // namespace colors

/**
 * @brief Convert a 24-bit BMP pixel (BGR byte order) to RGB565.
 * @param bgr Pointer to three bytes in B, G, R order.
 * @return RGB565 color suitable for SSD1331 65k mode.
 */
inline Color565 BmpPixelToColor565(const uint8_t* bgr) noexcept {
    return MakeColor565(bgr[2], bgr[1], bgr[0]);
}

}  // namespace isf15acp4
