/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"

extern void (*D_800ABABC)();
extern char D_80011E64[];
extern struct { char pad[0x10]; int (*f)(); } *D_800ABAB8;
extern char D_80011E80[];
extern int func_800959B8();
extern M2C_UNK D_80011E94;
extern u8 D_800ABAC2;

int func_80092C58();
void func_80092CB8();
void func_80092D50(s32 arg0);

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

int func_80092C58(a0) int a0; {
 int o;
 if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011E64, a0);
 o = *(int *)(D_800ABAC2 + 0xA);
 *(int *)(D_800ABAC2 + 0xA) = a0;
 return o; }

void func_80092CB8(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) D_800ABABC(D_80011E80, a0);
 if (a0 == 0) func_800959B8(p + 0x6A, -1, 0x14);
 D_800ABAB8->f(a0 ? 0x3000000 : 0x3000001); }

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 (*(void *(*))&D_800ABAB8)
#define D_800ABABC (*(M2C_UNK (*(*))(M2C_UNK *, s32))&D_800ABABC)

void func_80092D50(s32 arg0) {
    if ((u8) D_800ABAC2 >= 2U) {
        D_800ABABC(&D_80011E94, arg0);
    }
    M2C_FIELD(D_800ABAB8, M2C_UNK (**)(s32), 0x3C)(arg0);
}

#undef D_800ABAB8
#undef D_800ABABC
