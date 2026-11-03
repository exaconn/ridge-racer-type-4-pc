/* RODATA 0x80011200 0x1c */
#include "types.h"
#include "rr/line_emit.h"
#include "rr/glyph_list.h"
#include "rr/gpu.h"
#include "rr/icon_sprites.h"

#define ADDPRIM(ot, p) \
    (p)->addr = ((SPRT *)(ot))->addr; \
    ((SPRT *)(ot))->addr = (u32)(p)
#define SCRATCH ((Scratch *)0x1F800000)
/* The two original entries differ only in the sprite's vertical position. */
#define RR_ICON_SPRITE_BODY(Y) \
    RrSpritePacket *p; \
    RrGpuScratchPointers *scratch; \
    RrIconSpriteContext *context; \
    u32 low=0xFFFFFFU; \
    /* ASM_BARRIER: reuse the consumed color argument for the tag length mask. */ \
    register u32 high __asm__("$5"); \
    u32 u; \
    u16 clut; \
    u32 offset; \
    /* ASM_BARRIER: retain the sprite length until its first byte store. */ \
    register s32 length __asm__("$2"); \
    u32 command; \
    /* ASM_BARRIER: retain the context-slot page across both packet insertions. */ \
    register u8 *context_page __asm__("$8"); \
    /* ASM_BARRIER: finish the low-address mask before forming packet arguments. */ \
    __asm__("" : "+r"(low), "+r"(index)); \
    command=0xE1000000U; \
    /* ASM_BARRIER: stage the command page before the scratch base and length. */ \
    __asm__("" : "+r"(command) : : "$9", "$2"); \
    scratch=(RrGpuScratchPointers*)0x1F800000U; \
    length=4; \
    /* ASM_BARRIER: prepare the scratch capture and length before indexing. */ \
    __asm__("" : "+r"(scratch), "+r"(length)); \
    offset=(u32)index*2U; \
    u=(offset+(u32)index)*8U+112U; \
    p=(RrSpritePacket*)scratch->cursor; \
    /* ASM_BARRIER: finish UV/cursor capture before the context-slot page. */ \
    __asm__("" : "+r"(u), "+r"(p)); \
    context_page=(u8*)0x800B0000U; \
    /* ASM_BARRIER: materialize the context-slot page before packet fields. */ \
    __asm__("" : "+r"(context_page)); \
    ((u8*)p)[3]=length; \
    p->code=0x66; \
    p->x=148; \
    p->y=Y; \
    p->w=24; \
    p->h=16; \
    clut=*(u16*)((u8*)D_8001121C+offset); \
    p->r=color; \
    p->g=color; \
    p->b=color; \
    p->u=u; \
    p->v=172; \
    context=*(RrIconSpriteContext**)(context_page-0x33EC); \
    high=0xFF000000U; \
    p->clut=clut; \
    *(u32*)p=(*(u32*)p&high)|(context->head70&low); \
    context->head70=(context->head70&high)|((u32)(unsigned long)p&low); \
    p++; \
    scratch->cursor=(u8*)p; \
    ((u8*)p)[3]=1; \
    context=*(RrIconSpriteContext**)(context_page-0x33EC); \
    command|=0x23EU; \
    *((u32*)p+1)=command; \
    *(u32*)p=(*(u32*)p&high)|(context->head70&low); \
    context->head70=(context->head70&high)|((u32)(unsigned long)p&low); \
    scratch->cursor=(u8*)p+sizeof(RrGpuSingleCommandPacket);

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    s16 clut;
    s16 w, h;
} SPRT;

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u32 code[2];
} DR_MODE;

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u32 code[1];
} DR_MODE_1;

typedef struct {
    s32 a, b, c, d;
    s32 e[11];
} S3C;

typedef struct {
    u8 u, v, w, h;
} UvRect;

typedef struct {
    void *p;
} Scratch;

