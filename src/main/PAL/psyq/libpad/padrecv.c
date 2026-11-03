/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr_asm.h"

typedef struct {
    char pad0[0x3C];
    unsigned char *p3C;
    unsigned char *p40;
    unsigned char b44;
    unsigned char b45;
} S;

typedef struct {
    volatile unsigned char b0;
    char pad1[3];
    volatile unsigned short h4;
    char pad6[4];
    volatile unsigned short hA;
    char padC[2];
    volatile unsigned short hE;
} H;

typedef struct {
    volatile int a;
} Q;

extern void func_8009A7F4(s32);
extern s32 func_80098A00(void);
extern int func_8009A814();
extern Q *D_800ABCC8;
extern H *D_800ABCCC;
extern int D_801146B8, D_801146BC;

void func_80098488(RrChannelRecordF0 *channel);
int func_80098578(S *a0, int a1);

void func_80098488(RrChannelRecordF0 *channel) {
    s32 *stage=&D_800ABCAC;
    RrChannelStageCallback callback=D_800ABCEC[*stage];
    s32 result,value;
    *stage+=1;
    result=callback(channel);
    if(result<0)goto error;
    value=D_800ABCAC;
    if(value) {
        if(value==3 && channel->status.packet[0]==0x80)goto reload;
        func_8009A7F4(60);
        if(!func_80098A00())D_800ABC68(-3);
reload:
        value=D_800ABCAC;
        /* ASM_BARRIER: Keep the reload gap before the shared saturation label. */
        RR_NOP_DEP_ON(value);
    }
    if(value>=5) {
        s32 decremented=value-1;
        /* ASM_BARRIER: Form the decremented stage before constructing its PAL page. */
        __asm__("" : "+r"(decremented) : : "$1");
        {
            /* ASM_BARRIER: Retain the real PAL stage page in the assembler temporary. */
            register s32 *page __asm__("$1")=(s32 *)0x800B0000U;
            /* ASM_BARRIER: Expose the stage store for the final jump's delay slot. */
            __asm__("" : "+r"(page));
            page[-0x4354/4]=decremented;
        }
    }
    goto out;
error:
    D_800ABC68(result);
out:;
}

int func_80098578(S *a0, int a1) {
    /* ASM_BARRIER: pin s1 to $s1 so unpinned s2 gets $s2 */
    register int s1 __asm__("$17");
    int s2;

    if (a1 < 0) {
        unsigned char tmp = D_800ABCCC->b0;
        /* ASM_BARRIER: pin ~a1 to $v0 so nor $v0, $zero, $s3 fills beqz delay slot */
        register int v0 __asm__("$2");
        a0->b44 = 0xFF;
        a0->b45 = 1;
        *a0->p40 = ~a1;
        s2 = tmp;
        while (!(D_800ABCCC->h4 & 1))
            ;
        while (func_8009A814() == 0)
            ;
        v0 = ~a1;
        /* ASM_BARRIER: compute ~a1 before loading D_800ABCCC so nor fills beqz delay slot */
        __asm__ volatile("" : "+r"(v0));
        *(unsigned char *)&D_800ABCCC->b0 = v0;
        /* ASM_BARRIER: keep return value copy in the jump delay slot */
        __asm__ volatile("" : : : "memory");
        return s2;
    }
    {
        /* ASM_BARRIER: pin shifted byte to $v0 */
        register int v0 __asm__("$2") = *a0->p3C;
        /* ASM_BARRIER: pin constant 8 to $v1 */
        register int v1 __asm__("$3");
        /* ASM_BARRIER: emit lbu $v0, 0($v0) before addiu $v1, $zero, 8 */
        __asm__("" : "+r"(v0), "=r"(v1));
        v1 = 8;
        /* ASM_BARRIER: place addiu $v1, $zero, 8 in load delay slot of lbu $v0, 0($v0) and keep s1 = 0x88 after it */
        __asm__("" : "+r"(v0), "+r"(v1), "=r"(s1));
        v0 >>= 4;
        s1 = 0x88;
        if (v0 == v1) {
            if (a0->b44 >= 9) {
                s1 = 0x22;
            }
        }
    }
    D_801146BC = 0x1AE;
    D_801146B8 = *(volatile unsigned short *)0x1F801120;
    while (!(D_800ABCCC->h4 & 2))
        ;
    s2 = D_800ABCCC->b0;
    D_800ABCCC->hE = s1;
    while (!(D_800ABCC8->a & 0x80)) {
        if (func_8009A814() != 0) {
            return -0x14;
        }
    }
    D_800ABCCC->b0 = a1;
    if (s1 == 0x22) {
        D_800ABCC8->a = -0x81;
        D_800ABCCC->hA |= 0x10;
    }
    a0->b45++;
    a0->p3C[a0->b44] = s2;
    a0->b44++;
    /* ASM_BARRIER: keep return value copy after b44 store */
    __asm__ volatile("" : : : "memory");
    return s2;
}
