#ifndef RR_OVERLAY665_MENU_H
#define RR_OVERLAY665_MENU_H
#include "rr/menu_arenas.h"
#include "rr/menu_setup.h"
#include "rr/menu_text_state.h"
#include "rr/renderer_setup.h"
#include "rr/overlay665_visual_types.h"
#include "rr/overlay_menu.h"
#include "rr/menu_pulse.h"
#include "rr/frame_environment.h"
typedef struct {u8 reserved00[0x18];s32 mode18;} ReplayMenuContextPrefix;
extern ReplayMenuContextPrefix RaceContext;
extern Overlay665VisualState ovl665_options_state;
extern s16 FadeCountdown;
extern u8 ScreenFadeState[12];
extern s32 ovl665_saved_stereo_mode;
void RaceLoadReset(void);
void SeqStartTrack(s32);
void AddScreenFadeTiles(u32 *,s32,u32);
void SetPresentationDrawEnabled(s32);
void SeqFadeVolume(s32,s32);
void DrawUiRow(s32);
void UpdateScreenFade(s32);
void ovl665_init_controller_options(void);
void ovl665_init_menu(void);
void ovl665_update_menu(void);
/* Helpers may replace the full text-state root. */
RR_ASSERT_SIZE(ReplayMenuContextPrefix,0x1C);
RR_ASSERT_OFFSET(Overlay665VisualState,selection08,8);
RR_ASSERT_OFFSET(Overlay665VisualState,exit34,0x34);
#endif
