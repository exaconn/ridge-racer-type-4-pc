/* RODATA 0x800120d8 0x14 */
/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "psyq/libpad/padenab.h"
#include "psyq/libpad/padopen.h"
#include "psyq/libpad/padpatch.h"
#include "psyq/libpad/padstat2.h"

struct E { unsigned char n; char pad[3]; unsigned char *p; };

struct S { char pad[8]; struct E *e; char pad2[0xde]; unsigned char c; };

extern int D_800ABCB4;

void func_80097834();
void func_80097854();
void func_80097874();
int func_80097894();
int func_800978E0();
int func_800979AC();
int func_80097AA4();
int func_80097B78();
void func_80097C20();
void func_80097C58(s32 selector, s32 first, s32 second);
void func_80097CA0();

void func_80097834() { func_80098030(); }

/* legacy typing of shared globals in this part of the unit */
#define func_80098044 ((int (*)())func_80098044)

void func_80097854() { func_80098044(); }

#undef func_80098044

void func_80097874() { func_80098110(); }

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC9C ((char *)D_800ABC9C)

int func_80097894(a0) int a0; {
    if (D_800ABCB4) {
        int i = a0 >> 4;
        return D_800ABC9C[i * 0xF0 + 0xE8] == 8;
    }
    return 0;
}

#undef D_800ABC9C

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(char *(*(*))())&D_800ABC7C)

int func_800978E0() {
  char *p = D_800ABC7C();
  int a;
  if (p[0x37] == 0 && p[0x38] == 0) {
    if (p == *(char **)(p + 0x10) || p[0x39] == 0) {
      if (**(char **)(p + 0x30) == 0) return p[0x49];
    }
  }
  a = p[0x49];
  switch (a) {
  case 3: return 1;
  case 2: return 1;
  case 6: return 4;
  }
  return p[0x49];
}

#undef D_800ABC7C

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(char *(*(*))())&D_800ABC7C)

int func_800979AC(a0, a1, a2) int a0, a1, a2; {
  char *p = D_800ABC7C();
  switch (a1) {
  case 1: return *(unsigned char *)(p + 0xE8);
  case 2: return *(unsigned short *)(p + 0xE6);
  case 3: return *(unsigned char *)(p + 0xE4);
  case 4:
    if (a2 < 0) return *(unsigned char *)(p + 0xE3);
    if (a2 < *(unsigned char *)(p + 0xE3)) return (*(unsigned short **)p)[a2];
    return 0;
  case 100: return *(int *)(p + 0x4C);
  }
  return 0;
}

#undef D_800ABC7C

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(unsigned char *(*(*))())&D_800ABC7C)

int func_80097AA4(a0, a1, a2) int a0, a1, a2; {
  unsigned char *p = D_800ABC7C();
  unsigned char *q;
  if (a1 < 0) return p[0xE9];
  if (a1 < p[0xE9]) {
    q = *(unsigned char **)(p + 4) + a1 * 5;
    switch (a2) {
    case 1: return q[0];
    case 2: return q[1];
    case 3: return q[2];
    case 4: return q[3];
    case 5: return q[4];
    }
  }
  return 0;
}

#undef D_800ABC7C

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(struct S *(*(*))())&D_800ABC7C)

int func_80097B78(a0, a1, a2) int a0, a1, a2; {
    struct S *s = D_800ABC7C();
    struct E *e;
    if (a1 < 0) return s->c;
    if (a1 >= s->c) return 0;
    e = s->e + a1;
    if (a2 < 0) return e->n;
    if (a2 >= e->n) return 0;
    return e->p[a2];
}

#undef D_800ABC7C

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(int (*(*))())&D_800ABC7C)

void func_80097C20(a0, a1) int a0, a1; { func_80099380(D_800ABC7C(), a1); }

#undef D_800ABC7C

void func_80097C58(s32 selector, s32 first, s32 second) {
    func_800994CC(D_800ABC7C(selector), first & 0xFF, second & 0xFF);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(int (*(*))())&D_800ABC7C)

void func_80097CA0(a0, a1, a2) int a0, a1, a2; { func_80098AC4(D_800ABC7C(), a1, a2); }

#undef D_800ABC7C
