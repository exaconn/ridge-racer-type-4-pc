#include "game/system/pad.h"
#include "rr/overlay668_replay_controller.h"
#include "rr/frame_storage_types.h"
void ovl668_update_epilogue_credits(void) {
    s32 temp_a1;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_s0;
ReplayCameraContextPrefix *ctx;

    switch (RaceContext.counter444) {                          
    case 0:
        ovl668_init_epilogue_credits();
        RaceContext.counter444=RaceContext.counter444+1U;
        break;
    case 4:
        SetDispMask(1);
    case 1: case 2: case 3:
        RaceContext.counter444=RaceContext.counter444+1U;
        break;
    case 5:                                        
        var_a0 = -1;
        for(var_s0=0;ovl668_epilogue_events[var_s0].key>=0;var_s0++){
          if(ovl668_epilogue_events[var_s0].key==RaceContext.state44C){var_a0=ovl668_epilogue_events[var_s0].action;break;}
        }
        switch (var_a0) {                          
        case 0:                                    
            RaceContext.phase4AC = 1;
            RaceContext.counter4B4 = 0;
            CdAudioBeginPlay(0);
            break;
        case 1:                                    
            RaceContext.mode4A8 = 2;
            break;
        }
        ovl668_step_epilogue_camera_yaw();
        ctx=&RaceContext;
        ctx->position450[0] = ((u32)ctx->position450[0] + (ctx->delta478[0] / 6));
        temp_a1 = ctx->delta478[2] / 6;
        ctx->position450[2] = ((u32)ctx->position450[2] + temp_a1);
        ovl668_project_camera_to_track(ctx->position450);
        ovl668_step_epilogue_camera_pitch();
        ovl668_apply_epilogue_camera();
        ovl668_emit_letterbox_pass(2);
        temp_s1 = ctx->mode4A8;
        switch (temp_s1) {                         
        case 1:                                    
            temp_v0 = ctx->counter4A4 + 4U;
            ctx->counter4A4 = temp_v0;
            if (temp_v0 >= 0xFF) {
                ctx->counter4A4 = 0xFF;
                ctx->mode4A8 = 0;
}
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (255U - ctx->counter4A4), 0x45);
            break;
        case 2:                                    
            temp_v0_2 = ctx->counter4A4 - 4U;
            ctx->counter4A4 = temp_v0_2;
            if (temp_v0_2 <= 0) {
                ctx->counter4A4 = 0;
                ctx->mode4A8 = 0;
                CdAudioFade(0, 0);
                ModeGroup = (s16) temp_s1;
                ModeIndex = 3;
                ctx->flagB0 = 1;
                SubmodeState = 0;
            }
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], (255U - ctx->counter4A4), 0x45);
            break;
        }
        temp_v1 = RaceContext.phase4AC;
        if (temp_v1 != 0) {
            if (temp_v1 == 1) {
                var_s0 = 0;
                EpilogueCreditScrollY = (u32)EpilogueCreditScrollY - EpilogueCreditScrollStep;
                for(;var_s0<62;var_s0++)ovl668_draw_credit_band(var_s0,160);
            }
        }
        if ((PlayerControllers[0].edges08.fields.pressed & 0x800) && (RaceContext.mode4A8 == 0)) {
            RaceContext.mode4A8 = 2;
            CdAudioFade(0, 0x15);
        }
        SetupPlayerView(0);
        DrawTrackSceneryForPlayer(0);
        RaceContext.state44C = (RaceContext.state44C + 1U);
        ovl668_emit_letterbox_pass(1);
        ovl668_emit_letterbox_pass(0);
        /* fallthrough */
    default:                                       
        return;
    }
}
