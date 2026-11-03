#include "types.h"
#include "rr/track_lateral.h"
#include "rr/text_sprites.h"
#include "m2c_macros.h"
#include "rr/gpu.h"
#include "game/course/trktext.h"

typedef struct { char p0; unsigned char f1; char p2[6]; unsigned short f8; char pa[0x52]; } T;

typedef struct { short a[8], b[8], c[8]; } U;

extern T D_800F3A38[];
extern U D_800F2F78[];
extern s32 func_800924D4(s32 z, s32 x);
extern unsigned char D_800A017C[];
extern int func_80096434();

s32 func_8004C454(s32 node, s32 lane);
void func_8004C5A4(s32 x, s32 y, const u8 *text, s32 color, s32 bank);
int func_8004C73C(int a0, int a1, unsigned char *s, int a3, int a4);
int func_8004C7E8(int a0, int a1, unsigned char *s, int a3, int a4);
void func_8004C894(int a0, int a1, unsigned char *s, int a3, int a4);
void func_8004C928(int a0);
void *func_8004C998(char *a0);
void func_8004CBDC(char *p);
void func_8004CC1C(void *arg0, void *arg1, void *arg2, s32 arg3);
void func_8004CD88(unsigned short *a0, int a1, unsigned char *a2, unsigned char *a3, int a4);

int func_8004C370(int i) {
    T *t = &D_800F3A38[i];
    switch (t->f1) {
    case 0x41:
    case 0x73:
    default:
        return D_800F3A38[i].f8 & D_800F2F78[i].a[6];
    case 0x23:
        return t->f8 & D_800F2F78[i].b[6];
    case 0xE3:
        return t->f8 & D_800F2F78[i].c[6];
    }
}

s32 func_8004C454(s32 node, s32 lane)
{
    int delta[6];
    int previous;
    RrTrackLaneOffset8 *currentOffset, *previousOffset;
    int previousOffsetBytes, currentOffsetBytes;
    RrTrackSample3C *previousSample, *currentSample;
    previous = (node - 1) % D_800ABECC;
    if (previous < 0) previous += D_800ABECC;
    switch (lane) {
    case 0:
        currentOffset = D_800ACB9C + node;
        previousOffset = D_800ACB9C + previous;
        previousOffsetBytes = previous * (s32)sizeof(RrTrackSample3C);
        currentOffsetBytes = node * (s32)sizeof(RrTrackSample3C);
        previousSample = (RrTrackSample3C *)(previousOffsetBytes + (unsigned long)D_800ABEA4);
        currentSample = (RrTrackSample3C *)(currentOffsetBytes + (unsigned long)D_800ABEA4);
        delta[2] = previousSample->x08 + previousOffset->x - (currentSample->x08 + currentOffset->x);
        delta[3] = previousSample->z0C + previousOffset->z - (currentSample->z0C + currentOffset->z);
        break;
    case 10:
        delta[2] = D_800ABEA4[previous].x08 - D_800ABEA4[node].x08;
        delta[3] = D_800ABEA4[previous].z0C - D_800ABEA4[node].z0C;
        break;
    }
    return (s16)func_800924D4(delta[3], delta[2]);
}

