#include "rr/overlay668_stream.h"
#define STR_STATE ((StrPlaybackState *)RaceContext.activeB8)

void ovl668_start_stream_playback(u8 *arg0) {
    s32 vol;
    SetDispMask(0);
    STR_STATE->width00 = 320;
    STR_STATE->height04 = 240;
    SsSetSerialAttr(0, 0, 1);
    vol = (CdMusicMasterVolume * 127) / 16;
    SsSetSerialVol(0, vol, vol);
    VSyncThreshold = 128;
    ovl668_init_stream_buffers(arg0);
    ovl668_init_stream_display(&STR_STATE->display24, 0, 24, 0, 264);
    ovl668_setup_stream_ring(ovl668_on_mdec_slice_ready);
    while (ovl668_decode_next_frame(&STR_STATE->display24) == -1) {
        ovl668_seek_and_read_stream(RaceContext.currentC0.c);
    }
    RaceContext.phaseB4 = 1;
    RaceContext.counterC4 = 0;
    ModeFrame = 0;
    VSync(0);
}
