# Changelog

## 0.1 (unreleased)

First public build of the native port. It is an early alpha: expect bugs.

- The whole original game — resident program and every mode module (Grand
  Prix, Time Attack, VS battle, Extra Trial, garage and emblem editor,
  records, options, movie playback) — runs natively on 64-bit macOS and
  Linux on our PsyZ fork, from your own **European (PAL) CUE/BIN** disc
  image of *Ridge Racer Type 4* (`SCES-017.06`). Game data is not included.
- Boots through the opening movie to the title screen and main menu; the
  modes, races and the attract demo have been checked frame by frame against
  DuckStation reference runs. At 1x internal resolution frames are drawn
  bit-exactly like the PS1 GPU.
- Sequenced music, sound effects, CD audio and movie audio play through the
  emulated SPU, with the front-end and races kept on the PS1's field timing.
- Memory cards are kept as `bu00`/`bu10` folders (in the working directory,
  or beside the preferences when the macOS app is started from Finder).
- **Escape** opens a settings menu: internal resolution (1x to 8x),
  fullscreen, VSync, aspect ratio, dithering, antialiasing and experimental
  enhancements; keyboard and gamepad bindings; master volume; and the disc
  image. Settings are saved to `ridge-racer-type-4.toml` (portable when that
  file sits beside the executable or the app).
- Without a disc argument the game starts the image chosen last time, or asks
  for the CUE file with a file dialog.
- Release packages: a signed and notarized macOS app (Apple Silicon) and
  Linux x86-64 and ARM64 builds.

### Known limitations

- Serial link (two-console) play is not supported.
- Texture filtering, geometry correction and frame interpolation are
  experimental and off by default; higher internal resolutions use the GPU
  rasterizer and are not bit-exact.
- Only the European (PAL) release is supported; other regions and CHD images
  are not.
- No Windows build yet: the game code still assumes that `long` holds a
  pointer, which 64-bit Windows does not provide.