extern void func_80093C24();
extern s32 D_800A101C;
extern UvRect D_800A1020[];
extern UvRect D_800A1028;
extern UvRect D_800A102C;
extern s16 D_800A1030[];
extern s16 D_800A1040[];
extern struct { u16 x, y; } D_800A1050[];
extern s32 D_800A1070[];
extern S3C *D_800ABEA4;
extern s32 D_800ABECC;
extern s16 D_800AC648;
extern s16 D_800F3914;
extern void func_800528A8();
extern int func_80096464();
extern short D_800AD520;
extern short D_800F4D44;
extern int func_80052800();
extern signed char D_800AD4D8[];
extern int D_800A0C9C[];
extern int D_800F3970;
extern int D_800F765C;
extern int D_800F7660;
extern void func_8004F9C8(void);
extern s16 D_800A1818[];
extern s16 D_800A1824[];
extern s16 *D_800A18F0[];
extern s16 *D_800A1910[];
extern s16 *D_800A1930[];
extern s16 *D_800A1950[];
extern s16 *D_800A1970[];
extern s16 *D_800A1984[];
extern s16 *D_800A19A4[];
extern s16 *D_800A19C0[];
extern s32 D_800A19E0[];
extern s32 D_800A1A00[];
extern s32 D_800A1A20[];
extern s32 D_800A1A40[];
extern struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} D_800F4D98[];
extern struct {
    char pad[0x28A4];
    s32 w28a4;
    s32 w28a8;
    s32 w28ac;
} D_800F4DB8;
extern s32 D_8010E220;
extern s32 D_8010E228;
extern s32 D_8010E22C;
extern s32 D_8010E230;
extern s32 D_8010E234;
extern s32 D_8010E238;

void func_800587E4(s32 arg0, u8 *arg1, s32 *arg2);
void func_80058A3C(s32 *arg0);
void func_800590DC(void);
void func_80059168();
void func_80059284(short *a0, int a1);
void func_80059314(s32 arg0, s32 arg1);
void func_80059700(s16 *stream,s32 color,s32 bank);
void func_80059904();
void func_80059B1C(unsigned char *a0x, int a1, int a2, int a3);
void func_80059C28(s32 index,s32 color);
void func_80059D34(s32 index,s32 color);

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(u8 *(*))&D_800ACC14)
#define D_800A1158 (*(char (*)[])&D_800A1158)
#define D_800A1090 (*(u8 (*)[])&D_800A1090)

void func_800587E4(s32 arg0, u8 *arg1, s32 *arg2) {
    SPRT *sprt;
    s32 i, j;
    s32 x0, y0;
    s32 base_idx;
    u32 idx;
    u8 val;
    
    base_idx = ((0x100 - (arg2[2] + 0x200) / 256) << 8) + (0x100 - (arg2[0] + 0x200) / 256);
    sprt = *(SPRT **)0x1F800000;
    for (i = 0; i < 24; i++) {
        x0 = (i + 24) * 4 + 0x6C;
        y0 = (i - 24) * 4 + 0xD0;
        for (j = 0; j < 24; j++) {
            val = 0;
            idx = base_idx + (i - 12) * 256 + (j - 12);
            if (idx < 0x10000) val = arg1[idx];
            sprt->len = 4; sprt->code = 0x64; sprt->w = 4; sprt->h = 8;
            if (val == 0) {
                idx = 1;
                sprt->u0 = 0x20; sprt->v0 = 0xE8;
                sprt->x0 = x0;
                sprt->y0 = y0;
                sprt->code = (sprt->code & ~2) | 1;
                sprt->clut = 0x7E24;
            } else {
                idx = 0;
                sprt->r0 = 0xE4; sprt->g0 = 0x40; sprt->b0 = 0x18;
                sprt->y0 = y0 - 0x18;
                sprt->u0 = 0x20; sprt->v0 = 0xE8;
                sprt->x0 = x0;
                sprt->code = (sprt->code & ~2) | 1;
                sprt->clut = 0x7F64;
            }
            ADDPRIM(D_800ACC14 + idx * 4 + 0x98, sprt);
            sprt++;
            x0 -= 4;
            y0 += 4;
        }
    }
    func_80093C24(sprt, 0, 1, 0x19, 0);
    (sprt)->addr = ((SPRT *)(D_800ACC14 + 0x9C))->addr;
    ((SPRT *)(D_800ACC14 + 0x9C))->addr = (u32)(sprt);
    ((struct { void *p; } *)0x1F800000)->p = (DR_MODE *)sprt + 1;
    func_80058A3C(arg2);
}

