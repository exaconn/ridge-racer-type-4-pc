#ifndef RR_PSYQ_LIBSIO_SIOSTART_H
#define RR_PSYQ_LIBSIO_SIOSTART_H
#include "types.h"
/* Link cable driver under its early libsio names (AddSIO/_sio_control and the
 * "sio:" device); src/main/psyq/libcomb forwards them to PsyZ libcomb. */
void AddSIO(void);
#endif /* RR_PSYQ_LIBSIO_SIOSTART_H */
