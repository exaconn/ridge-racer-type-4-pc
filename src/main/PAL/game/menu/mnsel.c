#include "rr/menu_grid.h"
#include "game/menu/mnsel.h"
#include "rr/bios_devices.h"
#include "game/data/selection.h"

typedef struct { char p[0x10]; short s10; } T3;

extern int ResetScreenCounters();
extern int PlaySfx();
extern int D_8009FDA4;
extern char D_800AD4D8[];
extern short D_800AD520;
extern int D_800F2DE4;
extern short D_800F3A26;
extern unsigned short D_800F3A42;
extern int D_800F3A60;
extern T3 *D_800F3AFC;
extern short D_800F4C6A;
extern char *D_800F4D80;
extern short D_800F4D8A;
extern signed char *D_800F4D8C;
extern char *D_800FF68C;

int func_80041A18(int a, int n);
void func_80041A54(s32 index, s32 layout, s32 count, s32 x, s32 y);

void func_800417EC()
{
    int s0;
    char *q; char *r;
    int t = D_800F4D98[0];
    D_800AD520 = t;
    D_800F4D80[t + 0xC] = 0;
    if (D_800F3A42 & 0x2000) {
        if (D_800AD520 < 3) {
            D_800AD520 = (unsigned short)D_800AD520 + 1;
            D_8009FDA4 = 2;
            goto L_a8;
        }
    }
    if (D_800F3A42 & 0x8000) {
        if (D_800AD520 > 0) {
            D_800AD520 = (unsigned short)D_800AD520 - 1;
            D_8009FDA4 = 1;
            goto L_a8;
        }
    }
    D_8009FDA4 = 0;
    goto L_54;
L_a8:
    PlaySfx(0);
    if (D_800AD520 < 0) {
        D_800AD520 = 0;
    } else if (D_800AD520 >= 4) {
        D_800AD520 = 3;
    }
    D_800F4D98[0] = D_800AD520;
    D_800FF68C[4] = (unsigned char)D_800AD520 + 0x22;
    D_800F4D8C[D_800AD520 * 0x1C + 2] = -6;
    D_800F4D8C[D_800AD520 * 0x1C] = 0;
    D_800F4D80[0x16] = D_800AD520;
L_54:
    q = D_800F4D80 + 0xC;
    q[D_800AD520] = 2;
    if (D_800F3A60 != 0) {
        s0 = D_800F3AFC->s10;
        if (s0 == 3) {
            PlaySfx(1);
            D_800F2DE4 = 0;
            ResetScreenCounters();
            r = D_800AD4D8;
            *(short *)(r + 0x1A) = 1;
            r[0x1C] = D_800F4D98[0];
            D_800F3AFC->s10 = 4;
            D_800F4D80[0x14] = s0;
            D_800F4C6A = 1;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A28 (*(int (*))&D_800F3A28)

int func_80041A18(int a, int n) {
    int sz = n * 56;
    D_800F3A28 = a;
    func_80096434(a, sz);
    return sz;
}

#undef D_800F3A28

void func_80041A54(s32 index, s32 layout, s32 count, s32 x, s32 y) {
    register s32 six = 6;
    register s32 quotient = six / count;
    register s32 offset;
    register u8 *page;
    /* ASM_BARRIER: retain the captured record in V1 through the copy. */
    register RrMenuGrid38 *entry asm("$3");
    register RrMenuGrid38 *next;
    register RrMenuGrid38 *last;
    const RrGridRow8 *row, *table;
    register s32 step_y;
    s32 step_x;
    register s32 layout_offset;
    /* ASM_BARRIER: retain both captured packed words in T4/T5 until stores. */
    register u32 first asm("$12"), second asm("$13");
    /* PAL RAM page owns the full-width record-pointer slot at offset 0x3A28. */
    page = (u8 *)0x800F0000U;
    offset = index * 0x38;
    step_y = y;
    /* ASM_BARRIER: capture the full stack argument before its signed16 narrowing. */
    __asm__("" : "+r"(step_y));
    entry = (RrMenuGrid38 *)((unsigned long)offset + (unsigned long)(*(RrMenuGrid38 **)(page + 0x3A28)));
    entry->columns2E = quotient * 3;
    ((RrMenuGrid38 *)((unsigned long)offset + (unsigned long)(*(RrMenuGrid38 **)(page + 0x3A28))))->rows2F = six;
    entry = (RrMenuGrid38 *)((unsigned long)offset + (unsigned long)(*(RrMenuGrid38 **)(page + 0x3A28)));
    step_y = (s16)step_y / (s32)entry->rows2F;
    step_x = (s16)x / (s32)((u32)count * (u32)entry->columns2E);
    layout_offset = layout * 8;
    entry->count2C = count;
    entry->width04 = 64;
    entry->height06 = 8;
    table = D_8009FDA8;
    entry->step_y0A = step_y;
    /* ASM_BARRIER: reload the record-pointer slot after publishing the dimensions. */
    __asm__("" : : : "memory");
    next = (RrMenuGrid38 *)((unsigned long)offset + (unsigned long)(*(RrMenuGrid38 **)(page + 0x3A28)));
    row = (const RrGridRow8 *)((u8 *)table + layout_offset);
    entry->step_x08 = step_x;
    first = row->first;
    second = row->second;
    /* ASM_BARRIER: stage the captured first word before the packed destination writes. */
    __asm__("" : : "r"(first));
    entry->layout30.first = first;
    entry->layout30.second = second;
    ((u8 *)&next->layout30)[2] = (u32)((u8 *)&next->layout30)[2] / next->columns2E;
    last = (RrMenuGrid38 *)((unsigned long)offset + (unsigned long)(*(RrMenuGrid38 **)(page + 0x3A28)));
    ((u8 *)&last->layout30)[3] = (u32)((u8 *)&last->layout30)[3] / last->rows2F;
}
