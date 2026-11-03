/* RODATA 0x8001011c 0x20 */
#include "rr_asm.h"

struct B8 {
    unsigned char c[8];
};

typedef struct {
    char pad0[0x20];
    struct B8 unk20;
    char unk28[0x08];
    int unk30;
    int unk34;
    int w38;
    short s3C;
    short s3E;
    short s40;
    short s42;
    short s44;
    short s46;
    short s48;
    short s4A;
} S_8010BF70;

typedef struct {
    char *cur;
    char *ot;
} SPD;

#define GS ((SPD *)0x1F800000)
#define W(o) (*(int *)(GS->cur + (o)))
#define LNK(n, t) \
    W(0) = *(int *)(GS->ot + (t)); \
    *(char *)(GS->cur + 3) = (n); \
    *(int *)(GS->ot + (t)) = (int)GS->cur; \
    *(char *)(GS->ot + (t) + 3) = 0

extern struct B8 D_8001010C;
extern struct B8 D_80010114;
extern int D_8009E268;
extern short D_800AC5D0;
extern char *D_800AC64C;
extern char *D_800ACC18;
extern unsigned char D_800AD4EA;
extern short D_800F3A26;
extern int D_800F3A60;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern S_8010BF70 *D_8010BF70;
extern int D_8010BF74;

extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_80012A68();
extern int func_80013890();
extern int func_800197C8();
extern int func_80019C90();
extern int func_8001B754();
extern int func_8001B83C();
extern int func_8001BAF8();
extern int func_8002A344();
extern int func_8002AF8C();
extern int func_8003A198();
extern int func_80043608();
extern int func_800436E8();
extern int func_8006E1AC();
extern int func_80091844();
extern int func_80096454();

