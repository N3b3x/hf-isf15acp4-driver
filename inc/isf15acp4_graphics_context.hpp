/**
 * @file isf15acp4_graphics_context.hpp
 * @brief Runtime-selectable graphics backend for SmartDisplay applications.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "isf15acp4_canvas.hpp"
#include "isf15acp4_graphics_surface.hpp"

namespace isf15acp4 {

/**
 * @brief Type-erased presenter used by HardwareDirect backend.
 *
 * SmartDisplay registers itself via BindPresenter() so hardware-direct
 * drawing can forward without virtual calls in the hot SPI path.
 */
struct DisplayPresenter {
    bool (*draw_pixel)(void* ctx, uint8_t x, uint8_t y, Color565 color) noexcept{nullptr};
    bool (*draw_line)(void* ctx, Point a, Point b, Color565 color) noexcept{nullptr};
    bool (*draw_rect)(void* ctx, Rect r, Color565 color, bool filled) noexcept{nullptr};
    bool (*fill_screen)(void* ctx, Color565 color) noexcept{nullptr};
    bool (*flush_canvas)(void* ctx, const Color565* data, std::size_t bytes) noexcept{nullptr};
    void* context{nullptr};
};

/**
 * @class GraphicsContext
 * @brief Owns the active graphics backend selected at construction time.
 *
 * @code
 * GraphicsContext gfx(ProductVariant::Isf15acp4, GraphicsBackend::BuiltinCanvas);
 * gfx.Clear(colors::Black);
 * gfx.DrawString(4, 4, "Ready", colors::White);
 * display.Present(gfx);
 * @endcode
 */
class GraphicsContext : public GraphicsSurface<GraphicsContext> {
public:
    GraphicsContext(ProductVariant variant, GraphicsBackend backend) noexcept;

    [[nodiscard]] GraphicsBackend Backend() const noexcept { return backend_; }
    [[nodiscard]] ProductVariant Variant() const noexcept { return variant_; }

    /** @brief Access underlying canvas (BuiltinCanvas / Animator backends). */
    [[nodiscard]] Canvas& CanvasBuffer() noexcept { return canvas_; }
    [[nodiscard]] const Canvas& CanvasBuffer() const noexcept { return canvas_; }

    /** @brief Bind SmartDisplay for HardwareDirect immediate-mode drawing. */
    void BindPresenter(const DisplayPresenter& presenter) noexcept { presenter_ = presenter; }

    void Clear(Color565 color) noexcept;
    void SetPixel(uint8_t x, uint8_t y, Color565 color) noexcept;
    [[nodiscard]] Color565 GetPixel(uint8_t x, uint8_t y) const noexcept;
    void DrawHLine(uint8_t x0, uint8_t x1, uint8_t y, Color565 color) noexcept;
    void DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, Color565 color) noexcept;
    void DrawRect(const Rect& rect, Color565 color) noexcept;
    void FillRect(const Rect& rect, Color565 color) noexcept;
    void DrawChar(uint8_t x, uint8_t y, char ch, Color565 color, Color565 bg = colors::Black) noexcept;
    void DrawString(uint8_t x, uint8_t y, const char* text, Color565 color,
                    Color565 bg = colors::Black) noexcept;
    void BlitRgb565(uint8_t x, uint8_t y, uint8_t src_width, uint8_t src_height,
                      const Color565* src) noexcept;

    [[nodiscard]] uint8_t Width() const noexcept { return canvas_.Width(); }
    [[nodiscard]] uint8_t Height() const noexcept { return canvas_.Height(); }
    [[nodiscard]] std::size_t ByteSize() const noexcept { return canvas_.ByteSize(); }
    [[nodiscard]] const Color565* Data() const noexcept { return canvas_.Data(); }
    [[nodiscard]] Color565* Data() noexcept { return canvas_.Data(); }

private:
    ProductVariant variant_;
    GraphicsBackend backend_;
    Canvas canvas_;
    DisplayPresenter presenter_{};
};

}  // namespace isf15acp4

#define ISF15ACP4_GRAPHICS_CONTEXT_HEADER_INCLUDED
#include "../src/isf15acp4_graphics_context.ipp"
#undef ISF15ACP4_GRAPHICS_CONTEXT_HEADER_INCLUDED
