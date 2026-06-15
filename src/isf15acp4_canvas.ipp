#ifndef ISF15ACP4_CANVAS_HEADER_INCLUDED
#error "isf15acp4_canvas.ipp must be included from isf15acp4_canvas.hpp"
#endif

#include "isf15acp4_font.hpp"

namespace isf15acp4 {

inline Canvas::Canvas(ProductVariant variant) noexcept : geometry_(GetGeometry(variant)) {
    Clear(colors::Black);
}

inline void Canvas::Clear(Color565 color) noexcept {
    for (std::size_t i = 0; i < geometry_.framebuffer_bytes / 2; ++i) {
        buffer_[i] = color;
    }
}

inline void Canvas::SetPixel(uint8_t x, uint8_t y, Color565 color) noexcept {
    if (x >= geometry_.width || y >= geometry_.height) {
        return;
    }
    buffer_[static_cast<std::size_t>(y) * geometry_.width + x] = color;
}

inline Color565 Canvas::GetPixel(uint8_t x, uint8_t y) const noexcept {
    if (x >= geometry_.width || y >= geometry_.height) {
        return colors::Black;
    }
    return buffer_[static_cast<std::size_t>(y) * geometry_.width + x];
}

inline void Canvas::DrawHLine(uint8_t x0, uint8_t x1, uint8_t y, Color565 color) noexcept {
    if (x0 > x1) {
        const uint8_t tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    for (uint8_t x = x0; x <= x1; ++x) {
        SetPixel(x, y, color);
    }
}

inline void Canvas::DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, Color565 color) noexcept {
    if (y0 > y1) {
        const uint8_t tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    for (uint8_t y = y0; y <= y1; ++y) {
        SetPixel(x, y, color);
    }
}

inline void Canvas::DrawRect(const Rect& rect, Color565 color) noexcept {
    DrawHLine(rect.x0, rect.x1, rect.y0, color);
    DrawHLine(rect.x0, rect.x1, rect.y1, color);
    DrawVLine(rect.x0, rect.y0, rect.y1, color);
    DrawVLine(rect.x1, rect.y0, rect.y1, color);
}

inline void Canvas::FillRect(const Rect& rect, Color565 color) noexcept {
    for (uint8_t y = rect.y0; y <= rect.y1; ++y) {
        DrawHLine(rect.x0, rect.x1, y, color);
    }
}

inline void Canvas::DrawChar(uint8_t x, uint8_t y, char ch, Color565 color, Color565 bg) noexcept {
    uint8_t cols[5] = {};
    if (!font::GetGlyph(ch, cols)) {
        return;
    }

    for (uint8_t col = 0; col < font::kWidth; ++col) {
        for (uint8_t row = 0; row < font::kHeight; ++row) {
            const bool on = (cols[col] >> row) & 0x01;
            SetPixel(static_cast<uint8_t>(x + col), static_cast<uint8_t>(y + row), on ? color : bg);
        }
    }
}

inline void Canvas::DrawString(uint8_t x, uint8_t y, const char* text, Color565 color,
                               Color565 bg) noexcept {
    if (text == nullptr) {
        return;
    }

    uint8_t cursor = x;
    while (*text != '\0') {
        DrawChar(cursor, y, *text, color, bg);
        cursor = static_cast<uint8_t>(cursor + font::kWidth + font::kSpacing);
        ++text;
    }
}

inline void Canvas::BlitRgb565(uint8_t x, uint8_t y, uint8_t src_width, uint8_t src_height,
                               const Color565* src) noexcept {
    if (src == nullptr) {
        return;
    }

    for (uint8_t row = 0; row < src_height; ++row) {
        for (uint8_t col = 0; col < src_width; ++col) {
            SetPixel(static_cast<uint8_t>(x + col), static_cast<uint8_t>(y + row),
                     src[static_cast<std::size_t>(row) * src_width + col]);
        }
    }
}

}  // namespace isf15acp4
