# PC Port Plan

One-way native port of *Ridge Racer Type 4* (`SCES-017.06`): the decompiled
game code in `src/main/` and `src/overlays/`, compiled with a host C11 compiler
and linked against `third_party/psyz`.

## Key technical areas

| Area | Notes |
| --- | --- |
| **64-bit pointer & OT adaptation** | Convert `RelocateRelativeFile` / `RelocateOffsetTable` / `RelocateCountedOffsetTable` / `UnrelocateModelBank` and GPU ordering-table / primitive chains (`GpuDrawWorkspace`, `GS->ot`, `GS->cur`) to 64-bit-safe pointers and PsyZ `OT_TYPE` packets. |
| **Handwritten GTE & renderer routines** | Port the GTE polygon/model routines in `src/main/gfx/` (`gtemodel.c`, `gtepoly1.c`, `gtepoly2.c`, `gtepoly3.c`, `gtequad.c`, `gtetri.c`, `gteroad.c`, `gtetrail.c`) to PsyZ GTE macros (`libgte.h`) or portable C. |
| **Disc & `R4.BIN` / `R4.STR` I/O** | Mount user-supplied PAL `CUE`/`BIN` (or `CHD`) disc images via PsyZ `libcd`, serving `R4.BIN` sector reads, `R4.STR` MDEC movie streams and CD-DA / XA audio tracks. |
| **Static overlay dispatch** | Replace the `R4.BIN` overlay code loader (`659..668` at `0x80114998`) with a static overlay dispatch table calling `grand_prix`, `time_attack`, `versus`, `garage`, `records`, `options`, `extra_trial`, `serial_link` and `movie_playback` directly. |
| **Audio & SPU (`libsnd` / VAB)** | Connect `src/main/sound/` and `src/main/psyq/libsnd/` to PsyZ `libspu` so VAB banks, engine pitch modulation, sequence playback and reverb synthesize natively. |
| **Saves & controller input** | Route memory card slots (`src/main/data/cardini.c`, `src/main/menu/savemenu.c`) and DualShock/JogCon/NegCon input (`src/main/system/pad*.c`) through PsyZ pad and memory-card files. |

## Milestones

1. **WP00 — Foundation & Host Compilation:** CMake + PsyZ build, global state
   definitions (`globals.c`), scratchpad host buffer (`SCRATCHPAD`), and clean
   64-bit compilation of `src/main/` and `src/overlays/`.
2. **WP01 — Boot, `R4.BIN` Mount & Intro/Title:** Boot `src/main/system/main.c`
   against a PAL disc image, stream opening `movie_playback` (`R4.STR`) and
   reach the Title and Main Menu screens.
3. **WP02 — Menus, Garage & Mode Overlays:** Static overlay dispatch for
   `options`, `records`, `garage` (including emblem editor), `grand_prix`,
   `time_attack`, `versus` and `extra_trial` course/car selection menus.
4. **WP03 — Course & Vehicle Renderer + Race Loop:** Full GTE course/sky/car
   rendering, vehicle physics, AI opponents, HUD, lap timing, replay and race
   results.
5. **WP04 — Audio, Saves & Desktop Polish:** SPU engine/SFX/BGM synthesis,
   memory-card persistence, widescreen / high-resolution rendering via PsyZ,
   and cross-platform release packaging.
