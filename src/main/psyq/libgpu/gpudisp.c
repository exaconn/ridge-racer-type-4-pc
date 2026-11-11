/* libgpu SetDrawStp: PsyZ declares it but does not implement it. The packet
 * is the PsyZ DR_STP layout (host tag plus two u_long command words). */
#include "psyq/libgpu/gpudisp.h"

void SetDrawStp(GpuDrawModePacket *packet, int enabled) {
    packet->tag.length = 2;
    packet->command[0] = enabled ? 0xE6000001u : 0xE6000000u;
    packet->command[1] = 0;
}
