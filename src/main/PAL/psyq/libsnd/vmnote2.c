/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmnote2.h"
extern unsigned short SsSemitonePitchTable[];
extern unsigned short _svm_ptable[];

unsigned short SsPitchFromNote(int cen_note_high, int cen_note_low, int note_high, int note_low);

unsigned short note2pitch(void) { return SsPitchFromNote(_svm_cur.f02, 0, (unsigned char)_svm_cur.f10, (unsigned char)_svm_cur.f11); }

unsigned short note2pitch2(a0, a1) short a0; short a1; {
    short i = _svm_cur.f07 * 16 + _svm_cur.f0C;
    char *t = (char *)(i * 32 + (int)_svm_tn);
    return SsPitchFromNote(a0, a1, t[4], t[5]);
}

unsigned short SsPitchFromNote(int cen_note_high, int cen_note_low, int note_high, int note_low)
{
    int fine;
    short note;
    short frac;
    short oct;
    short scale;
    short octave;
    int q;
    int t;
    u32 pitch;
    int shift;

    fine = (short)((note_low & 0xFF) + cen_note_low);
    q = fine / 128;
    t = cen_note_high + q - (note_high & 0xFF);
    note = t;
    frac = fine - (q << 7);
    if (frac < 0) {
        frac = frac + 0x80;
        t--;
        note = t + frac / 128;
    }
    oct = note / 12;
    octave = oct - 2;
    scale = note % 12;
    if (scale < 0) {
        scale = scale + 12;
        octave = oct - 3;
    }
    pitch = (SsSemitonePitchTable[scale] * _svm_ptable[frac]) >> 16;
    if (octave >= 0) {
        pitch = 0x3FFF;
    } else {
        shift = -octave;
        pitch += 1 << (shift - 1);
        pitch = pitch >> shift;
    }
    return pitch;
}
