#ifndef RR_OVERLAY665_AUDIO_OPTIONS_H
#define RR_OVERLAY665_AUDIO_OPTIONS_H
#include "rr/overlay665_menu.h"
#include "rr/runtime_settings.h"
extern s32 ovl665_saved_bgm_volume,ovl665_saved_sfx_volume;
extern s16 D_800F7640;
void SeqSetVolume(s16);
void SetCdMixMode(s32);
void ovl665_init_audio_options(void);
/* Eight complete UI rows and a full0xB70 frame context are available. The
 * word index stays0..3; its signed low half is edited by width2 input. Mono is
 *0..1 whenever indexed. Services may replace roots and all mutable settings.
 * Timers wrap signed16; layout and scene updates wrap32. */
void ovl665_update_audio_options(void);
#endif
