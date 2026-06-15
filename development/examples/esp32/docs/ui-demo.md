# ISF15ACP4 UI Demo

**App:** `isf15acp4_ui_demo`  
**Source:** `main/isf15acp4_ui_demo.cpp`

## Purpose

Interactive demo using the `Canvas` API directly:

- Frame counter
- Debounced press counter
- Live button ON/OFF label
- Animated magenta progress bar

## Build

```bash
./scripts/build_app.sh isf15acp4_ui_demo Debug
./scripts/flash_app.sh flash_monitor isf15acp4_ui_demo Debug
```

## Notes

Runs at ~30 FPS (`vTaskDelay(33 ms)`). Uses shared pin defaults from `esp32_isf15acp4_test_config.hpp`.
