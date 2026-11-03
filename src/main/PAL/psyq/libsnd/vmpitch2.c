/* GCC_VERSION: 2.7.2 */
extern unsigned short D_800A5128[];
extern unsigned short D_800A5140[];

unsigned short func_80088670(int cen_note_high, int cen_note_low, int note_high, int note_low)
{
    int fine;
    short note;
    short frac;
    short oct;
    short scale;
    short octave;
    int q;
    int t;
    int pitch;
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
    pitch = (D_800A5128[scale] * D_800A5140[frac]) >> 16;
    if (octave >= 0) {
        pitch = 0x3FFF;
    } else {
        shift = -octave;
        pitch += 1 << (shift - 1);
        pitch = (unsigned)pitch >> shift;
    }
    return pitch;
}
