#ifndef RR_GAME_GFX_DRAWBUF_H
#define RR_GAME_GFX_DRAWBUF_H

#include "types.h"

/* The draw buffer CurrentDrawBuf points at: 0x70 bytes of environment, then the ordering table,
 * whose last entries are the list tails the camera code patches. Units that only do pointer
 * arithmetic on it use the byte pointer; units that read fields go through DRAWBUF. */
typedef struct {
    u32 addr : 24;
    u32 len : 8;
} DrawBufTag;

typedef struct {
    char pad00[0x70];
    DrawBufTag ot[(0xB68 - 0x70) / 4];
    DrawBufTag tailB68;
    DrawBufTag tailB6C;
    DrawBufTag tailB70;
} DrawBuf;

extern unsigned char *CurrentDrawBuf;

#define DRAWBUF ((DrawBuf *)CurrentDrawBuf)

#endif
