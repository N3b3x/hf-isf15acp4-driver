---
layout: default
title: "ISF15ACP4 Project Setup"
description: "ESP-IDF project setup for the HF-ISF15ACP4 driver examples"
nav_order: 3
parent: "Getting Started"
permalink: /docs/getting-started/project-setup/
---

# ISF15ACP4 ESP32 Project Setup

This project ships a complete ESP-IDF example tree for the [HF-ISF15ACP4 driver](https://github.com/N3b3x/hf-isf15acp4-driver).

## Repository layout

```
hf-isf15acp4-driver/
  examples/esp32/
    app_config.yml          # CI build matrix (4 apps)
    sdkconfig               # Committed baseline (ESP32-S3)
    sdkconfig.defaults      # ISF15ACP4 GPIO defaults
    components/hf_isf15acp4 # ESP-IDF component wrapper + Kconfig
    main/
      esp32_isf15acp4_test_config.hpp
      esp32_isf15acp4_setup.hpp
      TestFramework.h
      isf15acp4_*.cpp       # Four applications
    docs/                   # Per-app documentation
    scripts/                # hf-espidf-project-tools (submodule)
```

## Quick setup

```bash
git clone --recursive https://github.com/N3b3x/hf-isf15acp4-driver.git
cd hf-isf15acp4-driver/examples/esp32
./scripts/build_app.sh list
./scripts/build_app.sh isf15acp4_comprehensive_test Debug
```

If submodules were not cloned:

```bash
git submodule update --init --recursive
```

## Pin configuration

Default GPIO map (ESP32-S3 example wiring):

| Signal | Default GPIO | Kconfig symbol |
|--------|--------------|----------------|
| MOSI | 11 | `CONFIG_ISF15ACP4_PIN_MOSI` |
| SCK | 12 | `CONFIG_ISF15ACP4_PIN_SCLK` |
| CS (software SS) | 10 | `CONFIG_ISF15ACP4_PIN_CS` |
| D/C | 13 | `CONFIG_ISF15ACP4_PIN_DC` |
| RES | 14 | `CONFIG_ISF15ACP4_PIN_RES` |
| VCC enable | 9 | `CONFIG_ISF15ACP4_PIN_VCC_EN` |
| Switch | 8 | `CONFIG_ISF15ACP4_PIN_SWITCH` |

Override via `idf.py menuconfig` → **HF ISF15ACP4 SmartDisplay**, or edit `sdkconfig.defaults`.

## Hardware

Wire the module per [Hardware Setup](../../../docs/hardware_setup.md). The OLED requires **SPI mode 0** and a **VCC enable** sequence for the on-module charge pump.

## CI integration

All four apps have `ci_enabled: true` in `app_config.yml`. GitHub Actions workflow `esp32-examples-build-ci.yml` validates the matrix using `hf-espidf-ci-tools`.

## hf-core handler test

For platform integration, enable `HF_CORE_ENABLE_ISF15ACP4` in `hf-core/examples/esp32/components/hf_core/CMakeLists.txt` and build `isf15acp4_handler_test`.
