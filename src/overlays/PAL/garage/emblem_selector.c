#include "rr/overlay662_image_fade.h"
#include "rr/frame_storage_types.h"
#include "rr/overlay662_palette_initializer.h"
#include "rr/load_cursor.h"
#include "rr/overlay662_transition.h"
#include "game/system/pad.h"
#include "rr/overlay662_palette_controller.h"

s32 ovl662_draw_emblem_preview_fade(s32 mode) {
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
 GarageFadeGeometry quad;
 s32 amount=ovl662_emblem_preview_fade;
 s32 high,low,uv;
 s32 i,*x,*y,*u,*v;
 s32 color;
 void *cursor;
 u16 previous;

 s32 fresh;
 if(amount>=129)amount=128;
 i=0;
 high=256;
 low=128;
 uv=63;

 v=quad.v;
 u=quad.u;
 y=quad.y;
 x=quad.x;
 do {
  *x=((i&1)<<6)+128;
  if(i>=2)*y=high;else *y=low;
  *u=(i&1)*63;
  if(i>=2)*v=uv;else *v=0;
  v++;
  u++;
  y++;
  i++;
  x++;
 }while(i<4);
 if(mode==1){
  amount=rsin(amount*8)/64;
  quad.y[0]+=amount;quad.y[1]=quad.y[0];
  quad.y[2]-=amount;quad.y[3]=quad.y[2];amount=0;
 }
 color=64-rsin((ScreenFrame*64U))/64;
 cursor=RR_GPU_SCRATCH->cursor;
 if(amount>0){
  cursor=EmitPolyFT3(&CurrentDrawBuf->buffer.ordering70.words[0],cursor,quad.x,quad.y,quad.u,quad.v,42,0x1028,0,(u16)amount);
  cursor=EmitPolyFT3(&CurrentDrawBuf->buffer.ordering70.words[0],cursor,quad.x+1,quad.y+1,quad.u+1,quad.v+1,42,0x1028,0,(u16)amount);
  cursor=AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0],cursor,quad.x[0],quad.y[0],((u32)quad.x[1]-quad.x[0]),((u32)quad.y[2]-quad.y[0]),color,color,color);
 }else cursor=AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0],cursor,quad.x[0],quad.y[0],((u32)quad.x[1]-quad.x[0]),((u32)quad.y[2]-quad.y[0]),0,0,0);
 fresh=ovl662_emblem_preview_fade;

 previous=(u16)ovl662_emblem_preview_fade;
 RR_GPU_SCRATCH->cursor=cursor;

 if(fresh>=128)ovl662_emblem_preview_fade=128;
 else {
  ovl662_emblem_preview_fade=previous+4;
  if(mode==1){
   ovl662_emblem_preview_fade=previous+8;
  }
 }
}

void ovl662_init_emblem_selector(void) {
 s32 i;s16 val;BadgeRow1C *badge;
 ModeFrame=0;
 ScreenFrame=0;
 ovl662_menu_substate=0;
 ovl662_history_arrow_phase=0;
 MenuCursor=0;
 LoadCursor=MenuArenas.second;
 if(!ovl662_emblem_edit_from_menu)MenuItemCount=2;else MenuItemCount=1;
 InitRect16Row(0,128,128,64,128,0,0);
 InitUiRow(0,12,64,68,32,0,0,1,0,83);
 InitUiRow(1,12,104,68,32,0,0,1,0,85);
 InitBadgeRow(0,108,320,112,34,0,0x601);
 badge=BadgeRows;ovl662_emblem_choice_count=25;badge->half02=1;
 for(i=0;i<25;i++) {
  if(i>=21&&SaveProgress.prefix.reserved50.bytes[i-13]==0)ovl662_emblem_choice_map[i]=25;
  else ovl662_emblem_choice_map[i]=i;
 }
 ovl662_emblem_preview_fade=0;
 if(ovl662_emblem_edit_from_menu==1){ovl662_selected_emblem_id=0;ovl662_selected_emblem_slot=0;}
 else{val=CourseChoiceTable[1][ovl662_selected_car_slot].second;ovl662_selected_emblem_id=val;
 if((s16)val>0)ovl662_selected_emblem_slot=val-1;else ovl662_selected_emblem_slot=0;}
 ovl662_selected_emblem_id=(s8)ovl662_emblem_choice_map[ovl662_selected_emblem_slot];
 val=ovl662_selected_emblem_id;
 if(!val)UploadCarClut(CustomEmblemTimPtr,0,0);else UploadCarClut(PresetEmblemTimPtr,val-1,0);
 ovl662_menu_substate=0;
}

void ovl662_enter_emblem_selector(void) {
    ovl662_init_emblem_selector();
    ModeGroup = 1;
    ModeIndex = 0x46;
    SubmodeState = 0;
    UpdateScreenFade(0x10002);
}

