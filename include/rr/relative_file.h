#ifndef RR_RELATIVE_FILE_H
#define RR_RELATIVE_FILE_H
#include "rr/layout.h"
typedef union {u32 relative;u8 *absolute;} RrRelativeSlot;
typedef struct {u32 count;RrRelativeSlot first[3];u32 offsets[16];} RrRelativeHeader;
RR_ASSERT_SIZE(RrRelativeSlot,4);
RR_ASSERT_SIZE(RrRelativeHeader,0x50);
RR_ASSERT_OFFSET(RrRelativeHeader,first,4);
RR_ASSERT_OFFSET(RrRelativeHeader,offsets,0x10);
extern RrRelativeHeader *D_800F2EBC;
extern s32 D_800F39A4;
extern RrRelativeSlot *D_800F3820;
extern u8 *D_800F391C,*D_800F2EB4;
extern u8 *D_800F29E0;
extern u8 *D_800F4D34;
extern u8 *D_800ADAE8;
extern u8 *D_800F4D28;
extern u8 *D_800F4C60;
extern u8 *D_800ABEA8;
extern u8 *D_800F4C20;
extern u8 *D_800ABE9C;
extern u8 *D_800FFC40;
extern u8 *D_800F4C30;
extern u8 *D_800ABEA0;
extern u8 *D_800F4C78;
extern u8 *D_800ABEB8;
extern u8 *D_800F4C6C;
extern u8 *D_800ABEB4;
extern u8 *D_800ACC04;

/* PAL adds modulo32; signed offsets retain negative displacements on LP64. */
static __inline__ u8 *rr_relative_file_address(RrRelativeHeader *base,u32 offset){return (u8*)((long)(s32)offset+(unsigned long)base);}

/* Eight relocation slots per record; count multiplication wraps modulo32.
 * The writable image contains the header followed immediately by those slots.
 * Offset words add modulo32 on PAL; native views retain full pointer widths. */
void func_8007026C(RrRelativeHeader *image);
#endif
