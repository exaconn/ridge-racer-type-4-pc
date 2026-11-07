/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libsnd/ssopenq.h"
#include "psyq/libsnd/cc_0.h"
#include "psyq/libsnd/cc_6.h"
#include "psyq/libsnd/cc_91.h"
#include "psyq/libsnd/cc_912.h"
#include "psyq/libsnd/cc_nrpn.h"
#include "psyq/libsnd/cc_nrpn2.h"
#include "psyq/libsnd/cc_nrpn3.h"
#include "psyq/libsnd/cc_rpn.h"
#include "psyq/libsnd/cc_rpn2.h"
#include "psyq/libsnd/cc_rpn3.h"
#include "psyq/libsnd/cc_rpn4.h"
#include "psyq/libsnd/ccadsr.h"
#include "psyq/libsnd/ccadsr2.h"
#include "psyq/libsnd/ccadsr3.h"
#include "psyq/libsnd/ccadsr4.h"
#include "psyq/libsnd/midinote.h"
#include "psyq/libsnd/midipro2.h"
#include "psyq/libsnd/midiprog.h"
#include "psyq/libsnd/midiread.h"
#include "psyq/libsnd/seqinit.h"
#include "rr/audio_cd_controller.h"
#include "rr/library.h"
#include "rr/sequencer_init.h"
#include "rr/sequencer_tempo.h"
#include "psyq/libsnd/cc_98.h"
extern char D_800116E8[];
extern int D_8010EFC8[];
short SsSeqOpen(a0, a1) int a0; int a1; {
    /* ASM_BARRIER: pin t0 to match original register allocation */
    register int t0 asm("t0");
    /* ASM_BARRIER: pin v1 to match original register allocation */
    register int v1 asm("v1");
    /* ASM_BARRIER: pin a3 to match original register allocation */
    register int a3 asm("a3");
    int a;
    /* ASM_BARRIER: pin x to a1 to match original register allocation */
    register int x asm("a1");
    int bit;
    /* ASM_BARRIER: pin tab to v1 to match original register allocation */
    register int *tab asm("v1");
    /* ASM_BARRIER: pin s0 to match original register allocation */
    register int s0 asm("s0");
    /* ASM_BARRIER: pin v0 to match original register allocation */
    register int v0 asm("v0");
    /* ASM_BARRIER: pin t1 to match original register allocation */
    register int t1 asm("t1");
    /* ASM_BARRIER: pin a2 to match original register allocation */
    register int a2 asm("a2");
    t0 = 0;
    /* ASM_BARRIER: force t0 = 0 first */
    __asm__("" : "+r"(t0));
    a2 = a0;
    /* ASM_BARRIER: force a2 = a0 before _snd_openflag load */
    __asm__("" : "+r"(a2));
    x = a1;
    a3 = _snd_openflag;
    if (a3 == -1) {
    err:
        func_8001E84C(D_800116E8, x, a2, a3);
        return -1;
    } else {
        v1 = 0;
        /* ASM_BARRIER: prevent CSE from copying v1=0 into a */
        __asm__ volatile("" : "+r"(v1));
        a = 0;
        /* ASM_BARRIER: force addu a0, zero, zero */
        __asm__ volatile("" : "+r"(a));
        t1 = 1;
        /* ASM_BARRIER: prevent CSE from seeing t1 == 1 */
        __asm__ volatile("" : "+r"(t1));
        for (;;) {
            /* ASM_BARRIER: prevent stealing first loop insn into back-edge delay slot */
            __asm__ volatile("");
            bit = t1 << v1;
            if (!(a3 & bit)) {
                t0 = v1;
                a = 1;
                /* ASM_BARRIER: force addiu a0, zero, 1 */
                __asm__ volatile("" : "+r"(a));
            }
            v1++;
            if (v1 >= 0x20) goto err;
            /* ASM_BARRIER: force addu v0, a0, zero in delay slot */
            __asm__ volatile("" : "=r"(v0) : "0"(a));
            if (v0 != 0) {
                v0 = t0 << 16;
                /* ASM_BARRIER: force sll v0, t0, 16 */
                __asm__ volatile("" : "+r"(v0));
                s0 = v0 >> 16;
                /* ASM_BARRIER: force addu a0, s0, zero before 1 << s0 */
                __asm__ volatile("" : "=r"(a) : "0"(s0));
                /* ASM_BARRIER: force addiu v0, zero, 1 instead of reusing t1 */
                __asm__ volatile("" : "=r"(v0) : "0"(1));
                v0 <<= s0;
                /* ASM_BARRIER: force sllv v0, v0, s0 before sll a1, a1, 16 */
                __asm__ volatile("" : "+r"(v0));
                x <<= 16;
                /* ASM_BARRIER: force sll a1, a1, 16 before _snd_openflag load */
                __asm__ volatile("" : "+r"(x));
                v1 = _snd_openflag;
                /* ASM_BARRIER: load _snd_openflag into v1 */
                __asm__ volatile("" : "+r"(v1));
                v1 |= v0;
                /* ASM_BARRIER: force or v1, v1, v0 */
                __asm__ volatile("" : "+r"(v1));
                _snd_openflag = v1;
                x = _SsInitSoundSeq(a, x >> 16, a2, a3) << 16;
                /* ASM_BARRIER: keep x in a1 and reserve v0 across tab setup */
                __asm__ volatile("" : "+r"(x), "=r"(v0));
                tab = D_8010EFC8;
                /* ASM_BARRIER: force lui v1, addiu v1 */
                __asm__ volatile("" : "+r"(tab));
                tab[0] = (int)_SsNoteOn;
                tab[1] = (int)_SsSetProgramChange;
                tab[3] = (int)func_80081AE4;
                tab[2] = (int)_SsSetPitchBend;
                tab[4] = (int)_SsSetControlChange;
                tab[5] = (int)_SsContBankChange;
                tab[7] = (int)_SsContMainVol;
                tab[8] = (int)_SsContPanpot;
                tab[9] = (int)_SsContExpression;
                tab[10] = (int)_SsContDamper;
                tab[11] = (int)_SsContNrpn1;
                tab[12] = (int)_SsContNrpn2;
                tab[13] = (int)_SsContRpn1;
                tab[14] = (int)_SsContRpn2;
                tab[15] = (int)_SsContExternal;
                tab[16] = (int)_SsContResetAll;
                tab[6] = (int)_SsContDataEntry;
                tab[17] = (int)_SsSetNrpnVabAttr0;
                tab[18] = (int)_SsSetNrpnVabAttr1;
                tab[19] = (int)_SsSetNrpnVabAttr2;
                tab[20] = (int)_SsSetNrpnVabAttr3;
                tab[21] = (int)_SsSetNrpnVabAttr4;
                tab[22] = (int)_SsSetNrpnVabAttr5;
                tab[23] = (int)_SsSetNrpnVabAttr6;
                tab[24] = (int)_SsSetNrpnVabAttr7;
                tab[25] = (int)_SsSetNrpnVabAttr8;
                tab[26] = (int)_SsSetNrpnVabAttr9;
                tab[27] = (int)_SsSetNrpnVabAttr10;
                tab[28] = (int)_SsSetNrpnVabAttr11;
                tab[29] = (int)_SsSetNrpnVabAttr12;
                tab[30] = (int)_SsSetNrpnVabAttr13;
                tab[31] = (int)_SsSetNrpnVabAttr14;
                tab[32] = (int)_SsSetNrpnVabAttr15;
                tab[33] = (int)_SsSetNrpnVabAttr16;
                tab[34] = (int)_SsSetNrpnVabAttr17;
                tab[35] = (int)_SsSetNrpnVabAttr18;
                a = (int)_SsSetNrpnVabAttr19;
                /* ASM_BARRIER: materialize _SsSetNrpnVabAttr19 in a0 before shifting x */
                __asm__ volatile("" : "+r"(a));
                x >>= 16;
                /* ASM_BARRIER: shift x before final table store */
                __asm__ volatile("" : "+r"(x));
                tab[36] = a;
                /* ASM_BARRIER: end reservation of v0 */
                __asm__ volatile("" : : "r"(v0));
                v1 = -1;
                /* ASM_BARRIER: keep -1 in v1 for comparison */
                __asm__("" : "+r"(v1));
                if (x != v1) return s0;
                return -1;
            }
        }
    }
}
