# Wave 3 brief

You are helping decompile Ridge Racer Type 4 (PAL, PlayStation) in /Users/hasik/Projects/ridge-racer-type-4-decomp. Goal: C source that compiles byte-identically to the original functions.
FIRST read docs/DECOMP_GUIDE.md completely: toolchain, per-function loop, checker tools/scripts/fd.py, tricks from earlier agents, the two-compiler rule (marker comment `/* GCC_VERSION: 2.7.2 */` for PsyQ library code, delay-slot filling emulation, tools/scripts/place.py), GTE inline asm, and the decomp-permuter wrapper tools/scripts/perm.sh for near-misses.

Rules: only create/edit your own src/main/PAL/<func>.c files; never run make, verify.py, fd_all.py (other agents work in parallel); always use .venv/bin/python; leave a file in src/main/PAL only if fd.py reports MATCH (exit code 0); delete or move failed attempts out of src/main/PAL (keep drafts in the scratchpad dir, not the repo); use unique helper names prefixed with your chunk id; run at most PERM_JOBS=4 permuter jobs at a time. Functions marked near-miss in your list have an earlier draft in src/failed/PAL/<func>.c - start from it and use perm.sh on those (score < ~400 is promising).

This is the fifth pass: remaining functions that have a stack-frame prologue (near-misses from earlier waves + large ones). Many have drafts in src/failed/PAL and scratchpad dirs (w3*/ w4c*/ c*/); look there first (ls the scratchpad). Also read the new tricks at the end of the guide. Be systematic: for large functions work block by block (compile early and look at where the diff starts), try both compilers where relevant, use permutation search for near-misses. Skip functions that are clearly hand-written asm (no prologue, custom calling convention, bare $t/$s registers, clobbering $at, BIOS stubs such as `li $t2,0xA0; jr $t2`). Give up on a function after ~10 genuinely different attempts.

Final reply: SHORT report - how many matched (names), what blocked the rest, new general tricks not already in the guide.

Extra rules: never use pkill/killall (kill only your own PIDs via pgrep -f perm/<func>); never run shell commands that read stdin (no 'cat > file' without heredoc); stop any background job you started before replying. List entries: name addr size flags. Prefer small/medium functions first.
