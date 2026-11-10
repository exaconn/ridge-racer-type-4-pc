#include "rr/overlay662_palette_initializer.h"
#include "rr/load_cursor.h"
void ovl662_init_emblem_selector(void) {
 s32 i;s16 val;BadgeRow1C *badge;
 ModeFrame=0;ScreenFrame=0;ovl662_menu_substate=0;ovl662_history_arrow_phase=0;MenuCursor=0;
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
