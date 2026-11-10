#include "rr/overlay667_packet.h"
s32 ovl667_send_race_packet(void) {
    u8 csum;
    u32 i;
    if (_sio_control(3, 1, 0) == 0) {
        return 0;
    }
    ovl667_tx_staging_packet.packet.flags00 = ovl667_tx_slot_index;
    if (ovl667_checksum_error_flag == 1) {
        ovl667_tx_staging_packet.packet.flags00 |= 0x80;
    }
    csum = 0;
    i = 0;
    ovl667_tx_staging_packet.packet.first04 = LinkPendingCarInputs.choices[0];
    ovl667_tx_staging_packet.packet.second08 = LinkPendingCarInputs.choices[1];
    do {
        csum ^= ((u8 *)&ovl667_tx_staging_packet)[i + 4];
        i++;
    } while (i < 8);
    ovl667_tx_staging_packet.packet.checksum01 = csum;
    ovl667_tx_staging_packet.packet.status02 = RaceState;
    ovl667_tx_staging_packet.packet.phase03 = ovl667_pending_pause_bits;
    if (FileWrite(LinkWriteHandle, &ovl667_tx_staging_packet, 12) != 12) {
        return 2;
    }
    return 1;
}
