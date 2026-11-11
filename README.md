# Ridge Racer Type 4 PC

Native PC port of the PAL PlayStation release of *Ridge Racer Type 4*
(`SCES-017.06`), built from the matching decompilation in
[ridge-racer-type-4-decomp](https://github.com/khasinski/ridge-racer-type-4-decomp)
and [PsyZ](https://github.com/khasinski/parasite-eve-psyz) for the PlayStation
runtime APIs.

The code was imported from
[ridge-racer-type-4-decomp](https://github.com/khasinski/ridge-racer-type-4-decomp)
through revision `d38852ee`. From this point this repository develops
independently as a native C project. Byte matching, retail executable
checksums, original compiler output, fixed code addresses and PS1 binary builds
are no longer acceptance requirements. Changes are evaluated by behavior,
portability, readability and native builds.

The port is in its initial bootstrap phase. The executable opens a PsyZ window,
draws a frame counter and exits after eight frames. The original game sources
under `src/main/` and `src/overlays/` are retained for porting in place and
join the host CMake build as their remaining 32-bit pointer assumptions,
relocated asset tables and hardware interfaces are adapted to native C and PsyZ.

## Build and run

Use a C11 compiler, CMake 3.21 or newer and Git. PsyZ and SDL are source
submodules; no historical PS1 compiler, Sony SDK, Python packages or extracted
game executable is needed for this build.

```sh
git clone https://github.com/khasinski/ridge-racer-type-4-pc.git
cd ridge-racer-type-4-pc
git submodule update --init third_party/psyz third_party/psyz/external/SDL
cmake -S . -B build
cmake --build build --parallel
./build/ridge-racer-type-4-pc
```

`make`, `make run`, `make test` and `make ci` wrap the native CMake build.

## Layout

- `src/main/` — resident game logic (`camera`, `car`, `course`, `data`, `demo`,
  `effect`, `gfx`, `math`, `menu`, `race`, `sound`, `system`, `ui`) and
  `psyq/` helpers to port in place.
- `src/overlays/` — the nine mode and screen overlays (`grand_prix`,
  `time_attack`, `versus`, `garage`, `records`, `options`, `extra_trial`,
  `serial_link`, `movie_playback`) to link statically with explicit dispatch and
  reset boundaries.
- `src/port/` — native bootstrap entry point and host integration helpers.
- `include/` — game types, data formats and subsystem interfaces.
- `third_party/psyz/` — PsyZ and its SDL3 dependency.
- `docs/` — native architecture, code organization, conventions, port plan and
  format notes.

See [CONTRIBUTING.md](CONTRIBUTING.md), [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md),
[docs/CODE_ORGANIZATION.md](docs/CODE_ORGANIZATION.md) and
[docs/PC_PORT_PLAN.md](docs/PC_PORT_PLAN.md) for the development workflow.

This repository contains source only. Disc images (`CUE`/`BIN`/`CHD`),
`R4.BIN`, `R4.STR`, memory cards and proprietary SDK files are excluded.
Running disc-backed game features requires the user's own legal copy of the
game.
