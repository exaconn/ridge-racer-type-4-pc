#include "rr/sprite_emit.h"

void SetSprt(void *);

void SetShadeTex(void *,s32);

void AddPrim(u32 *,u32 *);

RrSpritePacketSignedClut *GameQueueSprite(u32 *a0, RrSpritePacketSignedClut *a1, s32 a2, s32 a3, int a4, int a5, int a6, int a7, int a8) {
    SetSprt(a1);
    SetShadeTex(a1, 1);
    a1->x = a2;
    a1->y = a3;
    a1->w = a4;
    a1->h = a5;
    a1->u = a6;
    a1->v = a7;
    a1->clut = a8;
    AddPrim(a0, (u32*)a1++);
    return a1;
}
