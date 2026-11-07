#include "rr/cd.h"

u8 CdStatus(void) {
    return CD_status.bytes.status;
}
