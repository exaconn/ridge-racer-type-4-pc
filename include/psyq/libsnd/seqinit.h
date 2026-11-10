#ifndef RR_PSYQ_LIBSND_SEQINIT_H
#define RR_PSYQ_LIBSND_SEQINIT_H

#include "types.h"

/* The callee decodes the signed low16 slot for both its table lookup and
 * the delta-decoder call. The entry accepts the complete selector word. */
s32 _SsInitSoundSeq(s32 slotInput,s32 bank,u8 *sequence);

#endif /* RR_PSYQ_LIBSND_SEQINIT_H */
