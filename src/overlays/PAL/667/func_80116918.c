#include "rr/overlay667_course_initializer.h"
typedef struct {s8 first,second;} Pair;
void ovl667_func_80116918(void){
 s32 i;s32 j;s32 offset;s32 pos;
 u8 *repeat;
 u8 *destBase;
 u8 *sourceBase;
 SetDispMask(0);
 SetupDrawDispEnvs(0,0,0,2);
 UpdateLinkHandshake(1);
 i=0;
 ClearScreenOverlayState();
 for(;i<2;i++) {LinkCarChoiceGroup[i]=0;LinkCarChoiceIndex[i]=0;LinkPlayerWinCounts[i]=0;}
 for(i=0;i<4;i++) {((MenuChoice4 *)((u8 *)&SelectionRows+i*4))->bytes[3]=0;LinkPlayerWinCounts[i]=0;}
 i=0;
 destBase=(u8 *)&LinkCarChoiceTable;
 sourceBase=(u8 *)&CourseChoiceTable;
 offset=0;
 CourseIndex=0;SelectedCourseChoice=0;NumLaps=3;
 ActivePlayerIndex=(1-(s16)LinkHostFlag)*2;
 for(;i<2;i++,offset+=36)for(j=0,repeat=sourceBase,pos=offset;j<9;repeat+=4,j++,pos+=4) {
  u8 *dest=((u8 *)(pos+(unsigned long)destBase));
  __builtin_memcpy(dest,(u8 *)(pos+(unsigned long)sourceBase),4);
  __builtin_memcpy(dest+72,repeat,4);
 }
 for(i=0;i<2;i++)CopyStringClearTail(LinkPlayerNames[ActivePlayerIndex+i],ovl667_D_80118E00[ActivePlayerIndex+i]);
 ovl667_func_801161A8((1-(s16)LinkHostFlag)*2);
 UnpackChoiceGlyph((s8*)((u8 *)&SelectionRows+ActivePlayerIndex*4),CourseChoiceTable[0][0].glyph);
 UnpackChoiceGlyph((s8*)((u8 *)&SelectionRows+4+ActivePlayerIndex*4),CourseChoiceTable[0][0].glyph);
 LinkAvailableCourseCount=8;SeqStartTrack(0);ModeGroup=1;ModeIndex=60;SubmodeState=0;D_8010C0B8=0;
 if(((Pair *)((s8 *)&LinkEntryState+ActivePlayerIndex))->second!=1)D_8010C0D4=0;else D_8010C0D4=2;
 SetMenuFrameOptions(0,1,1);
}
