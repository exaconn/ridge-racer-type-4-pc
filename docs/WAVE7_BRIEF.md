# Wave 3 brief

You are helping decompile Ridge Racer Type 4 (PAL, PlayStation) in /Users/hasik/Projects/ridge-racer-type-4-decomp. Goal: C source that compiles byte-identically to the original functions.
FIRST read docs/DECOMP_GUIDE.md completely: toolchain, per-function loop, checker tools/scripts/fd.py, tricks from earlier agents, the two-compiler rule (marker comment `/* GCC_VERSION: 2.7.2 */` for PsyQ library code, delay-slot filling emulation, tools/scripts/place.py), GTE inline asm, and the decomp-permuter wrapper tools/scripts/perm.sh for near-misses.

Rules: only create/edit your own src/main/PAL/<func>.c files; never run make, verify.py, fd_all.py (other agents work in parallel); always use .venv/bin/python; leave a file in src/main/PAL only if fd.py reports MATCH (exit code 0); delete or move failed attempts out of src/main/PAL (keep drafts in the scratchpad dir, not the repo); use unique helper names prefixed with your chunk id; run at most PERM_JOBS=4 permuter jobs at a time. Functions marked near-miss in your list have an earlier draft in src/failed/PAL/<func>.c - start from it and use perm.sh on those (score < ~400 is promising).

This is the seventh pass (LARGE functions >=0x2c0 bytes; work block by block, compile early, use m2c + gotos in asm block order; give each function a real effort), STOCK TOOLS ONLY: stock maspsx and stock gcc 2.7.2/2.8.1 (markers GCC_VERSION 2.7.2 and NOSPLIT only; NO FILL, no assembler emulation, no post-processing of cc1 output - those were removed; many guide notes about delay-slot fill are obsolete). Remaining = near-misses and previously fill-dependent functions (drafts in src/failed/PAL and scratchpad dirs; ls the scratchpad). Matches must come from C source alone. Also read the new tricks at the end of the guide. Be systematic: for large functions work block by block (compile early and look at where the diff starts), try both compilers where relevant, use permutation search for near-misses. Skip functions that are clearly hand-written asm (no prologue, custom calling convention, bare $t/$s registers, clobbering $at, BIOS stubs such as `li $t2,0xA0; jr $t2`). Give up on a function after ~10 genuinely different attempts.

Final reply: SHORT report - how many matched (names), what blocked the rest, new general tricks not already in the guide.

Extra rules: never use pkill/killall (kill only your own PIDs via pgrep -f perm/<func>); never run shell commands that read stdin (no 'cat > file' without heredoc); stop any background job you started before replying. List entries: name addr size flags. Prefer small/medium functions first.


> **BARRIER RULE:** every empty-asm barrier / `register asm` pin must have a `/* ASM_BARRIER: <reason> */` comment (same line or <=3 lines above). See docs/BARRIERS.md; audit with tools/scripts/list_barriers.py.
