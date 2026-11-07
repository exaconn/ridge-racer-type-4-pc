#include "rr/overlay668_playback_step.h"

#define STR_STATE ((RrStrPlaybackState *)initialContext->activeB8)

void ovl668_func_80114C3C(void)
{
    s32 timer;
    u32 frame;
    RrCdLocation *location;
    RrStrContextPrefix *context;
    s32 pixels;
    RrStrContextPrefix *initialContext;

    timer = (s32)((u32)D_800F2DE4 + 1U);
    D_800F2DE4 = timer;
    if (timer == 4) SetDispMask(1);
    initialContext = &RaceContext;
    ovl668_func_801163FC(STR_STATE->commands10[STR_STATE->display24.word00], 3);
    pixels = STR_STATE->display24.rectangles[2].w * STR_STATE->display24.rectangles[2].h;
    context = initialContext;
    location = &context->currentC0;
    ovl668_func_80116478(STR_STATE->buffers18[STR_STATE->display24.word08], pixels / 2);
retry:
    if (ovl668_func_801150D8(&((RrStrPlaybackState *)context->activeB8)->display24) == -1) {
        frame = (u32)StGetBackloc(location);
        func_8001E84C(ovl668_D_8011487C, frame);
        if (frame >= context->thresholdC8) {
            context->stopD0 = 1;
            goto complete;
        }
        if ((s32)frame <= 0) context->currentC0 = context->locationBC;
        ovl668_func_801153F0(location->c);
        goto retry;
    }
complete:
    ovl668_func_8011535C(&((RrStrPlaybackState *)RaceContext.activeB8)->display24, 0);
    DrawSync(0);
    ovl668_func_80116298(1);
    if (RaceContext.stopD0 == 1) {
        RaceContext.phaseB4 = 2;
        SsSetSerialVol(0, 0, 0);
    }
    if (D_800F3A38[0].buttons08 & 0x800) {
        SsSetSerialVol(0, 0, 0);
        RaceContext.phaseB4 = 2;
    }
}
