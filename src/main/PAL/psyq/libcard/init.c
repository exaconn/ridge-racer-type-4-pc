/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "psyq/libapi/pad1.h"
#include "rr/memory.h"
#include "rr/spu_tick_control.h"

extern int func_80095DD4();
extern int _patch_card();
extern int _patch_card2();
extern int func_80096F84();
int func_80095DE4();
int StopCARD2();
/* extern */
int _ExitCard();

void InitCARD(s32 arg0);
void StartCARD(void);
void StopCARD(void);

void InitCARD(s32 arg0) {
    s32 var_s0;

    var_s0 = arg0;
    func_80096F84(0);
    EnterCriticalSection();
    if (ReadInitPadFlag() == 0) {
        var_s0 = 0;
    }
    func_80095DD4(var_s0);
    _copy_memcard_patch();
    _patch_card();
    _patch_card2();
    ExitCriticalSection();
}

/* legacy typing of shared globals in this part of the unit */
#define func_80096F84 ((int (*)(int))func_80096F84)

/* extern */
void StartCARD(void) {
    EnterCriticalSection();
    func_80095DE4();
    func_80096F84(0);
    ExitCriticalSection();
}

#undef func_80096F84

/* extern */
void StopCARD(void) {
    StopCARD2();
    _ExitCard();
}