void func_8004C5A4(s32 x, s32 y, const u8 *text, s32 color, s32 bank) {
    register RrSpritePacket *packet;
    register s32 current_x;
    register s32 glyph;
    s32 width;
    register const u8 *width_entry;
    RrIconSpriteContext *context;
    u32 *head;

    /* ASM_BARRIER: retain the biased packet-field cursor in T0. */
    register u8 *fields asm("$8");
    register s32 row;
    s32 v;
    /* ASM_BARRIER: retain Y in S1 while A1 becomes glyph and width address. */
    register s32 yy asm("$17");
    /* ASM_BARRIER: retain the ordering-head index in S0 across the loop. */
    register s32 selected asm("$16");
    /* ASM_BARRIER: retain glyph U in T2 until the coordinate stores. */
    register s32 u asm("$10");
    const u8 *map, *widths;
    s32 missing, command;
    u32 low, high;
    const u8 *lookup;
    packet = (RrSpritePacket *)((RrGpuScratchPointers *)0x1F800000U)->cursor;
    current_x = x;
    yy = y;
    selected = bank;
    if (*text != 0) {
        map = D_800A015C + 32;
        missing = 255;
        command = 0x66;
        widths = D_800A01DC;
        low = 0xFFFFFF;
        high = 0xFF000000;
        fields = (u8 *)packet + 14;
        do {
            lookup = (const u8 *)((unsigned long)*text + (unsigned long)map);
            glyph = lookup[-32];
            text++;
            if (glyph != missing) {
                fields[-11] = 4;
                fields[-7] = 0x64;
                row = (s32)glyph / 21;
                {
                    s32 remainder = (s32)glyph - row * 21;
                    /* ASM_BARRIER: finish the remainder-times-three value in V1. */
                    register s32 triple asm("$3") = remainder * 3;
                    /* ASM_BARRIER: expose the unscaled triple before the final shift. */
                    __asm__("" : : "r"(triple));
                    u = triple * 4;
                }
                v = row * 12 + 188;
                if (color == missing) {
                    fields[-7] = 0x65;
                } else {
                    fields[-7] = command;
                    fields[-10] = color;
                    fields[-9] = color;
                    fields[-8] = color;
                }
                width_entry = (const u8 *)((unsigned long)glyph + (unsigned long)widths);
                *(s16 *)(fields - 6) = current_x;
                *(s16 *)(fields - 4) = yy;
                width = *width_entry;
                *(s16 *)(fields + 4) = 12;
                *(s16 *)fields = 0x7F78;
                fields[-2] = u;
                fields[-1] = v;
                context = D_800ACC14;
                head = (u32 *)((unsigned long)(selected * 4) + (unsigned long)context + 0x70);
                *(s16 *)(fields + 2) = width;
                fields += 20;
                *(u32 *)packet = (*(u32 *)packet & high) | (*head & low);
                *head = (*head & high) | ((u32)(unsigned long)packet & low);
                packet++;
                current_x = (s32)((u32)current_x + *width_entry);
            } else {
                current_x = (s32)((u32)current_x + 4U);
            }
        } while (*text != 0);
    }
    ((RrGpuScratchPointers *)0x1F800000U)->cursor = (u8 *)packet;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A01DC (*(unsigned char (*)[])&D_800A01DC)

int func_8004C73C(int a0, int a1, unsigned char *s, int a3, int a4) {
    int sum = 0;
    unsigned char *s0 = s;
    if (*s0 != 0) {
        do {
            unsigned char c = D_800A017C[*s - 0x20];
            s++;
            if (c != 0xFF) {
                sum += D_800A01DC[c];
            } else {
                sum += 4;
            }
        } while (*s != 0);
    }
    func_8004C5A4(0xA0 - sum / 2, a1, s0, a3, a4);
    return sum;
}

#undef D_800A01DC

/* legacy typing of shared globals in this part of the unit */
#define D_800A01DC (*(unsigned char (*)[])&D_800A01DC)

int func_8004C7E8(int a0, int a1, unsigned char *s, int a3, int a4) {
    int sum = 0;
    unsigned char *s0 = s;
    if (*s0 != 0) {
        do {
            unsigned char c = D_800A017C[*s - 0x20];
            s++;
            if (c != 0xFF) {
                sum += D_800A01DC[c];
            } else {
                sum += 4;
            }
        } while (*s != 0);
    }
    func_8004C5A4(a0 + (0xA0 - sum / 2), a1, s0, a3, a4);
    return sum;
}

#undef D_800A01DC

/* legacy typing of shared globals in this part of the unit */
#define D_800A01DC (*(unsigned char (*)[])&D_800A01DC)

void func_8004C894(int a0, int a1, unsigned char *s, int a3, int a4) {
    int sum = 0;
    unsigned char *s0 = s;
    if (*s0 != 0) {
        do {
            unsigned char c = D_800A017C[*s - 0x20];
            s++;
            if (c != 0xFF) {
                sum += D_800A01DC[c];
            } else {
                sum += 4;
            }
        } while (*s != 0);
    }
    func_8004C5A4(a0 - sum, a1, s0, a3, a4);
}

#undef D_800A01DC

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(int (*))&D_800ACC14)

