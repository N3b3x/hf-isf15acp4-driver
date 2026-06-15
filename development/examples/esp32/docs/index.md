# ESP32 ISF15ACP4 Applications — Documentation

This folder describes the ESP32 applications that exercise the ISF15ACP4 SmartDisplay driver.

## Overview

| Document | Content |
|----------|---------|
| [Index (this file)](index.md) | Overview and links |
| [Comprehensive Test](comprehensive-test.md) | Full driver API validation |
| [UI Demo](ui-demo.md) | Press counter and live progress bar |
| [Animation Demo](animation-demo.md) | Procedural animations, press to cycle |
| [Button Dashboard](button-dashboard.md) | Motor RPM gauge and debounced UI |

## Driver Documentation

- **Main docs**: [../../../docs/](../../../docs/) — architecture, hardware, graphics, API reference
- **Root README**: [../../../README.md](../../../README.md)

## Applications Summary

| App | Purpose | When to use |
|-----|---------|-------------|
| **isf15acp4_comprehensive_test** | Validate driver and hardware | After wiring; CI; regression |
| **isf15acp4_ui_demo** | Interactive press counter | Quick HMI smoke test |
| **isf15acp4_animation_demo** | Animation backend showcase | Graphics engine demos |
| **isf15acp4_button_dashboard** | Gauge + debounced button | Reference motor-panel UI |

## Build and Flash

From `examples/esp32`:

```bash
./scripts/build_app.sh list
./scripts/build_app.sh isf15acp4_comprehensive_test Debug
./scripts/flash_app.sh flash_monitor isf15acp4_comprehensive_test Debug
```

See [../README.md](../README.md) for prerequisites, pin override (`menuconfig` or `sdkconfig`), and troubleshooting.
