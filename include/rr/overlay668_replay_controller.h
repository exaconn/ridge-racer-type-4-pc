#ifndef RR_OVERLAY668_REPLAY_CONTROLLER_H
#define RR_OVERLAY668_REPLAY_CONTROLLER_H
#include "rr/overlay668_camera.h"
#include "rr/icon_sprites.h"
#include "rr/tables.h"
typedef struct {s32 key,action;} RrReplayEvent;
extern RrReplayEvent ovl668_D_80116CA8[3];
extern RrIconSpriteContext *D_800ACC14;
extern RrMenuItemState5C D_800F3A38[2];
extern s16 D_800F4C6A,D_800F3A26,D_800F4D8A;
void func_800207D4(u32*,s32,u32);void func_80052A30(s32);void func_80052B8C(u16,s32);void func_8006ECE8(s32);void func_80036FD4(s32);
void ovl668_func_80115BD0(s32);void ovl668_func_801159C0(s32,s32);
/* The three-entry event table contains a negative sentinel within its owner.
 * The camera owner covers byte0..4B7, including the low scene flag atB0 and
 * event counter at4B4. Services can mutate all camera words, input, text words
 * and the projection root; each fresh root selects a complete context.
 * Counter additions, fade steps and text subtraction wrap32; division by6
 * truncates toward zero. Each active text phase submits exactly62 entries. */
void ovl668_func_80115F80(void);
#endif
