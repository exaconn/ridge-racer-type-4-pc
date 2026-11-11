#ifndef RR_PSYQ_LIBGTE_GTECOLO3_H
#define RR_PSYQ_LIBGTE_GTECOLO3_H

#include "types.h"
#include "rr/gpu.h"

/* PsyZ: void DpqColor(CVECTOR *v0, long p, CVECTOR *v1). */
void DpqColor(const GpuColorCommand *input, long factor, GpuColorCommand *output);

#endif /* RR_PSYQ_LIBGTE_GTECOLO3_H */
