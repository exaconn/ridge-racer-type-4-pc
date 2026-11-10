#ifndef RR_PSYQ_LIBSND_VMNOTE2_H
#define RR_PSYQ_LIBSND_VMNOTE2_H

#include "types.h"

unsigned short note2pitch(void);
/* Inputs use their signed low halfwords, matching the promoted SDK call. */
unsigned short note2pitch2(s32 note, s32 fine);

#endif /* RR_PSYQ_LIBSND_VMNOTE2_H */
