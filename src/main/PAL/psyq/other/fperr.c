/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
extern s32 _err_math_mode;
extern s32 _err_math_param;
extern s32 DeliverEvent(u32, u32);
s32 _err_math(s32 mode, s32 parameter) {
    s32 *page;
    register s32 first_mode=33;
    _err_math_mode=mode;
    page=(s32 *)0x800A0000U;
    page[0x47D4/4]=parameter;
    switch(mode) {
    case 33: DeliverEvent(0xF4000002U,0x301); break;
    case 34: DeliverEvent(0xF4000002U,0x302); break;
    }
    return 0;
}
