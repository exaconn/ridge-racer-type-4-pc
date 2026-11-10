#ifndef RR_PSYQ_LIBGPU_SYS_DRAIN_H
#define RR_PSYQ_LIBGPU_SYS_DRAIN_H

#include "types.h"
#include "rr/gpu.h"
#include "rr/gpu_reset.h"
#include "rr/library.h"

struct GpuQueue {
    GpuOpFunc op;
    void *p0;
    unsigned long p1;
    int buf[21];
};

extern volatile struct GpuQueue GpuCommandQueue[];

void _addque(GpuOpFunc op, void *payload, unsigned long parameter);
int _addque2(GpuOpFunc op, void *payload, int length, unsigned long parameter);
int _exeque(void);

#endif /* RR_PSYQ_LIBGPU_SYS_DRAIN_H */
