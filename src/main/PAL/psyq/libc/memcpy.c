/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/memory.h"
u8 *PsyqMemmove(u8 *destination,const u8 *source,s32 length) {
    u8 *cursor=destination;
    if(cursor>=source) {
        while(length-- > 0) {
            cursor[length]=source[length];
        }
    } else {
        while(length-- > 0) {
            *cursor++=*source++;
        }
    }
    return cursor;
}
