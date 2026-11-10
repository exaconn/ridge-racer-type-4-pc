#ifndef RR_AUDIO_BANK_SETUP_TYPES_H
#define RR_AUDIO_BANK_SETUP_TYPES_H
#include "rr/layout.h"

/* Shared descriptor for both player and computer sound-bank constructors.
 * Opening the bank updates its program records in the supplied buffer. */
typedef struct AudioBankSetup {
 u8 *bank00;
 const u16 *data04;
 const u8 *group08;
 s32 slot0C;
} AudioBankSetup;
/* PAL layout is fixed; native pointer fields widen with the host ABI. */
#if defined(__mips__)
RR_ASSERT_SIZE(AudioBankSetup,0x10);
RR_ASSERT_OFFSET(AudioBankSetup,slot0C,0x0C);
#else
RR_ASSERT_OFFSET(AudioBankSetup,slot0C,3*sizeof(void *));
#endif
#endif