void ovl662_update_emblem_selector(void) {
    s32 badgePhase;
    s32 ready;
    void *palette;
    s32 paletteIndex;
    s32 index;
    s8 highlight;
    u16 previous;
    s32 rowPhase;
    s16 key;
    UiRow24 *acceptRow;
    UiRow24 *closingRow;
    UiRow24 *cancelRow;

    ModeFrame = ModeFrame + 1U;
    ScreenFrame = ScreenFrame + 1U;
    switch ((u16)SubmodeState) {
    case 0:
        if (ovl662_emblem_edit_from_menu == 0) SetMenuFrameOptions(1,1,1);
        else SetMenuFrameOptions(1,1,0);
        previous = ovl662_selected_emblem_slot;
        if (ovl662_emblem_preview_fade >= 0x11) {
            StepMenuValue(1, 2, (char *)&ovl662_selected_emblem_slot, 0x19, 0);
        }
        if ((s16) previous != (s16) ovl662_selected_emblem_slot) {
            key = (s8)ovl662_emblem_choice_map[(s16)ovl662_selected_emblem_slot];
            ovl662_selected_emblem_id = key;
            if (key == 0) {
                palette = CustomEmblemTimPtr;
                paletteIndex = 0;
            } else {
                palette = PresetEmblemTimPtr;
                paletteIndex = key - 1;
            }
            UploadCarClut(palette, paletteIndex, 0);
            ovl662_emblem_preview_fade = 0;
        }
        UiRows[(s16)MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, MenuItemCount, 0);
        highlight = 2;
        if (ovl662_selected_emblem_id >= 0x19) {
            UiRows->half20 = 1;
            if ((s16) MenuCursor == 0) {
                highlight = 4;
            }
        } else {
            UiRows->half20 = 0;
        }
        UiRows[(s16)MenuCursor].cells[0] = highlight;
        rowPhase = UiRows->byte14;
        if (rowPhase == 2) {
            badgePhase = BadgeRows->state10;
            if (badgePhase == 3) {
                if (PlayerControllers[0].action28 != 0) {
                    UiRows[(s16)MenuCursor].cells[0] = 1;
                    if ((s16) MenuCursor == 0) {
                        if (ovl662_selected_emblem_id < 0x19) {
                            PlaySfx(1);
                            index = 0;
                            ovl662_menu_substate = (s16) rowPhase;
                            ovl662_emblem_preview_fade = 0;
                            SubmodeState = (u16) badgePhase;
                            do {
                                acceptRow = (UiRow24 *)(index * sizeof(UiRow24) + (unsigned long)UiRows);
                                if (acceptRow->byte14 == 2) {
                                    acceptRow->byte14 = 3U;
                                }
                                index += 1;
                            } while (index < 2);
                            BadgeRows->state10 = 4;
                        } else {
                            PlaySfx(4);
                        }
                    } else if ((s16) MenuCursor == 1) {
                        PlaySfx(1);
                        index = 0;
                        ovl662_emblem_preview_fade = 0;
                        ovl662_menu_substate = (s16) rowPhase;
                        BadgeRows->state10 = 4;
                        do {
                            closingRow = (UiRow24 *)(index * sizeof(UiRow24) + (unsigned long)UiRows);
                            if (closingRow->byte14 == 2) {
                                closingRow->byte14 = 3U;
                            }
                            index += 1;
                        } while (index < 2);
                        goto closeMenu;
                    }
                } else if (PlayerControllers[0].action2C != 0) {
                    if (ovl662_emblem_edit_from_menu == 0) {
                        PlaySfx(2);
                        index = 0;
                        ovl662_menu_substate = (s16) rowPhase;
                        ovl662_emblem_preview_fade = 0;
                        BadgeRows->state10 = 4;
                        do {
                            cancelRow = (UiRow24 *)(index * sizeof(UiRow24) + (unsigned long)UiRows);
                            if (cancelRow->byte14 == 2) {
                                cancelRow->byte14 = 3U;
                            }
                            index += 1;
                        } while (index < 2);
closeMenu:
                        SubmodeState = 4;
                    }
                }
            }
        }
        goto drawMenu;
    case 3:
        if (ovl662_emblem_edit_from_menu == 0) SetMenuFrameOptions(0,1,1);
        else SetMenuFrameOptions(0,1,0);
        if ((UiRows->byte14 == 5) && (BadgeRows->state10 == 11)) {
            PlayerCourseChoices[0].second = (u16)ovl662_selected_emblem_id+1;
            if (ovl662_emblem_edit_from_menu == 0) goto enterPalette;
            goto publishScene;
        }
        goto drawMenu;
    case 4:
        if (ovl662_emblem_edit_from_menu == 0) SetMenuFrameOptions(0,1,1);
        else SetMenuFrameOptions(0,1,0);
        if ((UiRows->byte14 == 5) && (BadgeRows->state10 == 11)) {
            if (ovl662_emblem_edit_from_menu == 0) {
enterPalette:
                ovl662_init_garage_car_view();
            } else {
publishScene:
                ModeGroup=3;ModeIndex=1;SubmodeState=0;
            }
            break;
        }
        goto drawMenu;
    default:
drawMenu:
        if (ovl662_menu_substate == 0) {
            ready = UpdateRect16Row(0);
            if (ready == 1) {
                ovl662_menu_substate = ready;
            }
        } else {
            ovl662_draw_emblem_preview_fade(ovl662_menu_substate - 1);
            DrawMenuStepArrows(0xA0, 0xC0, 0x30, 0, (s16) ovl662_selected_emblem_slot, 0, ovl662_emblem_choice_count);
        }
        index = 0;
        if (MenuItemCount > 0) {
            do {
                DrawUiRow(index);
                index += 1;
            } while (index < MenuItemCount);
        }
        UpdateBadgeRowAnim(0);
        break;
    }
    UpdateScreenFade(0x1004A);
}
