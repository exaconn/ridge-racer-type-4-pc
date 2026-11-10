#include "rr/overlay667_events.h"

void ovl667_reset_sio_ports(void) {
    _sio_control(0, 0, 0);
    _sio_control(0, 1, 0);
}
