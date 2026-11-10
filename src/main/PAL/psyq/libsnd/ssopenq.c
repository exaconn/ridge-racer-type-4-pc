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
extern char SsSeqOpenFailMsg[];
extern void *SsFCALL[];
short SsSeqOpen(a0, a1) int a0; int a1; {
    int t0;
    int v1;
    int a3;
    int a;
    int x;
    int bit;
    /* ASM_BARRIER: pin tab to v1 to match original register allocation */
    register void **tab asm("$3");
    int s0;
    /* ASM_BARRIER: pin v0 to match original register allocation */
    register int v0 asm("$2");
    int t1;
    int a2;
    t0 = 0;
    a2 = a0;
    x = a1;
    a3 = _snd_openflag;
    if (a3 == -1) {
    err:
        DebugPrintStub(SsSeqOpenFailMsg, x, a2, a3);
        return -1;
    } else {
        v1 = 0;
        /* ASM_BARRIER: prevent CSE from copying v1=0 into a */
        __asm__ volatile("" : "+r"(v1));
        a = 0;
        t1 = 1;
        for (;;) {
            bit = t1 << v1;
            if (!(a3 & bit)) {
                t0 = v1;
                a = 1;
            }
            v1++;
            if (v1 >= 0x20) goto err;
            /* ASM_BARRIER: force addu v0, a0, zero in delay slot */
            __asm__ volatile("" : "=r"(v0) : "0"(a));
            if (v0 != 0) {
                v0 = t0 << 16;
                s0 = v0 >> 16;
                /* ASM_BARRIER: force addu a0, s0, zero before 1 << s0 */
                __asm__ volatile("" : "=r"(a) : "0"(s0));
                v0 = 1;
                v0 <<= s0;
                x <<= 16;
                v1 = _snd_openflag;
                v1 |= v0;
                _snd_openflag = v1;
                x = _SsInitSoundSeq(a, x >> 16, a2, a3) << 16;
                tab = SsFCALL;
                tab[0] = _SsNoteOn;
                tab[1] = _SsSetProgramChange;
                tab[3] = _SsGetMetaEvent;
                tab[2] = _SsSetPitchBend;
                tab[4] = _SsSetControlChange;
                tab[5] = _SsContBankChange;
                tab[7] = _SsContMainVol;
                tab[8] = _SsContPanpot;
                tab[9] = _SsContExpression;
                tab[10] = _SsContDamper;
                tab[11] = _SsContNrpn1;
                tab[12] = _SsContNrpn2;
                tab[13] = _SsContRpn1;
                tab[14] = _SsContRpn2;
                tab[15] = _SsContExternal;
                tab[16] = _SsContResetAll;
                tab[6] = _SsContDataEntry;
                tab[17] = _SsSetNrpnVabAttr0;
                tab[18] = _SsSetNrpnVabAttr1;
                tab[19] = _SsSetNrpnVabAttr2;
                tab[20] = _SsSetNrpnVabAttr3;
                tab[21] = _SsSetNrpnVabAttr4;
                tab[22] = _SsSetNrpnVabAttr5;
                tab[23] = _SsSetNrpnVabAttr6;
                tab[24] = _SsSetNrpnVabAttr7;
                tab[25] = _SsSetNrpnVabAttr8;
                tab[26] = _SsSetNrpnVabAttr9;
                tab[27] = _SsSetNrpnVabAttr10;
                tab[28] = _SsSetNrpnVabAttr11;
                tab[29] = _SsSetNrpnVabAttr12;
                tab[30] = _SsSetNrpnVabAttr13;
                tab[31] = _SsSetNrpnVabAttr14;
                tab[32] = _SsSetNrpnVabAttr15;
                tab[33] = _SsSetNrpnVabAttr16;
                tab[34] = _SsSetNrpnVabAttr17;
                tab[35] = _SsSetNrpnVabAttr18;
                {
                    void *fn = _SsSetNrpnVabAttr19;
                    /* ASM_BARRIER: materialize _SsSetNrpnVabAttr19 in a0 before shifting x */
                    __asm__ volatile("" : "+r"(fn));
                    x >>= 16;
                    tab[36] = fn;
                }
                /* ASM_BARRIER: end reservation of v0 */
                __asm__ volatile("" : : "r"(v0));
                v1 = -1;
                if (x != v1) return s0;
                return -1;
            }
        }
    }
}
