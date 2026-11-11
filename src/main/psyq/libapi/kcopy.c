#include "rr/memory.h"
extern void StopCARD2(void);
extern void _memcard_patch_stub(void);
void _copy_memcard_patch(void) {
    unsigned long destination=0xDF80U;
    unsigned long source=(unsigned long)StopCARD2+0x10;
    unsigned long limit=(unsigned long)_memcard_patch_stub;
    do {
        *(u32 *)destination=*(const u32 *)source;
        source+=sizeof(u32);
        destination+=sizeof(u32);
    } while(source!=limit);
}
