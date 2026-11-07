/* GCC_VERSION: 2.7.2 */

extern char D_801144D8[];

char *_padPortToChannel(a0) int a0; {
    char *p = D_801144D8;
    if (a0 & 0xF0) p += 0xF0;
    return p;
}
