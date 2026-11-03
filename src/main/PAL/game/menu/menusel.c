#include "types.h"
#include "m2c_macros.h"
#include "rr/selection.h"

struct S { int a; short b; short c; };

struct E { int a; short b; short c; };

extern int func_8002AF8C();
extern int func_80032A14();
extern int func_80032DF0();
extern int func_8003D6F0();
extern s32 D_800F3AFC;
extern void *D_800FF68C;
extern struct S D_800F2A68[];
extern struct S *D_800F2DE8[];
extern short D_800AC59C;

void func_80031CE4(void);
void func_80031DA0(void);
void func_80031DA8(void);
void func_80031DB0();
void func_80031DEC(void);
void func_80031E3C(void);

void func_80031CE4(void) {
    void *temp_v0;

    if (M2C_FIELD(((M2C_FIELD(D_800FF68C, s16 *, 0x16) * 0x1C) + D_800F3AFC), s16 *, 0x10) == 3) {
        func_8002AF8C(1, 1, 0);
        func_80032DF0(D_800FF68C);
        temp_v0 = (M2C_FIELD(D_800FF68C, s16 *, 0x16) * 0x1C) + D_800F3AFC;
        func_80032A14(M2C_FIELD(temp_v0, s16 *, 8) + 0xA, M2C_FIELD(temp_v0, s16 *, 0xA) + 0x28, D_800FF68C);
    }
    func_8003D6F0(M2C_FIELD(D_800FF68C, s16 *, 0x16));
}

void func_80031DA0(void) {
}

void func_80031DA8(void) {
}

void func_80031DB0() {
    int i = 0;
    struct S **q = D_800F2DE8;
    struct S *p = D_800F2A68;
    do { p->a = 0; p->b = i; *q++ = p; i++; p++; } while (i < 8);
}

void func_80031DEC(void) {
    int i = 0;
    struct E *e;
    struct E **t;
    int n;
    if (D_800AC59C > 0) {
        n = D_800AC59C;
        t = D_800F2DE8;
        e = D_800F2A68;
        do {
            e->a = 0;
            e->b = i;
            *t = e;
            t++;
            i++;
            e++;
        } while (i < n);
    }
}

void func_80031E3C(void) {
    s32 a2 = 0;
    s32 a1;
    RrSelectionRow *a0;
    RrSelectionRow *v1;
    RrSelectionRows *b;
    unsigned char *page;
    const u8 *choices, *variants;
    const RrSelectionConfigView *config;
    u8 t;
    s32 x;
    unsigned char *stage;
    /* ASM_BARRIER: Preserve the indexed byte offset in the address temporary. */
    register u32 offset __asm__("$2");
    u32 attribute;
    page=(unsigned char *)0x800F0000U;
    /* ASM_BARRIER: Materialize the state page after the initial row index. */
    __asm__("" : "+r"(page) : "r"(a2));
    b=(RrSelectionRows *)(page+0x4D98);
    /* ASM_BARRIER: Retain the full frame pointer separately from its page. */
    __asm__("" : "+r"(b));
    stage=(unsigned char *)0x800A0000U;
    /* ASM_BARRIER: Expose the selection-table page before adding its signed offset. */
    __asm__("" : "+r"(stage));
    choices=stage-0xBDC;
    stage=(unsigned char *)0x800A0000U;
    /* ASM_BARRIER: Keep the separate variant-table page materialization. */
    __asm__("" : "+r"(stage));
    variants=stage-0xBE0;
    stage=(unsigned char *)0x800B0000U;
    /* ASM_BARRIER: Expose the configuration page before adding its signed offset. */
    __asm__("" : "+r"(stage));
    config=(const RrSelectionConfigView *)(stage-0x2B28);
    /* ASM_BARRIER: Form lookup bases before initializing the byte offset. */
    __asm__("" : : "r"(choices), "r"(variants), "r"(config) : "$5");
    a1=RR_FIELD_OFFSET(RrSelectionRows,rows);
    do {
        x = page[0x4D98];
        offset=x*8;
        offset=a2+offset;
        t=*(unsigned char *)((unsigned long)offset+(unsigned long)choices);
        a0 = (RrSelectionRow *)((unsigned long)a1+(unsigned long)b);
        a0->selection = t;
        if (t == page[0x4D98]) {
            a0->variant = b->variant;
        } else {
            a0->variant = *(unsigned char *)((unsigned long)a0->selection+(unsigned long)variants);
            /* ASM_BARRIER: Retain the original row-pointer recomputation after the variant store. */
            __asm__("" : "+r"(a0));
            a0 = (RrSelectionRow *)((unsigned long)a1+(unsigned long)b);
        }
        if (a0->selection != page[0x4D98]) {
            if (a0->variant == b->variant) {
                a0->variant = *(unsigned char *)((unsigned long)page[0x4D98]+(unsigned long)variants);
            }
        }
        v1 = (RrSelectionRow *)((unsigned long)a1+(unsigned long)b);
        /* ASM_BARRIER: Keep the current row and index before publishing its attribute. */
        __asm__("" : : "r"(v1), "r"(a2));
        attribute=config->parameter;
        /* ASM_BARRIER: Read the row attribute before advancing the loop index. */
        __asm__("" : "+r"(attribute) : "r"(a2));
        a2 += 1;
        v1->parameter = attribute;
        a1 += sizeof(RrSelectionRow);
    } while (a2 < 7);
}
