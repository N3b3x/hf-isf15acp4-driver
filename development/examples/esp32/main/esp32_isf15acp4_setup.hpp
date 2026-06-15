/**
 * @file esp32_isf15acp4_setup.hpp
 * @brief Factory helpers for ISF15ACP4 SPI adapter and SmartDisplay config.
 */
#pragma once

#include "driver/spi_master.h"
#include "esp32_isf15acp4_spi_adapter.hpp"
#include "esp32_isf15acp4_test_config.hpp"
#include "isf15acp4.hpp"

namespace esp32_isf15acp4 {

inline SpiBusConfig default_bus_config() noexcept {
    return SpiBusConfig{
        .host = static_cast<spi_host_device_t>(CONFIG_ISF15ACP4_SPI_HOST),
        .mosi = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_MOSI),
        .sclk = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_SCLK),
        .clock_hz = ISF15ACP4_SPI_CLOCK_HZ,
    };
}

inline PinConfig default_pin_config() noexcept {
    return PinConfig{
        .cs = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_CS),
        .dc = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_DC),
        .reset = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_RES),
        .vcc_enable = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_VCC_EN),
        .switch_in = static_cast<gpio_num_t>(CONFIG_ISF15ACP4_PIN_SWITCH),
        .switch_active_low = true,
    };
}

inline isf15acp4::SmartDisplayConfig default_display_config(
    isf15acp4::GraphicsBackend backend = isf15acp4::GraphicsBackend::BuiltinCanvas) noexcept {
    return isf15acp4::SmartDisplayConfig{
        .variant = isf15acp4::ProductVariant::Isf15acp4,
        .graphics = backend,
        .enable_vcc_on_init = true,
    };
}

}  // namespace esp32_isf15acp4
