/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "psyq/libapi/pad1.h"
#include "rr/memory.h"
#include "rr/spu_tick_control.h"

extern int InitCARD2();
extern int _patch_card();
extern int _patch_card2();
extern int ChangeClearPAD();
int StartCARD2();
int StopCARD2();
/* extern */
int _ExitCard();

void InitCARD(s32 arg0);
void StartCARD(void);
void StopCARD(void);

void InitCARD(s32 arg0) {
    s32 var_s0;

    var_s0 = arg0;
    ChangeClearPAD(0);
    EnterCriticalSection();
    if (ReadInitPadFlag() == 0) {
        var_s0 = 0;
    }
    InitCARD2(var_s0);
    _copy_memcard_patch();
    _patch_card();
    _patch_card2();
    ExitCriticalSection();
}

/* legacy typing of shared globals in this part of the unit */
#define ChangeClearPAD ((int (*)(int))ChangeClearPAD)

/* extern */
void StartCARD(void) {
    EnterCriticalSection();
    StartCARD2();
    ChangeClearPAD(0);
    ExitCriticalSection();
}

#undef ChangeClearPAD

/* extern */
void StopCARD(void) {
    StopCARD2();
    _ExitCard();
}
