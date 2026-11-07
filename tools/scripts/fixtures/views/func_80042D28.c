/* legacy typing of shared globals in this part of the unit */
#define FormatDecimalDigits ((void (*)())FormatDecimalDigits)

#define D_8009FA1C (*(R4_1 (*)[])&D_8009FA1C)

#undef FormatDecimalDigits

#undef D_8009FA1C

/* legacy typing of shared globals in this part of the unit */
#define FormatDecimalDigits ((void (*)())FormatDecimalDigits)

#undef FormatDecimalDigits

extern int FormatDecimalDigits();

extern char D_8010C150[];

char *FormatCountOfTotal(int a, int b, int c) {
    int i = 7;
    char *p = D_8010C150 + i;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    FormatDecimalDigits(D_8010C150, a, c);
    D_8010C150[c + 1] = 0x2F;
    FormatDecimalDigits(D_8010C150 + c + 2, b, c);
    return D_8010C150;
}
