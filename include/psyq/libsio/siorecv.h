#ifndef RR_PSYQ_LIBSIO_SIORECV_H
#define RR_PSYQ_LIBSIO_SIORECV_H

#include "types.h"

extern short SioRxFifoSizeTable[];
int _sio_read(u8 *buffer, int length);

#endif /* RR_PSYQ_LIBSIO_SIORECV_H */
