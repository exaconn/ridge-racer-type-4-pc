/* GCC_VERSION: 2.7.2 */
#include "rr/channels.h"

extern RrChannelControlRow36 D_8010F908[];
void func_80088D64(u8 index) {
    s32 offset = index * 54;
    /* ASM_BARRIER: retain the original assembler temporary for the final field store. */
    register u8 *base __asm__("$1");
    ((RrChannelControlRow36 *)((u8 *)D_8010F908 + offset))->flags1D = 0;
    ((RrChannelControlRow36 *)((u8 *)D_8010F908 + offset))->value00 = 0;
    /* ASM_BARRIER: finish the first two field stores before binding the last address. */
    __asm__ volatile("" : : "r"(offset) : "$1", "memory");
    /* PAL RAM mapping: D_8010F908 is base minus 0x6F8. */
    base = (u8 *)0x80110000U;
    /* ASM_BARRIER: expose the indexed field store in the return delay slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base += offset;
    ((RrChannelControlRow36 *)(base - 0x6F8))->value04 = 0;
}
