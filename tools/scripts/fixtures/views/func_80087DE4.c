/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

typedef struct {
    unsigned char *c0;
    int c4, c8, cC;
    char p10[4];
    unsigned char b14;
    char p15[2];
    unsigned char b17;
    char p18[0x60 - 0x18];
    short h60[0x10];
    char p80[0xB0 - 0x80];
} Ch;

typedef struct {
    unsigned char f00, f01, f02, f03, f04, f05;
    char p06[0x10 - 6];
} Prog;

typedef struct {
    char p00[0x12];
    unsigned short h12;
} VabHdr;

typedef struct {
    unsigned char f00, f01, f02, f03, f04, f05;
    char p06[0x20 - 6];
} Tone;

typedef struct {
    signed char f00, f01, f02, f03, f04, f05, f06, f07, f08, f09, f0A, f0B, f0C, f0D, f0E, f0F, f10, f11, f12, f13;
    short f14, f16, f18;
} SvmCur;

typedef struct {
    short f00, f02, f04, f06, f08;
    signed char f0A, f0B;
    short f0C, f0E, f10, f12, f14, f16, f18, f1A;
    signed char f1C, f1D;
    char p1E[0x36 - 0x1E];
} SvmVoice;

extern Ch *_ss_score[];

extern Prog *_svm_pg;

extern VabHdr *_svm_vh;

extern Tone *_svm_tn;

extern signed char spuVmMaxVoice;

extern SvmCur _svm_cur;

extern SvmVoice _svm_voice[];

extern int _SsVmVSetUp();

extern int _SsVmKeyOff();

extern unsigned char _SsVmSelectToneAndVag();

extern unsigned char _SsVmAlloc();

extern void _SsVmDoAllocate();

extern void vmNoiseOn();

extern unsigned short note2pitch();

extern void _SsVmKeyOnNow();

int _SsVmKeyOn(short a0, short a1, short a2, short a3, int a4_in, int a5_in)
{
    unsigned char sp10[0x80];
    unsigned char sp90[0x80];
    unsigned char n;
    unsigned char i;
    short v;
    int ret;
    Ch *s5;
    Prog *prog;
    Tone *tone;
    Ch **tbl;
    int a4 = (unsigned short)a4_in;
    int a5 = (unsigned short)a5_in;

    ret = 0;
    tbl = _ss_score;
    s5 = tbl[a0 & 0xFF] + ((a0 & 0xFF00) >> 8);
    if (_SsVmVSetUp(a1, a2) != 0) {
        return -1;
    }
    _svm_cur.f14 = a0;
    _svm_cur.f02 = a3;
    _svm_cur.f03 = 0;
    if (a0 == 0x21) {
        _svm_cur.f04 = a4;
    } else {
        /* ASM_BARRIER: pins the product to $v0; with the identical arms (merged by
           post-reload cross-jumping) this keeps mult and div in separate flow blocks,
           so the highpart stays in HI and is reloaded via $t0 like the original */
        register int t asm("$2") = a4 * ((Ch *)((s5->b17 << 1) + (int)s5))->h60[0];
        if (t < 0) {
            _svm_cur.f04 = t / 127;
        } else {
            _svm_cur.f04 = t / 127;
        }
    }
    _svm_cur.f05 = a5;
    prog = (Prog *)(((int)a2 << 4) + (int)_svm_pg);
    _svm_cur.f0A = prog->f01;
    _svm_cur.f0B = prog->f04;
    _svm_cur.f00 = prog->f00;
    if (_svm_cur.f07 >= _svm_vh->h12) {
        return -1;
    }
    if (a4 == 0) {
        ret = _SsVmKeyOff(a0, a1, a2, (unsigned short)a3);
    } else {
        n = _SsVmSelectToneAndVag(sp90, sp10);
        for (i = 0; i < n; i++) {
            _svm_cur.f16 = sp10[i];
            _svm_cur.f0C = sp90[i];
            tone = (Tone *)(((int)(unsigned short)((_svm_cur.f07 * 16) + _svm_cur.f0C) << 5) + (int)_svm_tn);
            _svm_cur.f0F = tone->f00;
            _svm_cur.f0D = tone->f02;
            _svm_cur.f0E = tone->f03;
            _svm_cur.f10 = tone->f04;
            _svm_cur.f11 = tone->f05;
            _svm_cur.f12 = tone->f01;
            v = _SsVmAlloc(0);
            _svm_cur.f18 = v;
            if (v < spuVmMaxVoice) {
                int a0_ext = a0;
                _svm_voice[_svm_cur.f18].f1D = 1;
                _svm_voice[_svm_cur.f18].f02 = 0;
                _svm_voice[_svm_cur.f18].f10 = a0;
                _svm_voice[_svm_cur.f18].f18 = _svm_cur.f01;
                _svm_voice[_svm_cur.f18].f12 = _svm_cur.f07;
                _svm_voice[_svm_cur.f18].f14 = a2;
                if (a0_ext != 0x21) {
                    _svm_voice[_svm_cur.f18].f08 = a4;
                    _svm_voice[_svm_cur.f18].f0C = s5->b17;
                }
                _svm_voice[_svm_cur.f18].f0A = a5;
                _svm_voice[_svm_cur.f18].f16 = _svm_cur.f0C;
                _svm_voice[_svm_cur.f18].f0E = a3;
                _svm_voice[_svm_cur.f18].f1A = _svm_cur.f0F;
                _svm_voice[_svm_cur.f18].f00 = _svm_cur.f16;
                _SsVmDoAllocate();
                if (_svm_cur.f16 == 0xFF) {
                    vmNoiseOn((unsigned char)_svm_cur.f18);
                } else {
                    _SsVmKeyOnNow(n, note2pitch());
                }
                ret |= 1 << _svm_cur.f18;
            } else {
                ret = -1;
            }
        }
    }
    return ret;
}
