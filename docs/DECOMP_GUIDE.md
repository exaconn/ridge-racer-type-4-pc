# Reconstructing C (PAL, Ridge Racer Type 4)

The objective is readable, correctly typed C that retains the original program.
The matching metric is Levenshtein distance between relocated MIPS instruction
sequences. Distance 0 must also be confirmed by complete EXE identity.
A matching function can still contain reconstruction debt.

## Source and types

1. Locate the current definition, its defining unit header, every caller and the
   shared data owners. `configs/PAL/functions.txt` records function spans;
   `configs/PAL/sym.main.txt` records the current names. Use
   `c_functions.source_owners`, `definition_source` and `address_of` in tooling.
2. An `m2c` draft supplies hypotheses. Recover actual structures, array strides,
   pointer fields, argument counts, return values, signedness and ownership from
   the instruction/data accesses and callers. Do not treat register temporaries,
   labels or a guessed `int f()` declaration as original source evidence.
3. Include the defining unit's public header for calls and shared globals.
   Reconcile declarations and definitions together. Types that happen to generate
   the same PAL instructions can still be incompatible C interfaces.
4. Keep genuine C pointers at full native width. Integer words are appropriate
   for encoded resource offsets, instructions and GPU DMA tags; do not use them
   as stand-ins for pointers that C later dereferences. Check native alignment
   and pointer-sensitive layouts separately from the PAL layout assertions.
5. Use complete objects and real array extents. A large raw-offset accessor macro
   hides a missing layout. A local buffer sized only to match a stack frame does
   not recover the original object. Model distinct lifetimes/views of reused
   scratch memory explicitly, with unions where the storage is actually shared.
6. Express the real fixed-width arithmetic, including wrapping where the MIPS
   operation wraps. Avoid signed overflow, negative left shifts and `abs(INT_MIN)`
   dependencies. Name fields by established meaning; retain an honest reserved
   span when its interpretation is unknown.

See [SOURCE_LAYOUT.md](SOURCE_LAYOUT.md) for TU/header organization, canonical
owners, native fixtures and known remaining limitations.

## Matching and verification

1. Keep failed probes under ignored `build/`. Do not commit WIP or failed matches.
   Preserve concurrent edits; reread current sources before applying a probe.
2. Use `.venv/bin/python tools/scripts/fd.py <source>` for iteration. It masks
   relocations, so its `MATCH` result is not final verification.
3. Verify the entire owning TU after every implementation/type change:

   ```sh
   .venv/bin/python tools/scripts/verify_c_unit.py src/main/PAL/game/path/unit.c --output build/unit_check
   ```

   Require distance 0 for every function, correct function addresses, complete
   text extent and identical rodata. The existing `build/PAL/main.elf` supplies
   external addresses; a failed full link can remove it.
4. Run `make -j8 check` to confirm complete resident EXE identity. Coordinate
   shared build/config mutations when other work is active. When overlay sources
   or shared interfaces affect overlays, also run
   `.venv/bin/python tools/scripts/verify_overlay_samples.py`; it verifies the
   implemented overlay functions, duplicate sites and reconstructed R4.BIN.
5. Exercise the changed interfaces/owners through the actual current C bodies and
   relevant callers/native tests. Use real public headers, full native pointers,
   meaningful boundary/alias cases and independent behavior or original-MIPS
   evidence. Hardware/service adapters must state what they replace. A frozen
   fixture or a mock emitter does not prove correctness of the current emitter.
6. Use `.venv/bin/python tools/scripts/run_tests.py <test paths> --jobs 4 --output build/tests/check`
   or `make test`. The runner dispatches unittest and standalone script modules
   correctly and rejects zero-test unittest runs. Retain original test cases and
   oracles; repair stale fixtures to use actual current APIs rather than weakening
   their assertions or replacing the implementation with an old body.
7. Before reporting a result, inspect current logs and sources. Scope the claim
   to what was checked. A targeted test is not a full-suite run, and separate
   historical passes do not establish one stable current-source suite result.

## Compiler and assembler

Use stock toolchains through `tools/scripts/cc.sh`:

- The default is GCC 2.8.1 with `-O2 -G0 -mips1 -mcpu=3000 -funsigned-char`.
- `/* GCC_VERSION: 2.7.2 */` selects the stock library compiler.
- `/* GCC_VERSION: 970318 */` selects the 1997-03-18 snapshot built by
  `tools/gcc-970318/build.sh`; the wrapper supplies its required flags.
- `/* CC1_FLAGS: <stock options> */` records a verified per-unit configuration.
  `/* NOSPLIT */` is only the deprecated alias for `-mno-split-addresses`.

The macOS host builds need the logical real-word transfer repair in
`tools/scripts/repair_gcc_real_host.py`. RTL fields contain 64-bit host pointers,
while this port retains 32-bit `HOST_WIDE_INT` words. Contiguous copies across
those fields read padding and can turn executable floating constants into zero,
even when static initializer data is correct. The repair transfers each logical
word through its own field. GCC 2.7.2, 2.8.1 and 970318 were compared against
independent ILP32 Linux builds: 456 binary32/binary64 literal functions per
compiler have relocated instruction Levenshtein 0, identical read-only data and
the expected executed values. Target backends and compiler flags stay unchanged.

Rebuild the release C compilers on macOS with
`bash tools/gcc-psx/build.sh 2.7.2` and `bash tools/gcc-psx/build.sh 2.8.1`;
the snapshot uses `bash tools/gcc-970318/build.sh`. Each recipe checks executable
and static literal values before installing its compiler. Source archive hashes
and PSX patch revisions are pinned in the recipes. `Makefile` includes compiler
binaries among the C-object dependencies so rebuilding a compiler rebuilds code.

`tools/scripts/repair_maspsx_float_pairs.py tools/maspsx` completes the omitted
temporary/saved GPR pairs for the existing `li.d` expansion, including numeric
`$16/$17` for the already supported `$s0/$s1` pair. The regression test executes
768 numeric/named pair cases through both assembler profiles, checking values
and register guards while allowing the assembler's reserved `$at` temporary.
`test_target_float_literals.py` executes the full compiler literal corpus.

One TU has one compiler configuration and address-contiguous registered functions
and rodata. TU boundaries are reconstructed, not recovered original file names.
Keep stock target code generation and assembler scheduling rules. The host-word
transfer and register-name corrections above restore those rules for this host.
Do not post-process compiler output,
restore `MASPSX_FILL`/`aspsx_fill.py`, patch instructions or classify difficult C
as handwritten assembly to bypass reconstruction.

## Hardware and existing constraints

Actual GTE/MMIO operations belong in named hardware adapters such as
`include/rr_asm.h`. Describe their true inputs, outputs, clobbers and dependencies;
let the compiler allocate ordinary C locals. Use `volatile` for established
hardware or interrupt visibility, not for scheduling ordinary RAM operations.

Do not add register pins, empty barriers, fake volatile, dummy operations or
unused stack padding to repair a match. Existing constraints are debt. Retained
constraints need their `ASM_BARRIER` explanation, and removing them must preserve
the relocated TU match and full EXE. See [BARRIERS.md](BARRIERS.md).

Earlier chunk/wave scheduling experiments are preserved in
[history/MATCHING_NOTES.md](history/MATCHING_NOTES.md). They include obsolete and
incorrect approaches and are not the current workflow.