void func_80019FC8(void) {
    struct B8 sp10;
    int s1;

    func_8002A344(0x1000C);
    if (D_800AC5D0 != 0) {
        D_800AC5D0 += 4;
        if (D_800AC5D0 >= 0x100) {
            func_8003A198();
            D_800F4C6A = 1;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }

    switch (D_8010BF74) {
    case 0:
        if (func_8001292C(func_8001223C(0, D_800AD4EA + 1), D_800ACC18) == 0) {
            return;
        }
        D_8010BF74 = 1;
    case 1:
        if (func_80012A58() == 0) {
            return;
        }
        func_800436E8(D_800ACC18);
        D_8010BF74 = 2;
    case 2:
        if (func_8001292C(func_8001223C(0, 0x14), D_800ACC18) == 0) {
            return;
        }
        D_8010BF74 = 3;
    case 3:
        if (func_80012A58() == 0) {
            return;
        }
        func_80043608(D_800ACC18);
        D_8010BF74 = 4;
    case 4:
        if (func_8001292C(func_8001223C(0, D_800AD4EA + 0xA), D_800ACC18) == 0) {
            return;
        }
        D_8010BF74 = 5;
    case 5:
        s1 = func_80012A58();
        if (s1 == 0) {
            return;
        }
        func_80012A68(D_800AC64C = D_800ACC18);
        D_8010BF70 = (S_8010BF70 *)(D_800ACC18 += s1);
        D_800ACC18 = (char *)D_8010BF70 + 0x4C;
        func_80013890(3);
        D_8010BF70->unk20 = D_8001010C;
        func_80096454(D_8010BF70->unk28, 0, 0x10);
        {
            S_8010BF70 *p = D_8010BF70;
            D_8010BF74 = 6;
            p->s48 = 0x40;
            /* ASM_BARRIER: keep p in $v0 across D_8010BF74 = 6 and field stores */
            __asm__("" : "+r"(p));
            p->w38 = 0;
            p->s3C = 0;
            p->s3E = 0;
            p->s40 = 0;
            p->s42 = 0;
            p->s44 = 0;
            p->s46 = 0;
            p->s4A = 0x1000;
            p->s3C = 0;
        }
    case 6:
        if (D_8010BF70->s3C < 0x40) {
            if (D_8010BF70->s3C == 8) {
                D_8010BF70->s42 = 4;
            } else if (D_8010BF70->s3C == 0x20) {
                D_8010BF70->s44 = 4;
            }
            break;
        }
        func_8002AF8C(1, 1, 0);
        D_8010BF74 = 7;
    case 7:
        if (D_800AC5D0 == 0 && D_800F3A60 != 0) {
            D_800AC5D0 = 4;
            D_8010BF70->s44 = -4;
            D_8010BF70->s42 = -4;
        }
        break;
    }

    D_8010BF70->s3C++;
    *(short *)((char *)D_8010BF70 + 0x22) -= 4;
    func_8001B754(&D_8010BF70->s3E, &D_8010BF70->s42, 0x80);
    func_8001B754(&D_8010BF70->s40, &D_8010BF70->s44, 0x80);

    if (D_8010BF70->s4A != 0) {
        S_8010BF70 *p;
        /* ASM_BARRIER: keep interpolated IR2 result in $s2 */
        register int ir2 __asm__("$18");

        if (D_8010BF70->s4A / 32 != 0) {
            D_8010BF70->s4A -= D_8010BF70->s4A / 32;
        } else if (D_8010BF70->s4A >= 3) {
            D_8010BF70->s4A -= 2;
        } else {
            D_8010BF70->s4A = 0;
        }

        p = D_8010BF70;
        /* ASM_BARRIER: GTE INTPL setup and execution on p->s4A */
        __asm__ volatile(
            "lhu $8, 0x4A(%1)\n\t"
            "nop\n\t"
            "mtc2 $8, $8\n\t"
            "addiu $9, $0, 0x720\n\t"
            "mtc2 $9, $9\n\t"
            "addu $10, $0, $0\n\t"
            "ctc2 $10, $21\n\t"
            "addiu $11, $0, 0x90\n\t"
            "mtc2 $11, $10\n\t"
            "addiu $8, $0, 0xFF\n\t"
            "ctc2 $8, $22\n\t"
            "nop\n\t"
            "nop\n\t"
            ".word 0x4A980011\n\t"
            "addiu %1, %1, 0x30\n\t"
            "swc2 $9, 0(%1)\n\t"
            "mfc2 %0, $10"
            : "=r"(ir2), "+r"(p)
            :
            : "memory"
        );
        D_8009E268 = ir2;
    }

    func_8001BAF8(D_8010BF70->s3E, 0x25);
    func_8001BAF8(D_8010BF70->s40, 0x26);

    W(4) = 0xE1000229;
    LNK(1, 4);
    GS->cur += 8;
    func_8001B83C(&D_8010BF70->s46, &D_8010BF70->s48);

    if (D_8010BF70->s46 != 0) {
        /* ASM_BARRIER: Keep &sp10 in $a2 before unaligned struct copy */
        register struct B8 *a2 asm("$6");
        func_80019C90(D_8010BF70->s46);
        func_8006E1AC();
        func_80091844(&D_8010BF70->unk20, D_8010BF70);
        /* ASM_BARRIER: Emit 8-byte unaligned struct copy using $8, $9, $10 without spilling $8 at earlier block move */
        __asm__(
            "addiu $6, $29, 0x10\n\t"
            "lui $2, %%hi(D_80010114)\n\t"
            "addiu $8, $2, %%lo(D_80010114)\n\t"
            "lwl $9, 3($8)\n\t"
            "lwr $9, 0($8)\n\t"
            "lwl $10, 7($8)\n\t"
            "lwr $10, 4($8)\n\t"
            "swl $9, 0x13($29)\n\t"
            "swr $9, 0x10($29)\n\t"
            "swl $10, 0x17($29)\n\t"
            "swr $10, 0x14($29)"
            : "=r"(a2), "=m"(sp10)
        );
        func_800197C8(D_8010BF70->unk28, D_8010BF70, a2, D_8009E268);
    }
    W(4) = 0xE6000001;
    LNK(1, 0xAFC);
    GS->cur += 8;
}
