#include "types.h"
#include "psyq/libspu/spu3.h"
#include "rr/spu.h"

extern unsigned int *SPU_DELAY;

void _spu_FsetDelayW(void);

void _spu_FsetDelayW(void) {
    *SPU_DELAY = (*SPU_DELAY & 0xF0FFFFFF) | 0x20000000;
}

void _spu_FsetDelayR(void) {
    *SPU_DELAY = (*SPU_DELAY & 0xF0FFFFFF) | 0x22000000;
}

void _spu_Fw1ts(void) {
    volatile struct {
        s32 counter;
        u32 accumulator;
    } work;
    work.accumulator = 13;
    for (work.counter = 0; work.counter < 60; work.counter++) {
        work.accumulator *= 13;
    }
}
