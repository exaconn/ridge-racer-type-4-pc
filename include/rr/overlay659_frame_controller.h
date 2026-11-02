#ifndef RR_OVERLAY659_FRAME_CONTROLLER_H
#define RR_OVERLAY659_FRAME_CONTROLLER_H
#include "rr/overlay666_race_controller.h"
#include "rr/menu_choice.h"

extern s16 D_800AD522;
extern s32 D_800F2EB8;
extern s32 D_800F2A68;
extern s32 D_800F2A64;
extern s32 D_800AC6EC;
extern RrRaceVehicle *D_800FFC20[8];
extern u8 ovl659_D_80116530[4];
extern s32 D_80107190;

typedef struct {
    u8 *cursor;
    u32 *order;
    u8 pad08[0x70];
    s32 field78;
} RrOvl659Scratch;
#ifndef RR_659_SCRATCH
#define RR_659_SCRATCH ((RrOvl659Scratch *)0x1F800000U)
#endif

void func_8003BFC8(void);
void func_8002BA28(s32);
void func_8002BC7C(s32);
void func_8003C254(RrRaceVehicle *);
void func_80045868(RrRaceVehicle *);
void func_80033808(RrRaceVehicle *);

/* Both frame banks and two resident vehicles remain live. The eight-slot
 * vehicle table may select resident vehicles or other complete320-byte records.
 * Settings phase17 (also the physical AD4EF alias) stays0..7 across callbacks;
 * its two uses index the four-byte class threshold table.
 * All software pointers remain full width. Frame/pulse/budget, negation and
 * wipe-height arithmetic wrap32; countdown and input fields retain wire widths.
 * Services may mutate globals, frame/vehicle roots and both scratch pointers.
 * Scratchpad suffix78 is a real word; native fixtures widen pointer slots.
 */
void ovl659_func_80114908(void);
#if defined(mips)
RR_ASSERT_SIZE(RrOvl659Scratch,0x7C);
RR_ASSERT_OFFSET(RrOvl659Scratch,field78,0x78);
#endif
#endif
