#ifndef RR_SAVE_PROGRESS_H
#define RR_SAVE_PROGRESS_H
#include "rr/layout.h"
#include "rr/selection_types.h"
typedef union {u8 bytes[16];struct {char name[12];s32 configuration0C;} fields;} RrSaveHeader16;
typedef union {u8 bytes[12];s32 words[3];} RrSaveReserved12;

/* Sixteen progress masks shared by the garage counts and result screens. */
typedef struct {RrSaveHeader16 prefix00;u32 progress10[4][4];} RrSaveProgressPrefix50;
typedef struct {
    RrSaveProgressPrefix50 progress;
    RrSaveReserved12 reserved50;
    s8 flags5C[4];
} RrSaveProgressRecord60;
/* Resident initialization clears0x7C bytes and seeds four records at0x6C. */
typedef struct {
    RrSaveProgressRecord60 prefix;
    u8 unlocked60[9],reserved69[3];
    RrSelectionRow history6C[4];
} RrSaveProgressRecord7C;
extern RrSaveProgressRecord7C SaveProgress;
RR_ASSERT_SIZE(RrSaveHeader16,16);
RR_ASSERT_SIZE(RrSaveReserved12,12);
RR_ASSERT_SIZE(RrSaveProgressRecord7C,0x7C);
RR_ASSERT_OFFSET(RrSaveProgressRecord7C,unlocked60,0x60);
RR_ASSERT_OFFSET(RrSaveProgressRecord7C,history6C,0x6C);
RR_ASSERT_SIZE(RrSaveProgressPrefix50,0x50);
RR_ASSERT_OFFSET(RrSaveProgressPrefix50,progress10,0x10);
RR_ASSERT_SIZE(RrSaveProgressRecord60,0x60);
RR_ASSERT_OFFSET(RrSaveProgressRecord60,flags5C,0x5C);
#endif
