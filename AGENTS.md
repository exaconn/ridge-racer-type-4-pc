# Ridge Racer Type 4 PC project rules

## Porting contract

- This is a one-way native C11 port built on the matching decompilation and
  `third_party/psyz`. Matching PS1 assembly or retail binary checksums is not a
  goal.
- Repair the original modules in place under `src/main/` and `src/overlays/`.
  Do not build a parallel replacement engine or reimplement game subsystems in
  separate files.
- Keep `src/overlays/` modules (`grand_prix`, `time_attack`, `versus`,
  `garage`, `records`, `options`, `extra_trial`, `serial_link`,
  `movie_playback`) statically linked with explicit dispatch and state reset
  boundaries instead of fixed RAM overlay windows.
- Separate 32-bit disc offsets in `R4.BIN` / relative archives from 64-bit host
  pointers and use `OT_TYPE` / PsyZ primitives for GPU ordering tables and
  packets.

## Automated game runs

- When launching the game for automated tests or verification, use no audible
  audio and no visible window.
- Set `PSYZ_HEADLESS=1` and `SDL_AUDIODRIVER=dummy`; retain the real GPU
  backend when rendering or capture coverage needs it. Do not use visible or
  audible test runs unless the user explicitly requests them.
