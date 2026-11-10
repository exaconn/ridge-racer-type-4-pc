#include "game/system/pad.h"
#include "rr/overlay668_playback_step.h"

#define STR_STATE ((StrPlaybackState *)initialContext->activeB8)

void ovl668_step_stream_playback(void)
{
    s32 timer;
    u32 frame;
    CdLocation *location;
    StrContextPrefix *context;
    s32 pixels;
    StrContextPrefix *initialContext;

    timer = (ModeFrame + 1U);
    ModeFrame = timer;
    if (timer == 4) SetDispMask(1);
    initialContext = &RaceContext;
    ovl668_mdec_in(STR_STATE->commands10[STR_STATE->display24.word00], 3);
    pixels = STR_STATE->display24.rectangles[2].w * STR_STATE->display24.rectangles[2].h;
    context = initialContext;
    location = &context->currentC0;
    ovl668_mdec_out(STR_STATE->buffers18[STR_STATE->display24.word08], pixels / 2);
retry:
    if (ovl668_decode_next_frame(&((StrPlaybackState *)context->activeB8)->display24) == -1) {
        frame = StGetBackloc(location);
        DebugPrintStub(ovl668_backloc_debug_fmt, frame);
        if (frame >= context->thresholdC8) {
            context->stopD0 = 1;
            goto complete;
        }
        if ((s32)frame <= 0) context->currentC0 = context->locationBC;
        ovl668_seek_and_read_stream(location->c);
        goto retry;
    }
complete:
    ovl668_wait_frame_ready(&((StrPlaybackState *)RaceContext.activeB8)->display24, 0);
    DrawSync(0);
    ovl668_mdec_reset(1);
    if (RaceContext.stopD0 == 1) {
        RaceContext.phaseB4 = 2;
        SsSetSerialVol(0, 0, 0);
    }
    if (PlayerControllers[0].edges08.fields.pressed & 0x800) {
        SsSetSerialVol(0, 0, 0);
        RaceContext.phaseB4 = 2;
    }
}
