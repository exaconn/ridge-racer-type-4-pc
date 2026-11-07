#define EP ((E_2 *)((a0 * 16) + Rect16Rows))

/* legacy typing of shared globals in this part of the unit */
#define Rect16Rows ((E_1 *)Rect16Rows)

#undef Rect16Rows

/* legacy typing of shared globals in this part of the unit */
#define GlyphRows ((char *)GlyphRows)

#undef GlyphRows

typedef struct {
    short p0, f2, f4, f6, f8, fA, fC, fE, f10, f12, f14, f16, f18, f1A;
} E;

extern int DrawBadgeRow(), rsin(), rcos();

extern E *BadgeRows;

extern int BadgeRowCount;

void UpdateBadgeRowAnim(a0) int a0; {
    int s2;
    if (a0 < BadgeRowCount) {
        s2 = BadgeRows[a0].f12;
        switch (BadgeRows[a0].f10) {
        case 0:
            BadgeRows[a0].f1A = 2;
            BadgeRows[a0].f10 = 0xA;
            BadgeRows[a0].f18 = 0;
            BadgeRows[a0].f12 = 0;
            BadgeRows[a0].f4 = BadgeRows[a0].f8;
            BadgeRows[a0].f6 = (BadgeRows[a0].fA + BadgeRows[a0].fE) - 2;
            BadgeRows[a0].f14 = BadgeRows[a0].f8;
            BadgeRows[a0].f16 = (BadgeRows[a0].fA + BadgeRows[a0].fE) - 2;
            break;
        case 1:
            BadgeRows[a0].f18 = ((BadgeRows[a0].fC + 2) * rsin((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                BadgeRows[a0].f12++;
            } else {
                BadgeRows[a0].f10 = 2;
                BadgeRows[a0].f12 = 0;
            }
            break;
        case 2:
            BadgeRows[a0].f1A = (BadgeRows[a0].fE * rsin((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].f16 = (BadgeRows[a0].fA + BadgeRows[a0].fE) - BadgeRows[a0].f1A;
            BadgeRows[a0].f6 = BadgeRows[a0].f16;
            if (s2 < 0xF) {
                BadgeRows[a0].f12++;
            } else {
                BadgeRows[a0].f10 = 3;
                BadgeRows[a0].f12 = 0;
            }
            break;
        case 3:
            BadgeRows[a0].f4 = BadgeRows[a0].f8;
            BadgeRows[a0].f6 = BadgeRows[a0].fA;
            BadgeRows[a0].f14 = BadgeRows[a0].f8;
            BadgeRows[a0].f16 = BadgeRows[a0].fA;
            BadgeRows[a0].f18 = BadgeRows[a0].fC + 2;
            BadgeRows[a0].f1A = BadgeRows[a0].fE;
            break;
        case 4:
            BadgeRows[a0].f1A = (BadgeRows[a0].fE * rcos((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].f16 = BadgeRows[a0].fA + (BadgeRows[a0].fE - BadgeRows[a0].f1A);
            BadgeRows[a0].f6 = BadgeRows[a0].f16;
            if (s2 < 0xF) {
                BadgeRows[a0].f12++;
            } else {
                BadgeRows[a0].f10 = 5;
                BadgeRows[a0].f12 = 0;
            }
            break;
        case 5:
            BadgeRows[a0].f18 = ((BadgeRows[a0].fC + 2) * rcos((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                BadgeRows[a0].f12++;
            } else {
                BadgeRows[a0].f10 = 0xB;
                BadgeRows[a0].f12 = 0;
            }
            break;
        case 10:
            if (BadgeRows[a0].f2 != 0) {
                BadgeRows[a0].f10 = 1;
            }
            break;
        case 7:
            BadgeRows[a0].f18 = ((BadgeRows[a0].fC + 2) * rsin((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].f1A = BadgeRows[a0].fE;
            BadgeRows[a0].f14 = BadgeRows[a0].f8;
            BadgeRows[a0].f16 = BadgeRows[a0].fA;
            if (s2 < 0xF) {
                BadgeRows[a0].f12++;
            } else {
                BadgeRows[a0].f10 = 3;
                BadgeRows[a0].f12 = 0;
            }
            break;
        case 6:
        case 9:
        case 11:
            break;
        case 8:
            BadgeRows[a0].f18 = BadgeRows[a0].fC + 2;
            BadgeRows[a0].f1A = (BadgeRows[a0].fE * rsin((s2 << 10) / 15)) / 4096;
            BadgeRows[a0].f14 = BadgeRows[a0].f8;
            BadgeRows[a0].f16 = BadgeRows[a0].fA + ((BadgeRows[a0].fE / 2) - (BadgeRows[a0].f1A / 2));
            if (s2 < 0xF) {
                BadgeRows[a0].f12++;
            } else {
                BadgeRows[a0].f10 = 3;
                BadgeRows[a0].f12 = 0;
            }
            break;
        }
        DrawBadgeRow(a0);
    }
}
