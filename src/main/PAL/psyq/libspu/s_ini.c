/* GCC_VERSION: 2.7.2 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "rr/spu.h"
#include "psyq/libspu/s_crwa.h"
extern s32 _spu_isCalled;
extern s32 _spu_EVdma;
extern void _spu_FiDMA(void);
extern s32 EnterCriticalSection(void);
extern s32 ExitCriticalSection(void);
extern s32 OpenEvent(u32,s32,s32,LibInterruptCallback);
extern s32 EnableEvent(s32);
void SpuStart(void) {
    if(_spu_isCalled==0) {
        _spu_isCalled=1;
        EnterCriticalSection();
        _SpuDataCallback(_spu_FiDMA);
        _spu_EVdma=OpenEvent(0xF0000009U,0x20,0x2000,0);
        EnableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}