void func_80058A3C(s32 *arg0) {
    SVECTOR sp10;
    SVECTOR sp18;
    s16 sp20[4];
    u8 sp28[4];
    TILE *tile;
    SPRT *sprt;
    DR_MODE_1 *drmode;
    s32 i;
    s16 *s1;
    UvRect *s3;
    s32 s2;
    S3C *base;
    S3C *sec;

    tile = SCRATCH->p;
    sp10.vx = D_800A1050[D_800F3914].x;
    sp10.vy = D_800A1050[D_800F3914].y;

    for (i = 0; i < D_800ABECC; i++) {
        sp18.vx = D_800ABEA4[i].c / 384;
        sp18.vy = -D_800ABEA4[i].d / 192;
        sp18.vx += sp10.vx;
        sp18.vy += sp10.vy;
        tile->len = 3;
        tile->code = 0x62;
        tile->x0 = sp18.vx - 1;
        tile->y0 = sp18.vy - 3;
        tile->w = 3;
        tile->h = 6;
        tile->r0 = 0xFF;
        tile->g0 = 0xDE;
        tile->b0 = 0x7F;
        ADDPRIM(D_800ACC14 + 0x84, tile);
        tile++;
    }
    SCRATCH->p = tile;

    if (D_800AC648 < 8) {
        s1 = D_800A1030;
        s3 = &D_800A1028;
    } else {
        s1 = D_800A1040;
        s3 = &D_800A102C;
    }

    base = D_800ABEA4;
    sec = (S3C *)(D_800A1070[D_800F3914] * 0x3C + (s32)base);
    sp18.vx = sec->c / 384;
    sp18.vy = -sec->d / 192;
    sp18.vx += sp10.vx;
    sp18.vy += sp10.vy;

    s2 = sp20[0] = s1[0] + sp18.vx;
    sp20[1] = s1[1] + sp18.vy;
    sp20[2] = s1[2] + sp18.vx;
    sp20[3] = s1[3] + sp18.vy;
    sp28[0] = 0xFF;
    sp28[1] = 0xDE;
    sp28[2] = 0x7F;

    for (i = 0; i < 4; i++) {
        sp20[2] = sp20[0] = s2 + (i & 1);
        func_8004D290(sp20, sp28, 1, 5);
    }

    sprt = SCRATCH->p;
    sprt->len = 4;
    sprt->code = 0x64;
    sprt->u0 = D_800A1020[1].u;
    sprt->v0 = D_800A1020[1].v;
    sprt->w = D_800A1020[1].w;
    sprt->h = D_800A1020[1].h;
    sprt->x0 = s1[4] + sp18.vx;
    sprt->y0 = s1[5] + sp18.vy;
    sprt->code |= 3;
    sprt->clut = 0x7FE5;
    ADDPRIM(D_800ACC14 + 0x84, sprt);
    sprt++;

    sprt->len = 4;
    sprt->code = 0x64;
    sprt->u0 = s3->u;
    sprt->v0 = s3->v;
    sprt->w = s3->w;
    sprt->h = s3->h;
    sprt->x0 = s1[6] + sp18.vx;
    sprt->y0 = s1[7] + sp18.vy;
    sprt->code |= 3;
    sprt->clut = 0x7FE5;
    ADDPRIM(D_800ACC14 + 0x84, sprt);
    sprt++;

    SCRATCH->p = sprt;
    drmode = (DR_MODE_1 *)sprt;
    drmode->len = 1;
    drmode->code[0] = 0xE1000219;
    ADDPRIM(D_800ACC14 + 0x84, drmode);
    drmode++;
    SCRATCH->p = drmode;

    sp18.vx = arg0[0] / 384;
    sp18.vy = -arg0[2] / 192;
    sp18.vx += sp10.vx;
    sp18.vy += sp10.vy;

    D_800A101C &= 0xFF;
    if (D_800A101C & 0x80) {
        D_800A101C &= 0x7F;
        if (D_800A101C < 0x7A) {
            D_800A101C = (D_800A101C + 6) | 0x80;
        } else {
            D_800A101C = 0x7F;
        }
    } else {
        D_800A101C &= 0x7F;
        if (D_800A101C >= 6) {
            D_800A101C -= 6;
        } else {
            D_800A101C = 0x80;
        }
    }

    sprt = SCRATCH->p;
    sprt->len = 4;
    sprt->code = 0x64;
    sprt->u0 = D_800A1020[0].u;
    sprt->v0 = D_800A1020[0].v;
    sprt->w = D_800A1020[0].w;
    sprt->h = D_800A1020[0].h;
    sprt->r0 = D_800A101C & 0x7F;
    sprt->g0 = D_800A101C & 0x7F;
    sprt->b0 = D_800A101C & 0x7F;
    sprt->x0 = sp18.vx - 4;
    sprt->y0 = sp18.vy - 8;
    sprt->code = (sprt->code | 2) & ~1;
    sprt->clut = 0x7FE5;
    ADDPRIM(D_800ACC14 + 0x80, sprt);
    sprt++;

    SCRATCH->p = sprt;
    drmode = (DR_MODE_1 *)sprt;
    drmode->len = 1;
    drmode->code[0] = 0xE1000239;
    ADDPRIM(D_800ACC14 + 0x80, drmode);
    drmode++;
    SCRATCH->p = drmode;
}

