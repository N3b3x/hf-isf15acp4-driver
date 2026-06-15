/**
 * @file TestFramework.h
 * @brief Lightweight test harness for ISF15ACP4 ESP32 driver examples.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#ifdef __cplusplus
}
#endif

#include <cstdint>

struct TestResults {
    uint32_t passed{0};
    uint32_t failed{0};
    uint32_t total{0};
};

#define RUN_TEST(test_name, test_func, results_ptr)                                                \
    do {                                                                                           \
        ++(results_ptr)->total;                                                                    \
        if ((test_func)) {                                                                         \
            ++(results_ptr)->passed;                                                               \
            ESP_LOGI("TestFramework", "PASS: %s", (test_name));                                     \
        } else {                                                                                   \
            ++(results_ptr)->failed;                                                               \
            ESP_LOGE("TestFramework", "FAIL: %s", (test_name));                                   \
        }                                                                                          \
    } while (0)

inline void print_test_summary(const char* tag, const TestResults& results) noexcept {
    ESP_LOGI(tag, "=== TEST SUMMARY: %lu passed, %lu failed, %lu total ===",
             static_cast<unsigned long>(results.passed),
             static_cast<unsigned long>(results.failed),
             static_cast<unsigned long>(results.total));
}
