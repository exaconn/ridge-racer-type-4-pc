# Architecture

The native target compiles and runs the original C modules from
`ridge-racer-type-4-decomp`, repaired in place under `src/main/` and
`src/overlays/`, using CMake and PsyZ (`third_party/psyz`). No parallel game
engine, historical PS1 compiler, splat step, PS1 linker script or retail
checksum check is used.

## Native build and module integration

```
src/main/ + src/overlays/ -> host C11 compiler -> original game + PsyZ
```

1. **Resident executable (`src/main/`):**
   - `system/` — main loop, mode table dispatch (`ModeFuncTable`), frame task
     scheduler, pad polling and dual-actuator rumble.
   - `data/` — `R4.BIN` archive directory, asynchronous CD/sector streaming,
     relative file (`RelocateRelativeFile`) unpacking, memory card state
     machine and save data serialization.
   - `gfx/` — double-buffered `DRAWENV`/`DISPENV`, ordering tables, GTE course
     and car model projection (`model.c`, `gtemodel.c`, `gtepoly*.c`), skybox,
     billboards, rear-view mirror and screen fades.
   - `course/` — track node splines, surface projection, collision bounds,
     visibility octants and animated track objects.
   - `car/` — player and AI vehicle physics, tire grip, slipstream, collisions,
     replay recording and articulated wheel/body transforms.
   - `camera/` — race chase, cockpit, intro, finish and replay camera splines.
   - `race/` — race state machine, standings, lap/split timing and HUD.
   - `menu/` & `ui/` — mode select, Grand Prix story/dialogue panels, save UI,
     badge rows, glyph lists and UI windows.
   - `sound/` — VAB bank loader, engine voice pitch/pan synthesis, sequence
     queue and CD-XA / stream audio control.
   - `demo/` & `effect/` — opening/attract sequences, lens flare and spark
     particles.
2. **Statically linked overlays (`src/overlays/`):**
   - In retail PS1, `R4.BIN` entries `659..668` (copies `669..672`) are read
     on demand into `CdDirectoryBuffer`, linked for `0x80114878`. In the
     native port the nine modules (`grand_prix`, `time_attack`, `versus`,
     `garage`, `records`, `options`, `extra_trial`, `serial_link`,
     `movie_playback`) are linked statically; no code is read from disc.
   - **Dispatch:** `ModeFuncTable` / `ModeGroup0..4FuncTable`
     (`src/main/system/modeinit.c`) hold real function pointers; each entry
     the PS1 resolved into the window names the `ovlNNN_` function that
     image held at that address. Resident code calls other overlay entry
     points directly by name.
   - **Binding and reset:** the five loader steps that read an overlay image
     (`ldscn1.c`, `ldscn4.c`, `trkld8.c`) first call
     `OverlayModuleBind(entry)` (`modeinit.c`, `rr/overlay_module.h`). For an
     executable entry it runs `ovlNNN_reset_module()` and the step completes
     without reading or allocating; other entries (`664`, `673`, assets) are
     read as before. Each unit that defines module state lists it in an
     `ovlNNN_<unit>_state` table; the reset captures those objects on the
     first bind (their linked values = a fresh load: initialized data, zero
     BSS) and copies them back on every later bind.
   - Overlay data is defined in C with its original values.
3. **PsyZ runtime (`third_party/psyz/`):**
   - Provides `libgpu`, `libgte`, `libspu`, `libetc`, `libpad`, `libcd`,
     `libpress` (MDEC) and SDL3 window/audio/input integration.

## Repository layout

| Path | Contents |
| --- | --- |
| `src/main/<subsystem>/` | resident game modules |
| `src/main/psyq/lib*/` | legacy PsyQ helpers being replaced by PsyZ or kept as portable C |
| `src/overlays/<module>/` | the nine mode/screen overlay modules |
| `src/port/` | native entry point and host integration helpers |
| `include/` | shared types, asset layouts and subsystem headers |
| `third_party/psyz/` | PsyZ host runtime and SDL3 dependency |
| `docs/` | native development guidance, ABI notes and format reference |
