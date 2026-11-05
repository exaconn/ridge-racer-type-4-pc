#include "rr/sprite_emit.h"

/* legacy typing of shared globals in this part of the unit */
#define func_8001FCC8 (*(u8 *(*)(u32 *,u8 *,u16))&func_8001FCC8)

#undef func_8001FCC8

/* legacy typing of shared globals in this part of the unit */
#define AddPrim ((void (*)(u32 *, u32 *))AddPrim)

#undef AddPrim

/* legacy typing of shared globals in this part of the unit */
#define AddPrim ((int (*)())AddPrim)

#define func_80095B24 ((int (*)())func_80095B24)

#undef AddPrim

#undef func_80095B24

/* legacy typing of shared globals in this part of the unit */
#define AddPrim ((int (*)())AddPrim)

#define func_80095B24 ((int (*)())func_80095B24)

#undef AddPrim

#undef func_80095B24

/* legacy typing of shared globals in this part of the unit */
#define AddPrim ((void (*)(u32 *,u32 *))AddPrim)

#undef AddPrim

extern void func_80095B94(void *),func_80095AF4(void *,s32),AddPrim(u32 *,u32 *);

/* extern */
void func_80095AF4(void *, s32);

void func_80095B54(void *);

RrPolyF3Packet *func_8002071C(u32 *ordering,RrPolyF3Packet *packet,s32 xy0,s32 xy1,s32 xy2,const u8 *rgb,s32 flags){
 func_80095B54(packet);
 func_80095AF4(packet,flags);
 packet->xy0=xy0;
 packet->xy1=xy1;
 packet->xy2=xy2;
 packet->r=rgb[0];
 packet->g=rgb[1];
 packet->b=rgb[2];
 AddPrim(ordering,(u32*)packet++);
 return packet;
}
