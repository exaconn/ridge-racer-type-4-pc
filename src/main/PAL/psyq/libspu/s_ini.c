/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
#include "rr/spu.h"
#include "psyq/libspu/s_crwa.h"
extern s32 _spu_isCalled;
extern s32 EnterCriticalSection(void);
extern s32 ExitCriticalSection(void);
extern s32 OpenEvent(u32,s32,s32,RrLibInterruptCallback);
extern s32 EnableEvent(s32);
void SpuStart(void) {
    if(_spu_isCalled==0) {
        {
            s32 one=1;
            /* ASM_BARRIER: Keep the initialized-flag store relative to the original assembler temporary. */
            register s32 *page __asm__("$1");
            /* ASM_BARRIER: Materialize the flag page after the initialization gate. */
            __asm__ volatile("" : : "r"(one) : "$1", "memory");
            page=(s32 *)0x800A0000U;
            /* ASM_BARRIER: Expose the initialized-flag write in the critical-section call delay slot. */
            __asm__("" : "=r"(page) : "0"(page));
            page[0x48A8/4]=one;
        }
        EnterCriticalSection();
        {
            unsigned long page=0x80080000U;
            /* ASM_BARRIER: Materialize the stop-callback page before its signed call-delay offset. */
            __asm__("" : "=r"(page) : "0"(page));
            _SpuDataCallback((RrLibInterruptCallback)(page-0x10FC));
        }
        {
            /* ASM_BARRIER: Preserve the opened event as the next call's first argument. */
            register s32 event __asm__("$4");
            /* ASM_BARRIER: Keep event publication relative to the original assembler temporary. */
            register s32 *page __asm__("$1");
            event=OpenEvent(0xF0000009U,0x20,0x2000,0);
            page=(s32 *)0x800A0000U;
            /* ASM_BARRIER: Expose event publication in the enable-event call delay slot. */
            __asm__("" : "=r"(page) : "0"(page));
            page[0x4840/4]=event;
            EnableEvent(event);
        }
        ExitCriticalSection();
    }
}
