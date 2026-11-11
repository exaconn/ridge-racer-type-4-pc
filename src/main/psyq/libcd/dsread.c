#include "psyq/libcd/dsfinish.h"
#include "psyq/libcd/dsread.h"
int cd_read(a, b, c) int a; int b; int c; {
    int buf[2];
    int z, r;
    CdIntToPos(b, buf);
    CdControl(2, buf, 0);
    CdRead(a, c, 0x80);
    r = CdReadSync(z, 0) == 0;
    return r;
}
