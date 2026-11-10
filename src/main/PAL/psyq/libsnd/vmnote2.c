/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmnote2.h"
extern unsigned short SsSemitonePitchTable[];
extern unsigned short _svm_ptable[];

unsigned short SsPitchFromNote(int cen_note_high, int cen_note_low, int note_high, int note_low);

unsigned short note2pitch(void) { return SsPitchFromNote(_svm_cur.f02, 0, (unsigned char)_svm_cur.f10, (unsigned char)_svm_cur.f11); }

unsigned short note2pitch2(s32 raw_note, s32 raw_fine) {
    s16 note = raw_note;
    s16 fine = raw_fine;
    s16 toneIndex = _svm_cur.f07 * 16 + _svm_cur.f0C;
    return SsPitchFromNote(note, fine, _svm_tn[toneIndex][VAB_TONE_CENTER], _svm_tn[toneIndex][VAB_TONE_SHIFT]);
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

    fine = (short)((u32)(note_low & 0xFF) + (u32)cen_note_low);
    q = fine / 128;
    t = (u32)cen_note_high + (u32)q - (u32)(note_high & 0xFF);
    note = t;
    frac = fine - q * 128;
    if (frac < 0) {
        frac = frac + 0x80;
        t = (u32)t - 1U;
        note = t + frac / 128;
    }
    oct = note / 12;
    octave = oct - 2;
    scale = note % 12;
    if (scale < 0) {
        scale = scale + 12;
        octave = oct - 3;
    }
    pitch = ((s32)((u32)SsSemitonePitchTable[scale] * _svm_ptable[frac])) >> 16;
    if (octave >= 0) {
        pitch = 0x3FFF;
    } else {
        shift = -octave;
        pitch += 1U << (shift - 1);
        pitch = pitch >> shift;
    }
    return pitch;
}
