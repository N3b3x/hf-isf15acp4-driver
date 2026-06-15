/**
 * @file isf15acp4_font.hpp
 * @brief Compact 5x7 bitmap font for SmartDisplay text rendering.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

namespace isf15acp4::font {

inline constexpr uint8_t kWidth = 5;
inline constexpr uint8_t kHeight = 7;
inline constexpr uint8_t kSpacing = 1;

/** @brief Glyph bitmap for ASCII 32 (' ') through 126 ('~'). */
extern const uint8_t kGlyphData[95][5];

/**
 * @brief Get glyph columns for an ASCII character.
 * @param ch Character code.
 * @param out_cols Receives five column bytes (LSB = top pixel).
 * @return true when the character is printable.
 */
bool GetGlyph(char ch, uint8_t out_cols[5]) noexcept;

}  // namespace isf15acp4::font
