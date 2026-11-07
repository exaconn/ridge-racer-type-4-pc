/* legacy typing of shared globals in this part of the unit */
#define FormatDecimalDigits ((void (*)())FormatDecimalDigits)

#define UiFontGlyphs (*(R4_1 (*)[])&UiFontGlyphs)

#undef FormatDecimalDigits

#undef UiFontGlyphs

/* legacy typing of shared globals in this part of the unit */
#define FormatDecimalDigits ((void (*)())FormatDecimalDigits)

#undef FormatDecimalDigits

extern int FormatDecimalDigits();

extern char FractionText[];

char *FormatCountOfTotal(int a, int b, int c) {
    int i = 7;
    char *p = FractionText + i;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    FormatDecimalDigits(FractionText, a, c);
    FractionText[c + 1] = 0x2F;
    FormatDecimalDigits(FractionText + c + 2, b, c);
    return FractionText;
}
