/**
 * @file isf15acp4_ui_demo.cpp
 * @brief Interactive UI demo for the ISF15ACP4 SmartDisplay on ESP32.
 */
#include <cstdio>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp32_isf15acp4_setup.hpp"
#include "isf15acp4.hpp"

namespace {
constexpr const char* kTag = "ISF15ACP4_UI";
}

extern "C" void app_main(void) {
    esp32_isf15acp4::Esp32SpiAdapter adapter(esp32_isf15acp4::default_bus_config(),
                                             esp32_isf15acp4::default_pin_config());
    isf15acp4::SmartDisplay display(&adapter, isf15acp4::ProductVariant::Isf15acp4);
    isf15acp4::Canvas canvas(isf15acp4::ProductVariant::Isf15acp4);

    if (!display.Initialize(true)) {
        ESP_LOGE(kTag, "Initialization failed");
        return;
    }

    uint32_t frame = 0;
    uint32_t press_count = 0;
    bool last_pressed = false;

    while (true) {
        const bool pressed = display.IsPressed();
        if (pressed && !last_pressed) {
            ++press_count;
        }
        last_pressed = pressed;

        canvas.Clear(isf15acp4::colors::Black);
        canvas.DrawString(6, 4, "NKK SmartDisplay", isf15acp4::colors::White);
        canvas.DrawString(6, 14, "ISF15ACP4 96x64", isf15acp4::colors::Cyan);

        char line[32];
        snprintf(line, sizeof(line), "Frame: %lu", static_cast<unsigned long>(frame));
        canvas.DrawString(6, 28, line, isf15acp4::colors::Yellow);

        snprintf(line, sizeof(line), "Presses: %lu", static_cast<unsigned long>(press_count));
        canvas.DrawString(6, 38, line, isf15acp4::colors::Green);

        canvas.DrawString(6, 50, pressed ? "BUTTON: ON " : "BUTTON: OFF", isf15acp4::colors::Orange);

        const uint8_t bar_x = static_cast<uint8_t>((frame % 70) + 10);
        canvas.FillRect({bar_x, 58, static_cast<uint8_t>(bar_x + 8), 62}, isf15acp4::colors::Magenta);

        display.DrawCanvas(canvas);
        ++frame;
        vTaskDelay(pdMS_TO_TICKS(33));
    }
}
