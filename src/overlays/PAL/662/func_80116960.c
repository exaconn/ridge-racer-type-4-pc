#include "rr/overlay662_available_cars.h"

void ovl662_func_80116960(void){
 u8 parts[3];
 s32 decoded;
 s32 arenaOffset;
 s32 i,count;s32 j;
 s16 *status;RrGarageAvailableCar *entry;
 u8 *choice;
 RrGarageAvailableCar *captured;
 D_800F2DE4=0;D_800ABEB0=0;ovl662_D_801272D4=0;ovl662_D_801272E4=0;D_800AC64A=0;D_800AC5E0=2;
 func_8003A198();
 arenaOffset=0x2D000;i=0;count=i;
 D_800F4D70.second=D_800102F0+arenaOffset;
 func_8003A1B4();D_800ACC18=D_800F4D70.second;
 func_8003E928(0,24,88,68,32,0,0,1,0,83);
 func_8003E928(1,24,128,68,32,0,0,1,0,85);
 func_80037110();func_8003D228(1,24,192,84,34,0,0x201);func_80013890(5);
 status=ovl662_D_80127300;entry=ovl662_D_80127348;
 do{
  D_800F4D98.bytes[2]=i;
  *status=(D_800F3920.prefix.progress.progress10[D_800F4D98.bytes[0]][D_800F4D98.bytes[1]]>>i)&1;
  if(*status!=0){
   entry->value=func_8003708C(&D_800F4D98);j=0;captured=entry;choice=(u8*)&D_800ABE08;
   do{
    func_80037040((s8*)parts,((RrCourseChoice4*)(choice+36))->glyph);
    if(parts[2]<39)parts[2]%=20;
    decoded=func_8003708C(parts);
    if(captured->value==(s16)decoded)*status=2;
    j++;choice+=4;
   }while(j<8);
   entry++;count++;
  }
  i++;status++;
 }while(i<20);
 ovl662_D_801272E0=count;
 func_80037040((s8*)&D_800F4D98,ovl662_D_80127348[0].value);
 D_800ACBD0[0].first=0;D_800ACBD0[0].second=0;
 D_800F4C6A=1;D_800F3A26=67;D_800F4D8A=0;
 D_800ACC18=D_800F4D40;D_800F4D70.first=D_800F4D40;
}
