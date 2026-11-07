/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/cd_init.h"

s32 CD_init(void) {
    volatile u8 *statusBytes;
    s32 command;
    const u8 *parameters;
    volatile u8 *indexPort;
    func_800963DC(D_80011B08);
    /* The disabled logger consumes a32-bit PAL address value, never a pointer
     * used for access; hardware and callback pointers remain full-width. */
    func_8001E84C(D_80011B14, (u32)(unsigned long)&D_800A6768);
    D_800A64A5 = 0;
    D_800A64A4 = 0;
    CD_cbready = 0;
    CD_cbsync = 0;
    CD_status1 = 0;
    CD_status.word = 0;
    ResetCallback();
    InterruptCallback(2, func_8008DDAC);
    RR_CD_WRITE8(D_800A674C, 1);
    if (RR_CD_READ8(D_800A6758) & 7) {
        do {
            RR_CD_WRITE8(D_800A674C, 1);
            RR_CD_WRITE8(D_800A6758, 7);
            RR_CD_WRITE8(D_800A6754, 7);
        } while (RR_CD_READ8(D_800A6758) & 7);
    }
    command=1;parameters=0;
    statusBytes=D_800A6764;
    statusBytes[2] = 0U;
    statusBytes[1] = statusBytes[2];
    indexPort=D_800A674C;

    statusBytes[0] = 2;
    RR_CD_WRITE8(indexPort, 0);
    RR_CD_WRITE8(D_800A6758, 0);
    RR_CD_WRITE32(D_800A675C, 0x1325);
    CD_cw(command, parameters, 0, 0);
    if (CD_status.word & 0x10) {
        CD_cw(1, 0, 0, 0);
    }
    if (CD_cw(0xA, 0, 0, 0) == 0) {
        if (CD_cw(0xC, 0, 0, 0) == 0) {
            {
            s32 result;
            s32 expected;
            command=CD_sync(0,0);
            expected=2;
            if(command==expected){
                result=0;
                return result;
            }
            result=-1;
            return result;
            }
        }
        command=0;
        return -1;
    }
    return -1;
}
