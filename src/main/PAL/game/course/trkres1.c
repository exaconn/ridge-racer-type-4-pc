#include "types.h"

#include "m2c_macros.h"

#include "rr/gpu.h"

#include "rr/steering.h"

#include "rr/track_resources.h"

extern int D_800ACC14;

extern int func_80096434();

extern s32 D_800ACB90;

void func_8004C928(int a0);

void *func_8004C998(char *a0);

void func_8004CBDC(char *p);

void func_8004CC1C(void *arg0, void *arg1, void *arg2, s32 arg3);

void func_8004CD88(unsigned short *a0, int a1, unsigned char *a2, unsigned char *a3, int a4);

void func_8004CFB0(void *arg0, void *arg1, void *arg2, s32 arg3);

void func_8004D11C(unsigned short *a0, unsigned char *a1, unsigned char *a2, unsigned char *a3, unsigned char *a4, int a5);

void func_8004D290(unsigned short *a0, unsigned char *a1, int a2, int a3);

void func_8004D354(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate);

s32 func_8004D420(const RrCurve24 *curve, s32 time, s32 *output);

int func_8004D4BC(int a);

void func_8004D4DC(RrLookupTables32 *tables);

void func_8004D58C(RrTrackLaneOffset8 *offsets);

void func_8004D598(s32 *startNode);

void func_8004D5A4(u8 *workspace);

void func_8004D5CC(RrTrackResourceFile *file);

void func_8004D61C(s32 arg0);

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

void func_8004CBDC(char *p) {
    func_80096434(p, 0x2C);
    *(int *)(p + 4) = 1;
    p[0x10] = 0x80;
    p[0x11] = 0x80;
    p[0x12] = 0x80;
}

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
