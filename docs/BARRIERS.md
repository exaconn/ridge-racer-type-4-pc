# Empty-asm barriers

Some matches rely on an empty inline-asm statement (`__asm__("" : "=r"(x) : "0"(y))`, `register T x asm("$N")` pins, etc.) to steer
register allocation / CSE of stock gcc. They are accepted but must stay removable later (e.g. when the real source idiom is found).

Rule: every such statement carries a comment `/* ASM_BARRIER: <what it forces / why> */` on the same line or up to 3 lines above.

    /* ASM_BARRIER: forces copy a1->v1 (original has addu v1,a1,zero) */
    __asm__("" : "=r"(c) : "0"(a));

Audit: `.venv/bin/python tools/scripts/list_barriers.py` (all) or `--undocumented`.
To retire one: delete the statement, run `fd.py` on the file; if it still MATCHes it was unnecessary.

Named hardware pipeline gaps live in `include/rr_asm.h`, not raw instruction strings in function bodies.
Use `RR_NOP_DEP_ON(value)` when the load producing `value` must precede the gap (for example before a polling-loop label or an indirect-call dispatch label). It emits the same single `nop` as `RR_NOP_DEP()` while retaining an explicit register dependency. Document each use with `ASM_BARRIER` and verify the relocated instruction Levenshtein distance and full EXE.
