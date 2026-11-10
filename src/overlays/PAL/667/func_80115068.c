#include "rr/overlay667_packet.h"

void ovl667_receive_peer_inputs(void) {
    if (ovl667_link_shutdown_stage > 0) {
        if (ovl667_link_shutdown_stage == 1) {
            ovl667_link_shutdown_stage = 2;
        }
    } else {
        ovl667_tx_stage = 1;
        if (ovl667_poll_race_packet() == 1) {
            ovl667_rx_miss_count = 0;
            ovl667_rx_idle_flag = 0;
            ovl667_rx_packet_count = (ovl667_rx_packet_count + 1U);
        } else {
            ovl667_rx_miss_count = (ovl667_rx_miss_count + 1U);
            if (ovl667_tx_slot_index == ovl667_rx_slot_index) {
                ovl667_active_slot_index = (ovl667_rx_slot_index + 7) & 7;
            } else if (ovl667_rx_slot_index == ovl667_tx_slot_index + 1) {
                ovl667_active_slot_index = ovl667_tx_slot_index;
                ovl667_sync_retry_state = 3;
            } else if (ovl667_tx_slot_index == ovl667_rx_slot_index + 2) {
                ovl667_active_slot_index = ovl667_rx_slot_index;
                ovl667_tx_slot_index = (ovl667_rx_slot_index + 9) & 7;
            } else {
                ovl667_active_slot_index = ovl667_rx_slot_index;
            }
        }
        LinkRemoteCarInputs.choices[0] = ovl667_rx_packet_ring.packets[ovl667_active_slot_index].first04;
        LinkRemoteCarInputs.choices[1] = ovl667_rx_packet_ring.packets[ovl667_active_slot_index].second08;
        ovl667_remote_pause_bits = ovl667_rx_packet_ring.packets[ovl667_active_slot_index].phase03;
    }
}
