# Ridge Racer Type 4

A native PC port of *Ridge Racer Type 4* (PAL, `SCES-017.06`), built from the
original game code with [our PsyZ fork](https://github.com/khasinski/parasite-eve-psyz).

## Status

The port is not playable yet. The whole original game (resident code and all
mode overlays) builds natively for 64-bit hosts on PsyZ and boots from the
disc image through the opening movie to the title screen and main menu; the
attract demo race renders. Races, the remaining menus and audio are being
brought up. See the [port plan](docs/PC_PORT_PLAN.md) for the milestones.

## Play

You need your own **European (PAL) CUE/BIN image** of *Ridge Racer Type 4*.
Game data is not included. Keep the CUE file next to its BIN file.

Start the game with the CUE file:

```sh
./build/ridge-racer-type-4-pc "Ridge Racer Type 4 (Europe).cue"
```

or set `RR4_DISC` to its path. Without either, the game starts the image you
chose last time, or asks for the CUE file with a file dialog. Memory cards
are kept as `bu00`/`bu10` directories in the working directory; the macOS app
started from Finder keeps them beside its preferences instead.

Release builds are on the [releases page](https://github.com/khasinski/ridge-racer-type-4-pc/releases):
the macOS app (`Ridge Racer Type 4.app`) is signed and notarized.

Default keys: arrows steer and move through menus, `X` is Cross, `Z` Square,
`D` Circle, `S` Triangle, `Q`/`R` L1/R1, `W`/`E` L2/R2, `Enter` Start and
`Backspace` Select. A connected gamepad works as pad 1 alongside the keyboard.
`F4` toggles fullscreen.

**Escape** (or the gamepad's Guide button) pauses the game and opens the
settings menu; Escape again resumes. Game time stops and audio is paused
while it is open. Tabs (`Tab`/`PgUp`/`PgDn`, gamepad shoulders):

- **Graphics** — internal resolution (1x to 8x), windowed or fullscreen,
  VSync, aspect ratio, dithering, antialiasing, and experimental texture
  filtering, geometry correction and frame interpolation (off by default).
- **Controls** — every keyboard and gamepad binding with its default; `Enter`
  rebinds (a binding already in use is swapped), `Backspace` restores the
  default.
- **Audio** — master volume and mute. Music and effect levels are in the
  game's own Options.
- **Disc** — the image in the drive and the image to start with next time.

Each tab also has *Restore page defaults*, *Resume game* and *Quit game*.
Changes apply at once and are saved to `ridge-racer-type-4.toml`: in
`RR4_CONFIG_HOME` when that is set, else next to the executable when a
`ridge-racer-type-4.toml` already exists there (portable), else in the user
configuration directory (`~/.config/ridge-racer-type-4` on Linux,
`~/Library/Application Support/Ridge Racer Type 4` on macOS,
`%APPDATA%\Ridge Racer Type 4` on Windows).

## Build

Requires Git, CMake 3.21 or newer and a C11 compiler.

```sh
git clone https://github.com/khasinski/ridge-racer-type-4-pc
cd ridge-racer-type-4-pc
git submodule update --init third_party/psyz
git -C third_party/psyz submodule update --init external/SDL
cmake -S . -B build -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --parallel
./build/ridge-racer-type-4-pc
```

`make`, `make run` and `make test` wrap the same CMake build.
On macOS, `-DRR4_MACOS_BUNDLE=ON` builds `Ridge Racer Type 4.app` instead of
a bare executable. Packaging and signing are described in
[releasing](docs/RELEASING.md).

Automated and test runs are silent and windowless:

```sh
PSYZ_HEADLESS=1 SDL_AUDIODRIVER=dummy RR4_FRAME_LIMIT=600 \
    RR4_CAPTURE=frame.ppm ./build/ridge-racer-type-4-pc game.cue
```

`RR4_FRAME_LIMIT` stops after that many VSyncs, `RR4_CAPTURE` / `RR4_VRAM`
write the displayed frame / VRAM as PPM on exit and `RR4_TRACE=n` prints the
game's mode state every `n` VSyncs. Headless runs never read or write
preferences and never show a dialog; `RR4_INTERNAL_RES=n` sets the
resolution for one run (`PSYZ_HEADLESS=gpu` keeps the real GPU renderer, so
captures are `n` times larger) and `RR4_SETTINGS_SCRIPT` drives the Escape
menu (see `src/port/settings_overlay.h`).
`RR4_WATCH` (print globals with the trace) and `RR4_POKE` (set them) look the
game's globals up by name in the executable: macOS builds export them, Linux
and Windows builds only when configured with `-DRR4_EXPORT_SYMBOLS=ON`. All
diagnostics are listed at the top of `src/port/main.c`.

## Layout

- `src/main/` — resident game code: system and main loop, graphics and GTE
  renderers, course, car physics and AI, race flow, camera, effects, HUD/UI,
  menus, sound, disc and save data.
- `src/overlays/` — the mode modules (`grand_prix`, `time_attack`, `versus`,
  `garage`, `records`, `options`, `extra_trial`, `serial_link`,
  `movie_playback`), linked statically with explicit dispatch.
- `src/port/` — host entry point, preferences and the Escape settings menu
  (`tests/port/` checks the menu headlessly).
- `include/` — game types, data formats and subsystem interfaces.
- `third_party/psyz/` — PsyZ and its SDL3 dependency.
- `docs/` — architecture, code organization, conventions, port plan and
  format notes.

For development details, see [contributing](CONTRIBUTING.md),
[architecture](docs/ARCHITECTURE.md), [code organization](docs/CODE_ORGANIZATION.md)
and [conventions](docs/CONVENTIONS.md).

This repository contains source only. Disc images, `R4.BIN`, `R4.STR`, memory
cards and proprietary SDK files are not distributed.
