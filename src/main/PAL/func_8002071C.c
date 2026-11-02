#include "rr/sprite_emit.h"
void func_80095B54(void *);
void func_80095AF4(void *,s32);
void func_80095A44(u32 *,u32 *);
RrPolyF3Packet *func_8002071C(u32 *ordering,RrPolyF3Packet *packet,s32 xy0,s32 xy1,s32 xy2,const u8 *rgb,s32 flags){
 func_80095B54(packet);
 func_80095AF4(packet,flags);
 packet->xy0=xy0;
 packet->xy1=xy1;
 packet->xy2=xy2;
 packet->r=rgb[0];
 packet->g=rgb[1];
 packet->b=rgb[2];
 func_80095A44(ordering,(u32*)packet++);
 return packet;
}
