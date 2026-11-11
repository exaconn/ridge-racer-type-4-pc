/* The game links the link cable driver under its early libsio names; they
 * are PsyZ libcomb (AddCOMB/_comb_control), which reports no connected peer. */
#include <libcomb.h>
#include "psyq/libsio/siostart.h"
#include "rr/driver_control.h"

void AddSIO(void) { AddCOMB(); }

s32 _sio_control(s32 command, s32 operation, unsigned long argument) {
    return (s32)_comb_control((u32)command, (u32)operation, argument);
}