void func_8004C928(int a0) {
    register int *g asm("$8") = (int *)0x1F800000;
    int lo;
    int *p = (int *)g[0];
    int *q;
    int t;
    ((char *)p)[3] = 1;
    p[1] = 0xE100023E;
    q = (int *)(a0 * 4 + D_800ACC14);
    lo = 0xFFFFFF;
    t = q[0x70/4];
    p[0] = (p[0] & 0xFF000000) | (t & lo);
    q[0x70/4] = (q[0x70/4] & 0xFF000000) | ((int)p & lo);
    g[0] = (int)(p + 2);
}

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(int (*))&D_800ACC14)

void *func_8004C998(char *a0)
{
  unsigned short *new_var;
  char *new_var2;
  int new_var3;
  char *p = *((char **) 0x1F800000);
  unsigned short a1;
  int v;
  char *d;
  int dd;
  *((unsigned char *) (((char *) p) + 3)) = 9;
  *((unsigned char *) (((char *) p) + 7)) = 0x2C;
  v = *((unsigned int *) (((char *) a0) + 8));
  if (v == 0)
  {
    v = 0x2C;
  }
  else
  {
    v = 0x2E;
  }
  *((unsigned char *) (((char *) p) + 7)) = v;
  if ((*((unsigned int *) (((char *) a0) + 4))) != 0)
  {
    v = (*((volatile unsigned char *) (((char *) p) + 7))) | 1;
  }
  else
  {
    v = (*((volatile unsigned char *) (((char *) p) + 7))) & 0xFE;
  }
  *((unsigned char *) (((char *) p) + 7)) = v;
  *((unsigned char *) (((char *) p) + 4)) = *((unsigned char *) (((char *) a0) + 0x10));
  *((unsigned char *) (((char *) p) + 5)) = *((unsigned char *) (((char *) a0) + 0x11));
  *((unsigned char *) (((char *) p) + 6)) = *((unsigned char *) (((char *) a0) + 0x12));
  *((unsigned short *) (((char *) p) + 8)) = *((unsigned short *) (((char *) a0) + 0x14));
  new_var2 = (char *) a0;
  new_var3 = 1;
  *((unsigned short *) (((char *) p) + 0xA)) = *((unsigned short *) (((char *) a0) + 0x16));
  *((unsigned short *) (((char *) p) + 0x10)) = ((*((unsigned short *) (new_var2 + 0x14))) + new_var3) + (*((unsigned short *) (((char *) a0) + 0x18)));
  *((unsigned short *) (((char *) p) + 0x12)) = *((unsigned short *) (((char *) a0) + 0x16));
  *((unsigned short *) (((char *) p) + 0x18)) = *((unsigned short *) (((char *) a0) + 0x14));
  a1 = 1;
  *((unsigned short *) (((char *) p) + 0x1A)) = ((*((unsigned short *) (((char *) a0) + 0x16))) + a1) + (*((unsigned short *) (((char *) a0) + 0x1A)));
  *((unsigned short *) (((char *) p) + 0x20)) = (((short) (*((unsigned short *) (((char *) a0) + 0x14)))) + 1) + (*((unsigned short *) (((char *) a0) + 0x18)));
  new_var = &(*((unsigned short *) (((char *) a0) + 0x16)));
  *((unsigned short *) (((char *) p) + 0x22)) = ((*new_var) + a1) + (*((unsigned short *) (((char *) a0) + 0x1A)));
  *((unsigned char *) (((char *) p) + 0xC)) = *((unsigned char *) (((char *) a0) + 0x1C));
  *((unsigned char *) (((char *) p) + 0xD)) = *((unsigned char *) (((char *) a0) + 0x1E));
  *((unsigned char *) (((char *) p) + 0x14)) = (*((unsigned char *) (((char *) a0) + 0x1C))) + (*((unsigned char *) (((char *) a0) + 0x20)));
  *((unsigned char *) (((char *) p) + 0x15)) = *((unsigned char *) (((char *) a0) + 0x1E));
  *((unsigned char *) (((char *) p) + 0x1C)) = *((unsigned char *) (((char *) a0) + 0x1C));
  *((unsigned char *) (((char *) p) + 0x1D)) = (*((unsigned char *) (((char *) a0) + 0x1E))) + (*((unsigned char *) (((char *) a0) + 0x22)));
  *((unsigned char *) (((char *) p) + 0x24)) = (*((unsigned char *) (((char *) a0) + 0x1C))) + (*((unsigned char *) (((char *) a0) + 0x20)));
  *((unsigned char *) (((char *) p) + 0x25)) = (*((unsigned char *) (((char *) a0) + 0x1E))) + (*((unsigned char *) (((char *) a0) + 0x22)));
  a1 = *((unsigned short *) (((char *) a0) + 0x26));
  *((unsigned short *) (((char *) p) + 0x16)) = (((((*((unsigned short *) (((char *) a0) + 0xC))) & 3) << 7) | (((a1 & 0x100) << 16) >> 20)) | (((*((unsigned short *) (((char *) a0) + 0x24))) & 0x3FF) >> 6)) | ((a1 & 0x200) * 4);
  *((unsigned short *) (((char *) p) + 0xE)) = ((*((unsigned short *) (((char *) a0) + 0x2A))) << 6) | (((*((unsigned short *) (((char *) a0) + 0x28))) >> 4) & 0x3F);
  dd = D_800ACC14;
  *((unsigned int *) (((char *) p) + 0)) = ((*((unsigned int *) (((char *) p) + 0))) & 0xFF000000) | ((*((unsigned int *) (((char *) ((char *) (((*((unsigned int *) (((char *) a0) + 0))) * 4) + dd))) + 0x70))) & 0xFFFFFF);
  d = (char *) (((*((unsigned int *) (((char *) a0) + 0))) * 4) + dd);
  *((unsigned int *) (((char *) d) + 0x70)) = ((*((unsigned int *) (((char *) d) + 0x70))) & 0xFF000000) | (((unsigned int) p) & 0xFFFFFF);
  p += 0x28;
  *((char **) 0x1F800000) = p;
  return p;
}

