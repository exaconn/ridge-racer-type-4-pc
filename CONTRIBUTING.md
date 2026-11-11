# Contributing

This is a native C11 project. Preserve the intended game behavior and data
formats while adapting the code for modern 64-bit hosts. There is no byte-match
requirement, stock PS1 compiler requirement, PS1 checksum gate, source manifest
requirement or decompilation progress/debt ratchet.

## Development workflow

1. Repair the original modules in place under `src/main/` and `src/overlays/`;
   use subsystem headers under `include/` for shared declarations and ordinary
   C types and functions.
2. Remove or simplify unnecessary PS1 hardware/BIOS plumbing; use native C or
   our PsyZ fork where needed, preserving gameplay behavior.
3. Build with `cmake -S . -B build -DBUILD_TESTING=ON` and
   `cmake --build build --parallel`.
4. Run `ctest --test-dir build --output-on-failure` to execute the native test
   suite.
5. Validate changed game behavior with focused native tests or headless
   reproducible scenarios (`PSYZ_HEADLESS=1 SDL_AUDIODRIVER=dummy`).

Do not preserve register assignments, instruction scheduling, padding, linker
order or code addresses for compiler matching. Assembly or GTE macros that
perform real work must be translated to portable C or routed through PsyZ; do
not replace functionality with no-op stubs.

Serialized assets (`R4.BIN`, `R4.STR`, relative `.rel` archives, TIMs, VABs)
and save data retain their defined widths, offsets and endianness. Host
pointers must use real pointer types (`void *`, typed struct pointers,
`OT_TYPE *`), and host structures can be separated from raw 32-bit serialized
offsets.

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md),
[docs/CODE_ORGANIZATION.md](docs/CODE_ORGANIZATION.md),
[docs/CONVENTIONS.md](docs/CONVENTIONS.md) and
[docs/PC_PORT_PLAN.md](docs/PC_PORT_PLAN.md).
