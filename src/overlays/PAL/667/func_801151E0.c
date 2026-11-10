#include "rr/overlay667_packet.h"
void ovl667_transmit_local_inputs(void) {
    s32 state;
    if (ovl667_peer_race_state == RaceState && ovl667_peer_race_state >= 4) {
        if (ovl667_link_shutdown_stage == 0) ovl667_link_shutdown_stage = 1;
    } else if (ovl667_link_shutdown_stage <= 0) {
        state = ovl667_tx_stage;
        if (state == 1) {
            if (ovl667_send_race_packet() == state) {
                ovl667_tx_miss_count = 0;
                LinkLocalCarInputs.choices[0] = LinkPendingCarInputs.choices[0];
                LinkLocalCarInputs.choices[1] = LinkPendingCarInputs.choices[1];
                ovl667_tx_packet_count = (ovl667_tx_packet_count + 1U);
                ovl667_tx_slot_index = (ovl667_tx_slot_index + 1) & 7;
                ovl667_local_pause_bits = ((u8 *)&ovl667_pending_pause_bits)[0];
                if (ovl667_sync_retry_state == state) ovl667_sync_retry_state = 0;
                ovl667_tx_stage = 0;
            } else ovl667_tx_stage = 2;
        } else if (state == 2) {
            if (ovl667_send_race_packet() == 1) {
                ovl667_tx_miss_count = 0;
                LinkLocalCarInputs.choices[0] = LinkPendingCarInputs.choices[0];
                LinkLocalCarInputs.choices[1] = LinkPendingCarInputs.choices[1];
                ovl667_tx_packet_count = (ovl667_tx_packet_count + 1U);
                ovl667_tx_slot_index = (ovl667_tx_slot_index + 1) & 7;
                ovl667_local_pause_bits = ((u8 *)&ovl667_pending_pause_bits)[0];
                if (ovl667_sync_retry_state == 1) ovl667_sync_retry_state = 0;
                ovl667_tx_stage = 0;
            } else {
                ovl667_tx_stage = 0;
                ovl667_tx_miss_count = (ovl667_tx_miss_count + 1U);
                if (ovl667_sync_retry_state == 0) ovl667_sync_retry_state = 1;
                else if (ovl667_sync_retry_state == 1) ovl667_sync_retry_state = state;
            }
        }
    }
}
