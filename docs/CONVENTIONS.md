# C conventions

Use C11 and the native host compiler. Prefer clear functions, typed pointers,
subsystem headers and names that explain the role of a value.

Use fixed-width integer types (`u8`, `s8`, `u16`, `s16`, `u32`, `s32`) for
serialized disc and save data. Separate serialized 32-bit offset tables in
`R4.BIN` / `.rel` containers from host runtime pointers; decode offsets
explicitly instead of storing 64-bit host pointers in `s32` / `u32` fields.
Do not assume `long` or a pointer is 32 bits. Use `size_t` for host sizes,
`ptrdiff_t` for pointer differences and `uintptr_t` only when an integer
representation of a pointer is necessary.

Original code addresses, register bindings (`asm("$...")`), compiler scheduling
barriers (`__asm__("")`), per-file historical compiler flags (`GCC_VERSION`,
`MASPSX_FLAGS`, `CC1_FLAGS`) and padding added to match instructions are not
part of the native design.

Hardware access belongs in PsyZ or a small host adapter. Existing GTE routines
(`src/main/gfx/gte*.c`) perform real geometry and lighting work and must run
through PsyZ's GTE implementation or portable C equivalents.

Validate behavior with native tests and headless gameplay scenarios
(`PSYZ_HEADLESS=1 SDL_AUDIODRIVER=dummy`).
