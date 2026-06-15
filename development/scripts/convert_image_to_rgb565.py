#!/usr/bin/env python3
"""Convert a PNG image to RGB565 C array for ISF15ACP4 SmartDisplay."""

from __future__ import annotations

import argparse
import pathlib
import struct
import sys

try:
    from PIL import Image
except ImportError:
    print("Install Pillow: pip install pillow", file=sys.stderr)
    raise


def rgb888_to_rgb565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def convert_png(path: pathlib.Path, width: int, height: int) -> list[int]:
    img = Image.open(path).convert("RGB")
    if img.size != (width, height):
        img = img.resize((width, height), Image.Resampling.LANCZOS)

    pixels: list[int] = []
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            pixels.append(rgb888_to_rgb565(r, g, b))
    return pixels


def emit_c_array(name: str, pixels: list[int], width: int, height: int) -> str:
    lines = [
        f"// Auto-generated from convert_image_to_rgb565.py",
        f"// Resolution: {width}x{height}, RGB565, {len(pixels) * 2} bytes",
        "#pragma once",
        "#include <cstdint>",
        "#include <cstddef>",
        "",
        f"inline constexpr uint8_t k{name}Width = {width};",
        f"inline constexpr uint8_t k{name}Height = {height};",
        f"inline constexpr std::size_t k{name}PixelCount = {len(pixels)};",
        "",
        f"inline constexpr uint16_t k{name}Rgb565[k{name}PixelCount] = {{",
    ]

    for i in range(0, len(pixels), 8):
        chunk = pixels[i : i + 8]
        hexes = ", ".join(f"0x{v:04X}" for v in chunk)
        lines.append(f"    {hexes},")

    lines.append("};")
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=pathlib.Path, help="Source PNG")
    parser.add_argument("-o", "--output", type=pathlib.Path, required=True, help="Output .hpp")
    parser.add_argument("-n", "--name", default="DemoImage", help="C symbol base name")
    parser.add_argument("-W", "--width", type=int, default=96)
    parser.add_argument("-H", "--height", type=int, default=64)
    args = parser.parse_args()

    pixels = convert_png(args.input, args.width, args.height)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(emit_c_array(args.name, pixels, args.width, args.height), encoding="utf-8")
    print(f"Wrote {args.output} ({len(pixels)} pixels)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