void func_800590DC(void) {
 int r, a1; if (D_800F4D44 == 1) { if (D_800F3914 == 7) { a1 = D_800AD520 + 0x20; func_800528A8(0, a1); return; } } r = func_80096464(); a1 = r % 6 + 0x1A; func_800528A8(0, a1);
}

void func_80059168() {
    int a0, a1, v;
    func_80052800();
    if (D_800F4D44 == 1) {
        a0 = D_800AD4D8[D_800AD4D8[0x17] + 8] < 3;
    } else {
        a0 = 1;
    }
    if (D_800F3970 != 0 && D_800F4D44 != 1) {
        D_800F7660 = 0x10;
    } else {
        D_800F7660 = 0xF;
    }
    if (a0 == 0) {
        a1 = D_800F3914 + 3;
        func_800528A8(0, a1);
    } else if (D_800F765C == 0) {
        a1 = func_80096464() % 14 + 0xB;
        func_800528A8(0, a1);
    } else {
        a1 = D_800A0C9C[D_800F765C - 1];
        func_800528A8(0, a1);
    }
}

void func_80059284(short *a0, int a1)
{
  unsigned short *p = (unsigned short *) (a0 + 2);
  int sum = 0;
  unsigned short v;
  char *e;
  if (a0[2] >= 0)
  {
    do
    {
      v = p[0];
      p++;
      if (((short) v) >= 0x400)
      {
        e = D_800A1158 + (((short) (v - 0x400)) << 2);
      }
      else
      {
        e = (char *) ((((short) v) * 4) + a1);
      }
      if (a1)
      {
        sum += e[2];
        sum += ((short *) p)[0];
        p++;
      }
      else
      {
        sum += e[2];
        sum += ((short *) p)[0];
        p++;
      }
    }
    while (((short *) p)[0] >= 0);
  }
  a0[0] = 0xA0 - (sum / 2);
}

void func_80059314(s32 arg0, s32 arg1) {
    s32 i;

    D_8010E22C = arg0;
    D_8010E230 = arg1;
    D_8010E228 = -1;
    D_8010E234 = 0;
    D_8010E238 = 0;
    func_8004F9C8();

    if (D_800F4D44 != 1) {
        D_800F765C = 0;
    } else {
        switch (D_800AD520) {
        case 0:
            D_800F765C = D_800A1A40[D_800F3914] + 1;
            break;
        case 1:
            D_800F765C = D_800A1A00[D_800F3914] + 1;
            break;
        case 2:
            D_800F765C = D_800A19E0[D_800F3914] + 1;
            break;
        case 3:
            D_800F765C = D_800A1A20[D_800F3914] + 1;
            break;
        }
    }

    switch (D_800F4D44) {
    case 0:
    case 2:
    case 3:
    case 6:
        D_800F4DB8.w28a4 = D_800F4DB8.w28ac;
        for (i = 0; i < 2; i++) {
            if (D_800F4D98[i].unk1 == 0 && D_800F4D98[i].unk0 == 2 && D_800F4D98[i].unk2 == 0x27) {
                D_800F4DB8.w28a4 = 0xF;
            }
        }
    case 1:
    case 5:
        if (D_800F3970 != 0 && D_800F4D44 != 1) {
            D_800F7660 = 0x10;
        } else {
            D_800F7660 = 0xF;
        }
        break;
    case 4:
    default:
        D_8010E234 = -1;
        D_800F4DB8.w28a4 = D_800A19E0[D_800F3914] + 1;
        D_800F4DB8.w28a8 = 0;
        break;
    }

    D_8010E220 = D_800F765C + 0xB;

    for (i = 0; i < 8; i++) {
        func_80059284(D_800A18F0[i], D_800A1090);
        func_80059284(D_800A1910[i], D_800A1090);
        func_80059284(D_800A1930[i], D_800A1090);
        func_80059284(D_800A1950[i], D_800A1090);
    }

    for (i = 0; i < 5; i++) {
        func_80059284(D_800A1970[i], D_800A1090);
    }

    for (i = 0; i < 8; i++) {
        func_80059284(D_800A1984[i], D_800A1158);
        func_80059284(D_800A1984[i] + 8, D_800A1158);
        func_80059284(D_800A1984[i] + 14, D_800A1158);
    }

    for (i = 0; i < 7; i++) {
        func_80059284(D_800A19A4[i], D_800A1090);
    }

    func_80059284(D_800A1818, D_800A1158);
    func_80059284(D_800A1824, D_800A1158);

    for (i = 0; i < 8; i++) {
        func_80059284(D_800A19C0[i], D_800A1158);
    }
}

