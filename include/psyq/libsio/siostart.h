#ifndef RR_PSYQ_LIBSIO_SIOSTART_H
#define RR_PSYQ_LIBSIO_SIOSTART_H

#include "types.h"

void AddSIO(void);
s32 _sio_write_async(u8 *buffer, s32 length);

#endif /* RR_PSYQ_LIBSIO_SIOSTART_H */
