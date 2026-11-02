/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */

#include "rr/cd_init.h"

s32 func_8008D86C(void) {
    register volatile u8 *statusBytes;
    /* ASM_BARRIER: Keep command/result in a0 across the protocol calls. */
    register s32 command __asm__("$4");
    register const u8 *parameters;
    register volatile u8 *indexPort;
    func_800963DC(D_80011B08);
    /* The disabled logger consumes a32-bit PAL address value, never a pointer
     * used for access; hardware and callback pointers remain full-width. */
    func_8001E84C(D_80011B14, (u32)(unsigned long)&D_800A6768);
    D_800A64A5 = 0;
    D_800A64A4 = 0;
    D_800A648C = 0;
    D_800A6488 = 0;
    D_800A6498 = 0;
    D_800A6494.word = 0;
    func_8008B364();
    func_8008B394(2, func_8008DDAC);
    RR_CD_WRITE8(D_800A674C, 1);
    if (RR_CD_READ8(D_800A6758) & 7) {
        do {
            RR_CD_WRITE8(D_800A674C, 1);
            RR_CD_WRITE8(D_800A6758, 7);
            RR_CD_WRITE8(D_800A6754, 7);
        } while (RR_CD_READ8(D_800A6758) & 7);
    }
    command=1;parameters=0;
    /* ASM_BARRIER: Materialize command arguments before response reset. */
    __asm__("" : "=r"(command),"=r"(parameters) : "0"(command),"1"(parameters));
    statusBytes=D_800A6764;
    /* ASM_BARRIER: Retain the response base for all three volatile bytes. */
    __asm__("" : "=r"(statusBytes) : "0"(statusBytes));
    statusBytes[2] = 0U;
    statusBytes[1] = statusBytes[2];
    indexPort=D_800A674C;

    statusBytes[0] = 2;
    RR_CD_WRITE8(indexPort, 0);
    RR_CD_WRITE8(D_800A6758, 0);
    RR_CD_WRITE32(D_800A675C, 0x1325);
    func_8008D1C8(command, parameters, 0, 0);
    if (D_800A6494.word & 0x10) {
        func_8008D1C8(1, 0, 0, 0);
    }
    if (func_8008D1C8(0xA, 0, 0, 0) == 0) {
        if (func_8008D1C8(0xC, 0, 0, 0) == 0) {
            {
            /* ASM_BARRIER: Keep the common return value in v0. */
            register s32 result __asm__("$2");
            /* ASM_BARRIER: Preserve completion code2 separately in v1. */
            register s32 expected __asm__("$3");
            command=func_8008CC80(0,0);
            expected=2;
            /* ASM_BARRIER: Keep the full completion result and expected code. */
            __asm__("" : "=r"(command),"=r"(expected) : "0"(command),"1"(expected));
            if(command==expected){
                result=0;
                
                return result;
            }
            result=-1;
            
            return result;
            }
        }
        command=0;
        /* ASM_BARRIER: Retain the original a0=0 preparation on this cold edge. */
        __asm__("" : : "r"(command));
        return -1;
    }
    return -1;
}
