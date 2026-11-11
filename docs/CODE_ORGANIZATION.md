# Code organization

Repair complete original modules in their existing files under `src/main/` and
`src/overlays/`; `include/` contains shared subsystem headers. Do not create a
parallel replacement engine or extract selected functions into duplicate files
while bringing the original game up on the host. Add the existing whole modules
to `CMakeLists.txt` as they become portable.

`src/overlays/` holds the nine mode and screen modules (`grand_prix`,
`time_attack`, `versus`, `garage`, `records`, `options`, `extra_trial`,
`serial_link`, `movie_playback`). Native modules use static linking, explicit
registration, namespaced symbols (`ovl659_*` .. `ovl668_*`) and clear
initialization/reset boundaries. They do not need fixed RAM windows, PS1 linker
scripts or binary repacking.

`src/main/psyq/` contains only game-coupled PsyQ helpers that PsyZ lacks.
Standard PS1 SDK APIs (`libgpu`, `libgte`, `libspu`, `libetc`, `libpad`,
`libcd`, `libpress`) are provided by `third_party/psyz`; keep only portable C
helpers in `src/main/psyq/` where game-coupled behavior requires them.

Shared workspaces (`SharedWorkArena`, `RaceContext`, `SoundState`) are
accessed through multiple typed layout views. GCC/Clang builds of the original modules must preserve that aliasing and signed integer
wrapping (`-fno-strict-aliasing -fwrapv`, configured in the root
`CMakeLists.txt`).

Scratchpad work (`0x1F800000`) and arena allocators use ordinary host buffers.
Preserve data lifetimes and reset behavior, not PS1 physical addresses.
