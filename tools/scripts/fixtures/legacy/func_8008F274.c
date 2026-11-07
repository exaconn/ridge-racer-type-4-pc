extern int CdControlB(), VSync();
int CdDiskReady(a0)
int a0;
{
    unsigned char sp10[8];
    int r, i;
    CdControlB(1, 0, sp10);
    if (sp10[0] & 0x10) return 0x10;
    r = CdControlB(0x13, 0, sp10);
    if (a0 == 1) {
        if (sp10[0] == 2 && r != 0) return 2;
        return 5;
    }
    for (i = 0; i < 10; i++) {
        if (sp10[0] & 2) {
            if (sp10[0] == 2 && r != 0) return 2;
            while (1) {
                VSync(0x1E);
                r = CdControlB(0x13, 0, sp10);
                if (sp10[0] & 0x10) return 0x10;
                if (sp10[0] == 2 && r != 0) return 2;
            }
        }
        VSync(0x1E);
        r = CdControlB(0x13, 0, sp10);
        if (sp10[0] & 0x10) return 0x10;
    }
    return 5;
}
