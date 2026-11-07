/* CC1_FLAGS: -mno-split-addresses */

extern struct { signed char a; char p[4]; signed char b; } D_80110107;

extern char *_svm_tn;

unsigned short note2pitch2(a0, a1) short a0; short a1; {
    short i = D_80110107.a * 16 + D_80110107.b;
    char *t = (char *)(i * 32 + (int)_svm_tn);
    return SsPitchFromNote(a0, a1, t[4], t[5]);
}
