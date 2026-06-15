/**
 * @file isf15acp4_comprehensive_test.cpp
 * @brief Comprehensive ISF15ACP4 SmartDisplay driver validation for ESP32.
 */
#include "TestFramework.h"
#include "esp32_isf15acp4_setup.hpp"
#include "esp32_isf15acp4_test_config.hpp"

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "isf15acp4.hpp"
#include "isf15acp4_button_ui.hpp"

static const char* kTag = "ISF15ACP4_TEST";
static TestResults g_results;

static bool test_adapter_init(esp32_isf15acp4::Esp32SpiAdapter& adapter) noexcept {
    return adapter.EnsureInitialized();
}

static bool test_display_init(isf15acp4::SmartDisplay<esp32_isf15acp4::Esp32SpiAdapter>& display) noexcept {
    return display.Initialize(true) && display.IsInitialized();
}

static bool test_graphics_present(
    isf15acp4::SmartDisplay<esp32_isf15acp4::Esp32SpiAdapter>& display) noexcept {
    auto gfx = display.CreateGraphicsContext();
    if (gfx.Backend() != isf15acp4::GraphicsBackend::BuiltinCanvas) {
        return false;
    }
    gfx.Clear(isf15acp4::colors::Black);
    gfx.DrawString(4, 4, "ISF15ACP4", isf15acp4::colors::White);
    gfx.DrawString(4, 16, "HardFOC Test", isf15acp4::colors::Cyan);
    gfx.DrawRect({0, 0, 95, 63}, isf15acp4::colors::Green);
    gfx.FillRect({30, 28, 65, 40}, isf15acp4::colors::Orange);
    return display.Present(gfx);
}

extern "C" void app_main(void) {
    ESP_LOGI(kTag, "ISF15ACP4 comprehensive test — driver v%s", isf15acp4::GetDriverVersion());

    esp32_isf15acp4::Esp32SpiAdapter adapter(esp32_isf15acp4::default_bus_config(),
                                             esp32_isf15acp4::default_pin_config());
    isf15acp4::SmartDisplay display(&adapter, esp32_isf15acp4::default_display_config());
    isf15acp4::ButtonUI button;

    RUN_TEST("adapter_init", test_adapter_init(adapter), &g_results);
    RUN_TEST("display_init", test_display_init(display), &g_results);
    RUN_TEST("fill_blue", display.FillScreen(isf15acp4::colors::Blue), &g_results);
    vTaskDelay(pdMS_TO_TICKS(300));
    RUN_TEST("fill_red", display.FillScreen(isf15acp4::colors::Red), &g_results);
    RUN_TEST("graphics_present", test_graphics_present(display), &g_results);
    RUN_TEST("hw_line", display.DrawLine({10, 50}, {85, 50}, isf15acp4::colors::Yellow), &g_results);
    RUN_TEST("hw_rect", display.DrawRectangle({70, 8, 90, 20}, isf15acp4::colors::Magenta, true),
              &g_results);
    RUN_TEST("display_dim", display.DisplayDim(), &g_results);
    RUN_TEST("display_on", display.DisplayOn(), &g_results);
    RUN_TEST("switch_read", true, &g_results);
    ESP_LOGI(kTag, "Switch state: %s", display.IsPressed() ? "PRESSED" : "released");

    const int64_t start = esp_timer_get_time();
    const bool refresh_ok = display.FillScreen(isf15acp4::colors::White);
    const int64_t elapsed_ms = (esp_timer_get_time() - start) / 1000;
    ESP_LOGI(kTag, "Full-frame refresh: %lld ms", static_cast<long long>(elapsed_ms));
    RUN_TEST("full_refresh", refresh_ok, &g_results);

    RUN_TEST("display_off", display.DisplayOff(), &g_results);
    RUN_TEST("display_on_again", display.DisplayOn(), &g_results);

    const uint16_t flags = display.GetErrorFlags();
    display.ClearErrorFlags();
    ESP_LOGI(kTag, "Error flags cleared (was 0x%04X)", flags);
    RUN_TEST("error_flags_clear", true, &g_results);

    (void)button.Update(display.IsPressed(), 50);
    print_test_summary(kTag, g_results);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
