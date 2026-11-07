/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

/* legacy typing of shared globals in this part of the unit */
#define I_STAT_Pad ((struct Q_1 *)I_STAT_Pad)

#define PadSio0Base ((struct R *)PadSio0Base)

#define chkRC2wait ((int (*)())chkRC2wait)

#undef I_STAT_Pad

#undef PadSio0Base

#undef chkRC2wait

struct HW { volatile unsigned char b0; char p1[3]; volatile unsigned short s4; short p6; short s8; unsigned short sA; char pC[2]; volatile short sE; };

struct Q { int a; };

extern int setRC2wait(), chkRC2wait();

extern struct HW *PadSio0Base;

extern struct Q *I_STAT_Pad;

int _padClrIntSio0() {
    I_STAT_Pad->a = -0x81;
    if (PadSio0Base->c & 0x80) {
        do {
            if (chkRC2wait()) return 0;
        } while (PadSio0Base->c & 0x80);
    }
    PadSio0Base->f |= 0x10;
    return 1;
}
