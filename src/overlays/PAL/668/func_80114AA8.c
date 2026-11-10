#include "rr/overlay668_dispatch.h"
#include "rr/load_cursor.h"
void ovl668_update_stream_mode(void) {
    switch (D_800AC704) {
    case 0:
        ovl668_start_stream_playback(LoadCursor);
        break;
    case 1:
        ovl668_step_stream_playback();
        break;
    case 2:
        ovl668_stop_stream_playback();
        break;
    }
}
