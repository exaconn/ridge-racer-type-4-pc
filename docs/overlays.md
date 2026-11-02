# PAL overlays: static analysis

Analysis date: 2026-10-28. Entry indices below are **zero-based**.

## Container and address map

`disc/PAL/R4.BIN` has 950 entries: a little-endian u32 count followed by
951 u32 sector offsets (2048-byte sectors). First payload sector: 2; terminal
sector: 28776, exactly the file end (58,933,248 bytes). Entries are heterogeneous:
**not every entry is a nested container**. Executable overlays are flat images
with a module ID, strings/jump tables, code, data, and sector padding.

The overlay destination is **0x80114878**, not the formerly suspected 0x80110000.
Evidence:

- Main EXE word at VA `0x800102F0` (file offset `0xAF0`) is `0x80114878`.
- Startup at `0x8007D718` clears BSS through `0x80114878` (exclusive).
  The `0x80110000` region contains resident library state, not overlay code.
- `func_8003A874` resets `D_800ACC18` to `D_800102F0`, then requests
  `func_8001292C(func_8001223C(1, mode + 0xB), D_800ACC18)`.
  The category-1 mapper adds `0x292`, so this selects entries 669..673 for
  mode 0..4. These include copies of the executable menu modules.
- `func_8003A9E8` likewise requests entries `659 + D_800F4D2C` into that buffer.
- `func_8003BBBC` and `func_8003BD2C` explicitly load entry 668
  (`0x292 + 10`) into the reset buffer.
- Absolute jump tables and intra-overlay calls agree with this base.
  For example, entry 660 has a function prologue at file offset `0xA8`,
  hence VA `0x80114920`, called by resident `func_8007C308`.

The loader reuses the ordinary asynchronous CD state machine:
`func_80012290` converts the R4 directory into `D_801094A8` entries;
`func_8001292C` selects destination/location/sector count;
`func_800127A0` starts the read through `func_8008EEF4`;
`func_80012A58` reports completed bytes. The callers advance their allocation
cursor by the full sector-rounded size. Static code contains absolute resident
and overlay addresses; there is no evidence here of a relocation pass.
Runtime cache behavior has not been verified in an emulator.

## Executable inventory

The spans below run from the first identified instruction to the final observed
return delay slot, with an exclusive end. They are **candidate text spans**,
not a finalized linker section map or a verified function partition.
Role names are inferred from embedded strings and callers.

| Entry | Module ID | Sector | Stored bytes | Candidate text offsets | Text span bytes | Interpretation |
|---|---|---|---:|---|---:|---|
| 659 | 10 | 19512 | 8192 | 0x54..0x1CB8 | 7268 | Menu module; exact role pending |
| 660 | 11 | 19516 | 14336 | 0x98..0x2FA8 | 12048 | Time attack course selection (`job_ta_crs_info0`) |
| 661 | 8 | 19523 | 16384 | 0xAC..0x3AE0 | 14900 | Versus selection (`PLAYER 2`, `job_vs_car_info0`) |
| 662 | 12 | 19531 | 77824 | 0x120..0x1199C | 71804 | Garage (`garage0`, `job_garage_car_info0`) |
| 663 | 13 | 19569 | 10240 | 0x60..0x1FF0 | 8080 | Records (`recode0`) |
| 665 | 14 | 19582 | 20480 | 0xA0..0x47A0 | 18176 | Menu module; exact role pending |
| 666 | 15 | 19592 | 10240 | 0x5C..0x2668 | 9740 | Extra course selection (`job_ex_crs_info0`) |
| 667 | 9 | 19597 | 18432 | 0xB4..0x4588 | 17620 | Multiplayer / serial (`sio:`, `PLAYER 2/3/4`) |
| 668 | 6 | 19606 | 79872 | 0x98..0x12FAC | 77588 | STR playback (`strNext`, timeout diagnostics) |

Exact whole-entry duplicate pairs: **669 = 660**, **670 = 659**,
**671 = 661**, **672 = 667**. Thus 13 executable entries represent 9 unique
images. Their unique candidate text spans total **237,224 bytes**.
Duplicates should share source/build products and be checked at both disc sites.

Entries 658, 664 and 673 are not executable overlays on the evidence examined:
658/673 have asset container headers; 664 begins with the `SC` resource signature
and Shift-JIS text. Their sizes must not be added to a code denominator.

A full aligned-word scan of all 950 entries found return-instruction patterns
only in the 13 listed overlays and entry **879**. Entry 879 is a structured
resource container: its apparent instructions occur in repeated 44-byte records
with constants 1000/400 and zeroed operands, rather than contiguous executable
MIPS. Treat this as **unclassified MIPS-pattern data**, not a tenth confirmed
overlay. Its resource semantics still need investigation. This scan does not
prove absence of compressed code or code without standard returns elsewhere.

## Effect on coverage

The resident EXE has 572,684 text bytes; C definitions cover 233,480 bytes.
Adding the nine unique candidate overlay text spans yields an estimated code
pool of **809,908 bytes**, hence approximately **28.83%** recovered in C.
This is a provisional whole-program estimate: candidate section boundaries,
entry 879 and possible compressed payloads remain to be resolved. The previous
**40.77%** remains the exact stated coverage of the main EXE text only.
No overlay functions have been added to the 1200 completed C functions.
Do not interpret `jr ra` counts as function counts: functions can have multiple
returns, tail calls, or no standard return.

## Reproduction and next work

Run:

```sh
.venv/bin/python tools/scripts/analyze_overlays.py
```

