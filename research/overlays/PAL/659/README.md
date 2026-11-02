# Overlay 659 — unfinished C drafts

These files are research checkpoints, **not completed decompilation** and are not
part of the build. The active build uses reference assembler for these functions.
Do not move them into `src/overlays/PAL/659` without correcting semantics,
resolving instruction Levenshtein distance to zero and adding native tests.

Current relocated instruction Levenshtein distances:

- 80114908: 61 (explicit resident vehicle/page views).
- 801150CC: 20 (command table initialization; remaining argument scheduling and induction choices).
- 80115224: 211; compiler switch table is not identical.
- 801159E8: 7 (UI initialization; argument-store and scratchpad scheduling).
- 80115CE4: 406; compiler switch table is not identical.

The context is a working m2c aid, not an authoritative shared header. Some helper
prototypes still need review, especially K&R definitions. The 80115224 draft
requires a proof of valid menu states before treating the incoming s7 value as
irrelevant. The 80115CE4 RGB union and byte-copy representation need independent
review/native tests; an inactive union view and an unspecified fourth color byte
must not be mistaken for verified semantics. All pointer/register binding choices
in these drafts require the same lifetime audit as ordinary matching source.

The original seven function boundaries and exact reconstruction are recorded in
`configs/PAL/overlays/659.json`; use `tools/scripts/build_overlay659.py`.
