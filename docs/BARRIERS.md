# Empty-asm barriers

Some matches rely on an empty inline-asm statement (`__asm__("" : "=r"(x) : "0"(y))`, `register T x asm("$N")` pins, etc.) to steer
register allocation / CSE of stock gcc. These existing constraints are reconstruction
debt. A matching function that contains them is not yet clean reconstructed C.
Do not introduce new pins, empty barriers, fake `volatile`, unused stack padding
or dummy operations to repair a match. Reconstruct the actual types, interfaces,
object ownership and C operations instead; preserve the match while doing so.

Rule: every such statement carries a comment `/* ASM_BARRIER: <what it forces / why> */` on the same line or up to 3 lines above.

    /* ASM_BARRIER: forces copy a1->v1 (original has addu v1,a1,zero) */
    __asm__("" : "=r"(c) : "0"(a));

Audit: `.venv/bin/python tools/scripts/list_barriers.py` (all) or `--undocumented`.
To retire one, remove the constraint and verify the entire TU with
`tools/scripts/verify_c_unit.py`, then run `make check`. The relocated MIPS
instruction Levenshtein distance must remain 0 and the full EXE must remain
identical. `fd.py` is an iteration aid that masks relocations, not final proof.
Exercise changed types and interfaces through their real callers and relevant
native tests as well; matching target instructions alone does not prove native
pointer correctness or a consistent C interface.

Actual hardware operations and pipeline gaps live in `include/rr_asm.h`, not raw instruction strings in function bodies.
Require evidence for the hardware dependency; a named macro does not justify
inserting an otherwise artificial instruction to restore the match.
Use `RR_NOP_DEP_ON(value)` when the load producing `value` must precede the gap (for example before a polling-loop label or an indirect-call dispatch label). It emits the same single `nop` as `RR_NOP_DEP()` while retaining an explicit register dependency. Document each use with `ASM_BARRIER` and verify the relocated instruction Levenshtein distance and full EXE.
