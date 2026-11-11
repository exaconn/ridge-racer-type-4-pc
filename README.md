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

or set `RR4_DISC` to its path. Memory cards are kept as `bu00`/`bu10`
directories in the working directory.

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

Automated and test runs are silent and windowless:

```sh
PSYZ_HEADLESS=1 SDL_AUDIODRIVER=dummy RR4_FRAME_LIMIT=600 \
    RR4_CAPTURE=frame.ppm ./build/ridge-racer-type-4-pc game.cue
```

`RR4_FRAME_LIMIT` stops after that many VSyncs, `RR4_CAPTURE` / `RR4_VRAM`
write the displayed frame / VRAM as PPM on exit and `RR4_TRACE=n` prints the
game's mode state every `n` VSyncs.

## Layout

- `src/main/` — resident game code: system and main loop, graphics and GTE
  renderers, course, car physics and AI, race flow, camera, effects, HUD/UI,
  menus, sound, disc and save data.
- `src/overlays/` — the mode modules (`grand_prix`, `time_attack`, `versus`,
  `garage`, `records`, `options`, `extra_trial`, `serial_link`,
  `movie_playback`), linked statically with explicit dispatch.
- `src/port/` — host entry point.
- `include/` — game types, data formats and subsystem interfaces.
- `third_party/psyz/` — PsyZ and its SDL3 dependency.
- `docs/` — architecture, code organization, conventions, port plan and
  format notes.

For development details, see [contributing](CONTRIBUTING.md),
[architecture](docs/ARCHITECTURE.md), [code organization](docs/CODE_ORGANIZATION.md)
and [conventions](docs/CONVENTIONS.md).

This repository contains source only. Disc images, `R4.BIN`, `R4.STR`, memory
cards and proprietary SDK files are not distributed.