This validates the directory, checks duplicate identities, emits a full
950-entry inventory to `build/overlays/inventory.json`, extracts the 13 images,
and writes nine candidate-text disassemblies. It does not modify main EXE
configuration, symbols, source ownership, or completion statistics.

Use overlay identity **and** address in future symbols: the same RAM address
has different code across images. A useful layout is `src/overlays/PAL/<id>/`
and a separate split/link map per unique image. Establish code/data/BSS sections
and function partitions before enabling exact coverage accounting. Start with
659 (smallest candidate text span), then 660 (known resident call target and
recognizable course-selection structures). Garage 662 is a useful larger TU
for shared menu structures. STR module 668 likely contains substantial library
code and should be tracked separately when distinguishing game/library code.

## Compiler (verified on three small functions: func_800172DC, func_8001F254, func_80025F84)

gcc **2.8.1** PSX cc1 reproduces the original codegen (hoisted `lui/addiu` base pointers, loop delay slots);
2.7.2 folds the symbols into the load/store and does not match. Pipeline in `tools/scripts/cc.sh`:
cpp -> gcc 2.8.1 cc1 `-O2 -G0 -mips1 -mcpu=3000 -funsigned-char` -> maspsx -> GNU as.
Binaries are copied (not committed) into `tools/toolchain/` from ~/Projects/parasite-pc and ~/Projects/rage-pc.
ASPSX version (`--aspsx-version=2.79`) and gas backend are unverified guesses.


## First C samples (2026-10-28)

One complete function from every unique image is now in
`src/overlays/PAL/<entry>/`. Symbols include the image number to avoid collisions
between mutually exclusive RAM images. These are separate reconnaissance units;
they are not included in the resident EXE's 1200-function ownership count.

| Entry | Function VA | Behavior |
|---|---|---|
| 659 | 0x801148CC | Calls resident cleanup and sets the next menu state |
| 660 | 0x80114910 | Writes transition code 0x131 |
| 661 | 0x80117340 | Writes transition code 0x137 |
| 662 | 0x8011E7C0 | Stores four low-halfword arguments in overlay-local state |
| 663 | 0x80115BA0 | Initializes records display state, scratchpad and resident renderer |
| 665 | 0x80115C8C | Initializes a menu panel and publishes its state |
| 666 | 0x801166D0 | Cleans up course selection and updates the display |
| 667 | 0x80115048 | Reads an overlay-local state word |
| 668 | 0x8011688C | Reads a word through an overlay-local pointer |

All nine compile with the existing default **GCC 2.8.1 / MASPSX** pipeline,
without compiler overrides, register bindings or assembly barriers. All have
**instruction Levenshtein distance 0 after full relocation resolution**.
This supports the load address and toolchain choice for these functions; it
does not establish compiler identity for every function or library in each image.

Reproduce:

```sh
.venv/bin/python tools/scripts/verify_overlay_samples.py
.venv/bin/python -m unittest tools/scripts/test_overlay_samples.py -v
```

The verifier reads original disc entries directly, compiles each source, links
it at its original VA with every external address resolved, and compares the
resulting instruction sequence. `SUBALIGN(4)` preserves the original four-byte
placement even where it differs from the object's default section alignment.
It also replaces the nine function spans and their four duplicate sites in an
in-memory copy of R4.BIN, confirms whole-image identity, and independently checks
the currently built main EXE against its original. This is a **partial rebuild
using original bytes for untouched regions**, not a complete overlay link.
The machine-readable result is `build/overlays/samples/verified.json`.

The native test compiles the actual nine source bodies with UBSan. Its only
address adaptation replaces the records routine's literal scratchpad base with
a host array. Resident calls use assertions for arguments, call order and live
state; callbacks change pointers before subsequent stores to check fresh loads.
It covers every low-halfword value in the garage setter, surrounding storage,
word getters and changing full-width host pointers. These are unit-level checks,
not an emulator playthrough. Existing resident source files and shared headers
were left untouched to allow concurrent debt cleanup.

## Whole-image build for 659/670 — work in progress

`configs/PAL/overlays/659.json` records all seven complete function ranges.
`src/overlays/PAL/659/data.c` reconstructs the module ID, four threshold bytes,
34 command words and local countdown. The two jump tables come from their
reference function objects; the remaining sector padding is explicitly zeroed.

```sh
.venv/bin/python tools/scripts/build_overlay659.py
.venv/bin/python tools/scripts/build_overlay659.py --require-complete
.venv/bin/python -m unittest discover -s tools/scripts -p 'test_overlay*.py' -v
```

The normal command currently reproduces both complete 8192-byte images exactly,
with a separate relocated Levenshtein check on every function. The stricter
command deliberately fails until all seven functions use C. **Current C recovery
is 2/7 functions**: 801148CC and 801158B4; five reference assembly files remain in
`asm/overlays/PAL/659/`. Image identity must not be described as complete C recovery.
The build leaves the main EXE build configuration untouched.

The newly recovered standings routine 801158B4 copies a nonnegative prefix of
at most six results, stably sorts a local copy, and publishes the first minimum's
index. With status -1 it instead publishes a finish marker and invalidates a
selected result. Native UBSan checks cover all 3^6 small-value combinations,
every sentinel position, random signed-word boundaries and finish counts, ties,
and preservation of the complete resident vehicle storage. The valid positive
finish-count range is 1..6; nonpositive counts skip the indexed write.

Unfinished C research is retained under `research/overlays/PAL/659/`, explicitly
outside the build and outside completion accounting.
