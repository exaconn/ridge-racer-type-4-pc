#ifndef RR_RELATIVE_FILE_H
#define RR_RELATIVE_FILE_H
#include "rr/layout.h"
#include "rr/course_view_types.h"
typedef union {u32 relative;u8 *absolute;} RelativeSlot;
typedef struct {u32 count;RelativeSlot first[3];u32 offsets[16];} RelativeHeader;
#if defined(mips)
RR_ASSERT_SIZE(RelativeSlot,4);
RR_ASSERT_SIZE(RelativeHeader,0x50);
RR_ASSERT_OFFSET(RelativeHeader,first,4);
RR_ASSERT_OFFSET(RelativeHeader,offsets,0x10);
#endif
extern RelativeHeader *D_800F2EBC;
extern s32 RelFileSegmentCount;
extern RelativeSlot *RelFileSegmentViewSlots;
extern u8 *D_800F391C;
extern CourseViewRecord *RelFileViewRecords;
extern u8 *RelFileSection00;
extern u8 *RelFileSection01;
extern u8 *D_800ADAE8;
extern u8 *RelFileOffsetPtr3;
extern u8 *D_800F4C60;
extern u8 *RelFileSection05;
extern u8 *RelFileOffsetPtr6;
extern u8 *RelFileOffsetPtr7;
extern u8 *D_800FFC40;
extern u8 *RelFileOffsetPtr9;
extern u8 *D_800ABEA0;
extern u8 *RelFileSection11;
extern u8 *RelFileOffsetPtr12;
extern u8 *RelFileOffsetPtr13;
extern u8 *D_800ABEB4;
extern u8 *RelFileSection15;

/* PAL adds modulo32; signed offsets retain negative displacements on LP64. */
static __inline__ u8 *rr_relative_file_address(RelativeHeader *base,u32 offset){return (u8*)((long)(s32)offset+(unsigned long)base);}

/* Eight relocation slots per record; count multiplication wraps modulo32.
 * The writable image contains the header followed immediately by those slots.
 * Offset words add modulo32 on PAL; native views retain full pointer widths. */
void RelocateRelativeFile(RelativeHeader *image);
#endif
