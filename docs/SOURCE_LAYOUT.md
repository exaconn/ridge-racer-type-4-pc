# Source layout

`src/main/PAL/` is organised as the original program was: game code and the Sony PsyQ library
are separate trees, and each `.c` file is a translation unit (TU) holding several functions.

```
src/main/PAL/
  game/        Namco game code            (addresses below 0x8007D000)
    system/    boot, main loop, interrupt/timer glue, pad
    gfx/       ordering tables, primitives, sprites, fonts, renderers
    ui/        HUD, text, windows        menu/   front-end screens
    sound/     game-side audio manager   data/   CD/file loading, compression, save
    course/    track data and geometry   car/    vehicle state, physics, AI
    race/      race flow, rules, replay  camera/ effect/ math/ demo/
  psyq/        Sony PsyQ SDK (compiled with the library compiler, see DECOMP_GUIDE "Third compiler")
    libapi/ libc/ libcard/ libcd/ libgpu/ libgte/ libpad/ libsio/ libsnd/ libspu/ other/
  func_XXXXXXXX.c   (top level only) a freshly matched function that has not been placed yet
```

Overlay code stays in `src/overlays/PAL/<n>/` and is described in `docs/overlays.md`.

## File naming
Namco/PsyQ-era style: lower case, at most 8 characters, a noun or noun+verb abbreviation
(`carmove1.c`, `cdread.c`, `sprite.c`, `vmchval.c`). Where a PsyQ library file name is known
(`utgpa`, `ccadsr`, `midi*`) it is used. Many names are descriptive guesses; they carry no
information about the real original file names.

## How the TUs were formed (reconstructed, not recovered)
The original TU boundaries are not stored in the executable. The current grouping is a
reconstruction that obeys everything that is *hard evidence*:
* one compiler configuration per TU (`GCC_VERSION`, `CC1_FLAGS`, `MASPSX_FLAGS` markers);
* functions of a TU form one address-contiguous run (checked by `gen_config.py`);
* the `.rodata` blocks of a TU are one contiguous run;
* hand-written assembly (`configs/PAL/hasm.txt`) is never inside a C TU;
* every TU compiles as a whole and every function stays at relocated Levenshtein distance 0.

Between those constraints functions were grouped thematically (shared callees and data,
TextTiling over the call/data reference profile), then each group was cut at the first point
where the merged text no longer compiled or matched. Treat the boundaries as editable.

## Tools
* `tools/scripts/gen_config.py` + `make split` – segment names are the paths below `src/main/PAL`
  (`game/car/carmove1`), so moving or renaming a file is just a `git mv` followed by `make split`.
* `tools/scripts/tumerge.py` – parser/merger used to build the TUs (dedupes includes, typedefs and
  externs, renames colliding local types, emits prototypes for functions defined in the TU).
* `make views` writes `build/legacy/<func>.c`, the single-function view of every function that
  older unit tests expect; `make test` regenerates it and runs all `tools/scripts/test_*.py`.
* New matches: write `src/main/PAL/func_XXXXXXXX.c`, check with `fd.py`, then move it into the
  TU it belongs to (same compiler markers, address-contiguous with its neighbours) or leave it at
  top level until a neighbour is matched. `make check` is the final judge.