#undef D_800ACC14

void func_8004CBDC(char *p) {
    func_80096434(p, 0x2C);
    *(int *)(p + 4) = 1;
    p[0x10] = 0x80;
    p[0x11] = 0x80;
    p[0x12] = 0x80;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(s32 (*))&D_800ACC14)

void func_8004CC1C(void *arg0, void *arg1, void *arg2, s32 arg3) {
    void *temp_a3;
    void *temp_t0;

    temp_t0 = *(void **)0x1F800000;
    M2C_FIELD(temp_t0, s8 *, 3) = 8;
    M2C_FIELD(temp_t0, s8 *, 7) = 0x38;
    M2C_FIELD(temp_t0, u16 *, 8) = (u16) M2C_FIELD(arg0, u16 *, 0);
    M2C_FIELD(temp_t0, u16 *, 0xA) = (u16) M2C_FIELD(arg0, u16 *, 2);
    M2C_FIELD(temp_t0, s16 *, 0x10) = (s16) (M2C_FIELD(arg0, u16 *, 0) + M2C_FIELD(arg0, u16 *, 4));
    M2C_FIELD(temp_t0, u16 *, 0x12) = (u16) M2C_FIELD(arg0, u16 *, 2);
    M2C_FIELD(temp_t0, u16 *, 0x18) = (u16) M2C_FIELD(arg0, u16 *, 0);
    M2C_FIELD(temp_t0, s16 *, 0x1A) = (s16) (M2C_FIELD(arg0, u16 *, 2) + M2C_FIELD(arg0, u16 *, 6));
    M2C_FIELD(temp_t0, s16 *, 0x20) = (s16) (M2C_FIELD(arg0, u16 *, 0) + M2C_FIELD(arg0, u16 *, 4));
    M2C_FIELD(temp_t0, s16 *, 0x22) = (s16) (M2C_FIELD(arg0, u16 *, 2) + M2C_FIELD(arg0, u16 *, 6));
    M2C_FIELD(temp_t0, u8 *, 4) = (u8) M2C_FIELD(arg1, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 5) = (u8) M2C_FIELD(arg1, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 6) = (u8) M2C_FIELD(arg1, u8 *, 2);
    M2C_FIELD(temp_t0, u8 *, 0xC) = (u8) M2C_FIELD(arg2, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 0xD) = (u8) M2C_FIELD(arg2, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 0xE) = (u8) M2C_FIELD(arg2, u8 *, 2);
    M2C_FIELD(temp_t0, u8 *, 0x14) = (u8) M2C_FIELD(arg1, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 0x15) = (u8) M2C_FIELD(arg1, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 0x16) = (u8) M2C_FIELD(arg1, u8 *, 2);
    M2C_FIELD(temp_t0, u8 *, 0x1C) = (u8) M2C_FIELD(arg2, u8 *, 0);
    M2C_FIELD(temp_t0, u8 *, 0x1D) = (u8) M2C_FIELD(arg2, u8 *, 1);
    M2C_FIELD(temp_t0, u8 *, 0x1E) = (u8) M2C_FIELD(arg2, u8 *, 2);
    temp_a3 = (arg3 * 4) + D_800ACC14;
    M2C_FIELD(temp_t0, s32 *, 0) = (s32) ((M2C_FIELD(temp_t0, s32 *, 0) & 0xFF000000) | (M2C_FIELD(temp_a3, s32 *, 0x70) & 0xFFFFFF));
    M2C_FIELD(temp_a3, s32 *, 0x70) = (s32) ((M2C_FIELD(temp_a3, s32 *, 0x70) & 0xFF000000) | ((s32) temp_t0 & 0xFFFFFF));
    *(void **)0x1F800000 = temp_t0 + 0x24;
}

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(int (*))&D_800ACC14)

