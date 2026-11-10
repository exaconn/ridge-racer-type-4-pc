#ifndef RR_PSYQ_LIBPAD_PADENAB_H
#define RR_PSYQ_LIBPAD_PADENAB_H

#include "rr/layout.h"

typedef s32 (*PadInterruptHandler)(void);
enum { PAD_INTERRUPT_HANDLER=0, PAD_INTERRUPT_TEST=1 };
typedef struct PadInterruptQueueRecord {
    struct PadInterruptQueueRecord *next00;
    PadInterruptHandler callbacks04[2];
    u32 reserved0C;
} PadInterruptQueueRecord;
extern PadInterruptQueueRecord PadIntQueueEntry;
#if defined(__mips__)
RR_ASSERT_SIZE(PadInterruptQueueRecord,0x10);
RR_ASSERT_OFFSET(PadInterruptQueueRecord,callbacks04,4);
RR_ASSERT_OFFSET(PadInterruptQueueRecord,reserved0C,0xC);
#endif
s32 _padChkVsync(void);

#endif /* RR_PSYQ_LIBPAD_PADENAB_H */
