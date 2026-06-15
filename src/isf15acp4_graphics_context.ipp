#ifndef ISF15ACP4_GRAPHICS_CONTEXT_HEADER_INCLUDED
#error "isf15acp4_graphics_context.ipp must be included from isf15acp4_graphics_context.hpp"
#endif

namespace isf15acp4 {

inline GraphicsContext::GraphicsContext(ProductVariant variant, GraphicsBackend backend) noexcept
    : variant_(variant), backend_(backend), canvas_(variant) {}

inline void GraphicsContext::Clear(Color565 color) noexcept {
    if (backend_ == GraphicsBackend::HardwareDirect && presenter_.fill_screen) {
        presenter_.fill_screen(presenter_.context, color);
        return;
    }
    canvas_.Clear(color);
}

inline void GraphicsContext::SetPixel(uint8_t x, uint8_t y, Color565 color) noexcept {
    if (backend_ == GraphicsBackend::HardwareDirect && presenter_.draw_pixel) {
        presenter_.draw_pixel(presenter_.context, x, y, color);
        return;
    }
    canvas_.SetPixel(x, y, color);
}

inline Color565 GraphicsContext::GetPixel(uint8_t x, uint8_t y) const noexcept {
    return canvas_.GetPixel(x, y);
}

inline void GraphicsContext::DrawHLine(uint8_t x0, uint8_t x1, uint8_t y, Color565 color) noexcept {
    if (backend_ == GraphicsBackend::HardwareDirect && presenter_.draw_line) {
        presenter_.draw_line(presenter_.context, {x0, y}, {x1, y}, color);
        return;
    }
    canvas_.DrawHLine(x0, x1, y, color);
}

inline void GraphicsContext::DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, Color565 color) noexcept {
    if (backend_ == GraphicsBackend::HardwareDirect && presenter_.draw_line) {
        presenter_.draw_line(presenter_.context, {x, y0}, {x, y1}, color);
        return;
    }
    canvas_.DrawVLine(x, y0, y1, color);
}

inline void GraphicsContext::DrawRect(const Rect& rect, Color565 color) noexcept {
    if (backend_ == GraphicsBackend::HardwareDirect && presenter_.draw_rect) {
        presenter_.draw_rect(presenter_.context, rect, color, false);
        return;
    }
    canvas_.DrawRect(rect, color);
}

inline void GraphicsContext::FillRect(const Rect& rect, Color565 color) noexcept {
    if (backend_ == GraphicsBackend::HardwareDirect && presenter_.draw_rect) {
        presenter_.draw_rect(presenter_.context, rect, color, true);
        return;
    }
    canvas_.FillRect(rect, color);
}

inline void GraphicsContext::DrawChar(uint8_t x, uint8_t y, char ch, Color565 color,
                                      Color565 bg) noexcept {
    canvas_.DrawChar(x, y, ch, color, bg);
}

inline void GraphicsContext::DrawString(uint8_t x, uint8_t y, const char* text, Color565 color,
                                        Color565 bg) noexcept {
    canvas_.DrawString(x, y, text, color, bg);
}

inline void GraphicsContext::BlitRgb565(uint8_t x, uint8_t y, uint8_t src_width, uint8_t src_height,
                                        const Color565* src) noexcept {
    canvas_.BlitRgb565(x, y, src_width, src_height, src);
}

}  // namespace isf15acp4
