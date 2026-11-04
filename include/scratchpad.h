#ifndef RR_SCRATCHPAD_H
#define RR_SCRATCHPAD_H

/* R3000A data-cache scratchpad (1 KB at 0x1F800000). The game keeps its GTE work areas here. */
#define SCRATCHPAD 0x1F800000

/* Roles read off the code that uses them. */
#define SCRATCH_ROT_MATRIX (SCRATCHPAD + 0x28)  /* current rotation matrix: loaded into the GTE (SETROT), passed as MATRIX/Mat8 */
#define SCRATCH_CAR_WORK   (SCRATCHPAD + 0x80)  /* per-car physics work area, see game/car/carsp.h */

#endif
