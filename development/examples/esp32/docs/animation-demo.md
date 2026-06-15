# ISF15ACP4 Animation Demo

**App:** `isf15acp4_animation_demo`  
**Source:** `main/isf15acp4_animation_demo.cpp`

## Purpose

Showcases the **Animator** graphics backend:

1. Color wheel
2. Plasma
3. Scrolling text
4. Pulse

Press the integrated button to cycle animations.

## Build

```bash
./scripts/build_app.sh isf15acp4_animation_demo Debug
./scripts/flash_app.sh flash_monitor isf15acp4_animation_demo Debug
```

## Configuration

Set `GraphicsBackend::Animator` in `SmartDisplayConfig` (see source). Pin map via `menuconfig` or `esp32_isf15acp4_test_config.hpp`.
