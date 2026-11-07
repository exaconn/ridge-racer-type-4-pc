/* GCC_VERSION: 2.7.2 */

#include "rr/gpu.h"

#include "rr/timing.h"

void set_alarm(void) {
    D_800ABBFC = (u32)VSync(-1) + 0xF0;
    GpuAlarmCount = 0;
}
