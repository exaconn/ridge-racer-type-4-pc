#ifndef RR_SCRATCHPAD_H
#define RR_SCRATCHPAD_H

#include <stdint.h>
#include "types.h"

/* The R3000A data-cache scratchpad (1 KB at 0x1F800000) is an ordinary host
 * buffer. Scalar work areas keep their original byte offsets. Host pointers
 * never live in it: pointer state uses ordinary globals (GpuScratch). */
#define RR_SCRATCHPAD_SIZE 0x400
extern u8 RrScratchpad[RR_SCRATCHPAD_SIZE];
#define SCRATCHPAD ((uintptr_t)RrScratchpad)

/* Roles read off the code that uses them. */
#define SCRATCH_ROT_MATRIX (SCRATCHPAD + 0x28)  /* current rotation matrix: loaded into the GTE (SETROT), passed as MATRIX/Mat8 */
#define SCRATCH_CAR_WORK   (SCRATCHPAD + 0x80)  /* per-car physics work area, see game/car/carsp.h */

#endif
