#ifndef RR_OVERLAY662_MODAL_H
#define RR_OVERLAY662_MODAL_H
#include "game/system/pad.h"
#include "rr/overlay662_tool_action.h"
#include "rr/menu_text_state.h"
#include "rr/tables.h"
#include "rr/menu_transition.h"
extern s32 ovl662_emblem_edit_from_menu;
/* Negative modes reset the first option index. Modes1/2/4/8 configure
 * navigation; other positive modes preserve state. Mode0 enters or operates
 * the modal tool using two live UI rows and complete badge/text owners.
 * Services may replace those roots and mutate later selectors and input.
 * Modal state is captured after sound before publishing row states. */
void ovl662_tool_undo_modal(s32 mode);
#endif
