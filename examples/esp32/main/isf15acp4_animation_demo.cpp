/**
 * @file isf15acp4_animation_demo.cpp
 * @brief Procedural animation showcase with swappable graphics backend.
 */

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp32_isf15acp4_setup.hpp"
#include "isf15acp4.hpp"
#include "isf15acp4_animator.hpp"

namespace {

constexpr const char* kTag = "ISF15ACP4_ANIM";

esp32_isf15acp4::Esp32SpiAdapter* g_adapter = nullptr;
isf15acp4::SmartDisplay<esp32_isf15acp4::Esp32SpiAdapter>* g_display = nullptr;

}  // namespace

extern "C" void app_main(void) {
    static esp32_isf15acp4::Esp32SpiAdapter adapter(esp32_isf15acp4::default_bus_config(),
                                                    esp32_isf15acp4::default_pin_config());
    g_adapter = &adapter;

    isf15acp4::SmartDisplayConfig config =
        esp32_isf15acp4::default_display_config(isf15acp4::GraphicsBackend::Animator);

    static isf15acp4::SmartDisplay display(&adapter, config);
    g_display = &display;

    if (!display.Initialize(config.enable_vcc_on_init)) {
        ESP_LOGE(kTag, "Init failed");
        return;
    }

    isf15acp4::GraphicsContext gfx = display.CreateGraphicsContext();
    display.ConfigurePresenter(gfx);

    isf15acp4::animations::ScrollTextState scroll_state{.text = "HardFOC SmartDisplay"};
    isf15acp4::animations::PulseButtonState pulse_state{.pressed = false};

    isf15acp4::Animator wheel_anim(gfx, isf15acp4::animations::ColorWheel, nullptr, 20);
    isf15acp4::Animator plasma_anim(gfx, isf15acp4::animations::Plasma, nullptr, 25);
    isf15acp4::Animator scroll_anim(gfx, isf15acp4::animations::ScrollingText, &scroll_state, 30);
    isf15acp4::Animator pulse_anim(gfx, isf15acp4::animations::PulseButton, &pulse_state, 30);

    isf15acp4::Animator* active = &wheel_anim;
    int64_t last_us = esp_timer_get_time();
    bool last_pressed = false;

    ESP_LOGI(kTag, "Animation demo running — press button to cycle effects");

    while (true) {
        const int64_t now = esp_timer_get_time();
        const uint32_t elapsed_ms = static_cast<uint32_t>((now - last_us) / 1000);
        last_us = now;

        const bool pressed = display.IsPressed();
        pulse_state.pressed = pressed;

        if (pressed && !last_pressed) {
            if (active == &wheel_anim) {
                active = &plasma_anim;
            } else if (active == &plasma_anim) {
                active = &scroll_anim;
            } else if (active == &scroll_anim) {
                active = &pulse_anim;
            } else {
                active = &wheel_anim;
            }
            active->Reset();
            ESP_LOGI(kTag, "Switched animation profile");
        }
        last_pressed = pressed;

        if (active->Tick(elapsed_ms)) {
            display.Present(gfx);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
