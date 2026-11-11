/* BIOS bzero (A(28h)) and memset (A(2Bh)) as host C library calls. */
#include <string.h>
#include "rr/memory.h"

void PsyqBzero(void *buffer, s32 size);

void PsyqBzero(void *buffer, s32 size) {
    if (buffer != NULL && size > 0) {
        memset(buffer, 0, (size_t)size);
    }
}

void *PsyqMemset(void *destination, s32 value, u32 length) {
    if (destination != NULL) {
        memset(destination, value & 0xFF, length);
    }
    return destination;
}
