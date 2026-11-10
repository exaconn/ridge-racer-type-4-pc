#include "rr/overlay667_packet.h"

s32 ovl667_poll_race_packet(void) {
    u8 checksum;
    u32 i;
    if (TestEvent(ovl667_sio_read_event) == 1) {
        ovl667_rx_idle_flag = 0;
        ovl667_checksum_error_flag = 0;
        checksum = 0;
        i = 0;
        do {
            checksum ^= ((u8 *)&ovl667_rx_staging_packet.packet)[i+4];
            i++;
        } while (i < 8);
        if (checksum != ovl667_rx_staging_packet.packet.checksum01) ovl667_checksum_error_flag = 1;
        if (ovl667_rx_staging_packet.packet.flags00 >> 7) {
            if ((ovl667_rx_staging_packet.packet.flags00 >> 6) & 1) ovl667_remote_error_code = 2;
            else ovl667_remote_error_code = 1;
        }
        ovl667_rx_slot_index = ovl667_rx_staging_packet.packet.flags00;
        if (ovl667_tx_slot_index == ovl667_rx_slot_index) {
            ovl667_active_slot_index = (ovl667_rx_slot_index + 7) & 7;
        } else if (ovl667_rx_slot_index == ovl667_tx_slot_index + 1) {
            ovl667_active_slot_index = ovl667_tx_slot_index;
            ovl667_sync_retry_state = 3;
        } else if (ovl667_tx_slot_index == ovl667_rx_slot_index + 2) {
            ovl667_active_slot_index = ovl667_rx_slot_index;
            ovl667_tx_slot_index = (ovl667_rx_slot_index + 9) & 7;
        } else ovl667_active_slot_index = ovl667_rx_slot_index;
        ovl667_rx_packet_ring.packets[ovl667_rx_slot_index] = ovl667_rx_staging_packet.packet;
        FileRead(LinkReadHandle, &ovl667_rx_staging_packet.packet, 12);
        return 1;
    }
    if (ovl667_read_pending_flag != 0) return -1;
    _sio_control(2,0,0);
    FileRead(LinkReadHandle, &ovl667_rx_staging_packet.packet, 12);
    ovl667_read_pending_flag = 1;
    ovl667_rx_packet_count = (ovl667_rx_packet_count + 1U);
    return 0;
}