#undef D_800ACC14
#undef D_800A1158
#undef D_800A1090

void func_80059700(s16 *stream,s32 color,s32 bank)
{
    s32 x, y, defaultPage, clut, page, offset, gap;
    u32 cachedGlyph;
    
    s32 basePage;
    s32 spriteCode;
    u8 *cursor;
    RrUiUvRect *uv;
    x = *stream++;
    y = *stream++;
    if (bank == 0) { clut = 0x7F78; defaultPage = 0x3E; } else { clut = 0x7E38; defaultPage = 0x25; }
    cachedGlyph = *(u16 *)stream;
    if (*stream >= 0) {
        basePage = defaultPage & 0xFFFF;
        do {
            s32 glyph;
            cursor = RR_GLYPH_SCRATCH->cursor;
            page = defaultPage;
            if (basePage == 0x3E) {
                if ((s16)cachedGlyph >= 0x400) page = 0x25;
            }
            cachedGlyph = *(u16 *)stream;
            glyph = *stream++;
            if (glyph >= 0x400) {
                offset = (s32)((cachedGlyph - 0x400U) << 16) >> 14;
                uv = (RrUiUvRect*)((u8 *)D_800A1158 + offset);
            } else {
                offset = glyph * 4;
                if (bank == 0) uv = (RrUiUvRect*)((u8 *)D_800A1090 + offset);
                else uv = (RrUiUvRect*)((u8 *)D_800A1158 + offset);
            }
            spriteCode = 0x66;
            ((RrSpritePacketSignedClut*)cursor)->len = 4;
            ((RrSpritePacketSignedClut*)cursor)->code = spriteCode;
            ((RrSpritePacketSignedClut*)cursor)->r = color; ((RrSpritePacketSignedClut*)cursor)->g = color; ((RrSpritePacketSignedClut*)cursor)->b = color;
            ((RrSpritePacketSignedClut*)cursor)->x = x;
            ((RrSpritePacketSignedClut*)cursor)->y = y;
            ((RrSpritePacketSignedClut*)cursor)->w = uv->w;
            ((RrSpritePacketSignedClut*)cursor)->h = uv->h;
            ((RrSpritePacketSignedClut*)cursor)->u = uv->u;
            ((RrSpritePacketSignedClut*)cursor)->v = uv->v;
            ((RrSpritePacketSignedClut*)cursor)->clut = clut;
            ((RrSpritePacketSignedClut*)cursor)->addr = (D_800ACC14->head70&0xFFFFFFU);
            D_800ACC14->head70 = (D_800ACC14->head70 & 0xFF000000) | ((u32)(unsigned long)cursor & 0xFFFFFF);
            cursor+=20;
            RR_GLYPH_SCRATCH->cursor=cursor;
            ((RrGlyphCommand8*)cursor)->len=1;
            ((RrGlyphCommand8*)cursor)->command=(page&0x9FF)|0xE1000200;
            ((RrGlyphCommand8*)cursor)->addr=D_800ACC14->head70;
            D_800ACC14->head70=(D_800ACC14->head70&0xFF000000U)|((u32)(unsigned long)cursor&0xFFFFFFU);
            RR_GLYPH_SCRATCH->cursor=cursor+8;
            gap = *stream++;
            x=(s32)((u32)x+(u32)(uv->w+gap));
            cachedGlyph = *(u16 *)stream;
        } while (*stream >= 0);
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A1090 (*(char (*)[])&D_800A1090)
#define D_800A1158 (*(char (*)[])&D_800A1158)
#define D_800ACC14 (*(char *(*))&D_800ACC14)

void func_80059904(t1, a1, a2)
s16 *t1;
s32 a1;
s32 a2;
{
    s32 t5, s4, t6 = 0, t7, t8, t3, v, w;
    u32 a;
    
    s32 m;
    s32 k;
    RrSpritePacketSignedClut *p;
    u8 *t0;
    t5 = *t1++;
    s4 = *t1++;
    if (a2 == 0) { t8 = 0x7F78; t7 = 0x3E; } else { t8 = 0x7E38; t7 = 0x25; }
    a = *(u16 *)t1;
    if (*t1 >= 0) {
        m = t7 & 0xFFFF;
        do {
            s32 e;
            p = *(RrSpritePacketSignedClut **)0x1F800000;
            t3 = t7;
            if (m == 0x3E) {
                if ((s16)a >= 0x400) t3 = 0x25;
            }
            a = *(u16 *)t1;
            e = *t1++;
            if (e >= 0x400) {
                v = (s32)((a - 0x400) << 16) >> 14;
                t0 = (u8 *)D_800A1158 + v;
            } else {
                v = e * 4;
                if (a2 == 0) t0 = (u8 *)D_800A1090 + v;
                else t0 = (u8 *)D_800A1158 + v;
            }
            k = 0x66;
            p->len = 4;
            p->code = k;
            p->r = a1; p->g = a1; p->b = a1;
            p->x = t5;
            p->y = s4 + t6;
            p->w = t0[2];
            p->h = t0[3];
            p->u = t0[0];
            p->v = t0[1];
            p->clut = t8;
            t6++;
            p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
            *(u32 *)(D_800ACC14 + 0x70) = (*(u32 *)(D_800ACC14 + 0x70) & 0xFF000000) | ((u32)p & 0xFFFFFF);
            p++;
            *(RrSpritePacketSignedClut **)0x1F800000 = p;
            p->len = 1;
            *((u32 *)p + 1) = (t3 & 0x9FF) | 0xE1000200;
            p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
            *(u32 *)(D_800ACC14 + 0x70) = (*(u32 *)(D_800ACC14 + 0x70) & 0xFF000000) | ((u32)p & 0xFFFFFF);
            *(RrSpritePacketSignedClut **)0x1F800000 = (RrSpritePacketSignedClut *)((u32 *)p + 2);
            w = *t1++;
            t5 += t0[2] + w;
            a = *(u16 *)t1;
        } while (*t1 >= 0);
    }
}

void func_80059B1C(unsigned char *a0x, int a1, int a2, int a3) {
    register unsigned char *p asm("$10");
    int *g;
    unsigned hi;
    unsigned cmd;
    register unsigned char *q asm("$2");
    int t;
    unsigned t3;
    register unsigned lo asm("$5");
    register int v1 asm("$3");
    register char *d asm("$4");
    p = *(unsigned char **)0x1F800000;
    p[3] = 4;
    p[7] = 0x64;
    if (a3 < 0xFF) {
        p[7] = 0x66;
    } else {
        p[7] = 0x65;
    }
    v1 = a1;
    lo = 0xFFFFFF;
    cmd = 0xE100023E;
    p[4] = a3;
    p[5] = a3;
    p[6] = a3;
    *(short *)(p + 8) = v1;
    *(short *)(p + 0xA) = a2;
    *(short *)(p + 0x10) = a0x[2];
    *(short *)(p + 0x12) = a0x[3];
    p[0xC] = a0x[0];
    t = a0x[1];
    *(short *)(p + 0xE) = 0x7F78;
    p[0xD] = t;
    hi = 0xFF000000;
    g = (int *)0x1F800000;
    d = D_800ACC14;
    *(unsigned int *)p = (*(unsigned int *)p & hi) | (*(unsigned int *)(d + 0x70) & lo);
    *(unsigned int *)(d + 0x70) = (*(unsigned int *)(d + 0x70) & hi) | ((unsigned int)p & lo);
    p += 0x14;
    *g = (int)p;
    p[3] = 1;
    d = D_800ACC14;
    t3 = *(unsigned int *)p;
    *(unsigned int *)(p + 4) = cmd;
    *(unsigned int *)p = (t3 & hi) | (*(unsigned int *)(d + 0x70) & lo);
    *(unsigned int *)(d + 0x70) = (*(unsigned int *)(d + 0x70) & hi) | ((unsigned int)p & lo);
    q = p + 8;
    *g = (int)q;
}

#undef D_800A1090
#undef D_800A1158
#undef D_800ACC14

void func_80059C28(s32 index,s32 color) {
    RR_ICON_SPRITE_BODY(120)
}

void func_80059D34(s32 index,s32 color) {
    RR_ICON_SPRITE_BODY(128)
}
