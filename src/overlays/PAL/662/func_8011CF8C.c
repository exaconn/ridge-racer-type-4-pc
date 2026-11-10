#include "rr/overlay662_update.h"
#include "rr/frame_storage_types.h"

void ovl662_func_8011CF8C(void)
{
    s32 first,second;
    if(ovl662_D_801276A8.fade00>=0) {
        union {u32 bits;s32 value;} intensity;
        if(ovl662_D_801276A8.fade00!=0)ovl662_D_801276A8.fade00--;
        intensity.bits=(64U-ovl662_D_801276A8.fade00)<<2;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,intensity.value,69);
    }
    if(ovl662_D_801276A8.flagsD1&1)UpdateBadgeRowAnim(0);
    if(ovl662_D_801276A8.flagsD1&2){DrawUiRow(0);DrawUiRow(1);}
    if(ovl662_D_801276A8.flagsD1&4){SkipDialoguePage(CardDialogueStatePtr);UpdateDialogue(22,320,CardDialogueStatePtr);}
    first = 0;
    do {
        if (ovl662_D_801276A8.fade00 < 0) {
            if (!(ovl662_D_801276A8.flagsD1 & 3)) {
                first = 1;
            } else if (ovl662_D_801276A8.flagsD1 & 4) {
                first = 1;
                second = first;
                break;
            }
        }
        /* ASM_BARRIER: retain the common second-flag assignment after the branch. */
        __asm__ ("" : : : "memory");
        second = 0;
    } while (0);
    SetMenuFrameOptions(first,second,0);
    UpdateScreenFade(0x1004E);
}