void func_8004CD88(unsigned short *a0, int a1, unsigned char *a2, unsigned char *a3, int a4) {
    RrPolyG4Packet *p = *(RrPolyG4Packet **)0x1F800000;
    int *a;
    p->len = 8;
    p->code = 0x3A;
    if (a1 == 0) {
        p->x0 = a0[0];
        p->y0 = a0[1];
        p->x1 = a0[0] + a0[2];
        p->y1 = a0[1] + (short)a0[3] / 2;
        p->x2 = a0[0];
        p->y2 = a0[1] + a0[3];
        p->x3 = a0[0] + a0[2];
        p->y3 = a0[1] + a0[3];
    } else {
        p->x0 = a0[0];
        p->y0 = a0[1];
        p->x1 = a0[0] + a0[2];
        p->y1 = a0[1];
        p->x2 = a0[0];
        p->y2 = a0[1] + a0[3];
        p->x3 = a0[0] + a0[2];
        p->y3 = a0[1] + (short)a0[3] / 2;
    }
    p->r0 = a2[0];
    p->g0 = a2[1];
    p->b0 = a2[2];
    p->r1 = a2[0];
    p->g1 = a2[1];
    p->b1 = a2[2];
    p->r2 = a3[0];
    p->g2 = a3[1];
    p->b2 = a3[2];
    p->r3 = a3[0];
    p->g3 = a3[1];
    p->b3 = a3[2];
    a = (int *)(a4 * 4 + D_800ACC14);
    *(int *)p = (*(int *)p & 0xFF000000) | (a[0x70 / 4] & 0xFFFFFF);
    *(volatile int *)&a[0x70 / 4] = (a[0x70 / 4] & 0xFF000000) | ((int)p & 0xFFFFFF);
    p++;
    *(RrPolyG4Packet **)0x1F800000 = p;
}

#undef D_800ACC14
