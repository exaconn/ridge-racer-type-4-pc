/* GCC_VERSION: 2.7.2 */
#include "psyq/libcd/dsstrm4.h"
extern char *D_80110310;
void func_8008F814(int a, unsigned n) {
    unsigned i = 0;
    if (n) { do { int t = i + a; i++; *(int *)(D_80110310 + (t << 5)) = 0; } while (i < n); }
}
