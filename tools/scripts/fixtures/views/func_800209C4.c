/* legacy typing of shared globals in this part of the unit */
#define CurrentDrawBuf ((char *)CurrentDrawBuf)

#undef CurrentDrawBuf

/* legacy typing of shared globals in this part of the unit */
#define AddDrawModePrim ((s32 (*)())AddDrawModePrim)

#undef AddDrawModePrim

/* legacy typing of shared globals in this part of the unit */
#define AddDrawModePrim ((int (*)())AddDrawModePrim)

#undef AddDrawModePrim

extern int AddPrim();

extern char BadgeRowPresetTableA[];

extern char BadgeRowPresetTableB[];

extern char Rect16RowDefaultPresets[];

extern char D_800D0160[];

extern short LinkPlayerCount;

void InitRaceHudPackets(int a0)
{
    int n, i, j, a, b;
    char *tab;
    char *base;
    char *s1;
    char *s0;
    switch (a0) {
    case 0:
    case 5:
    default:
        n = 4;
        tab = BadgeRowPresetTableA;
        break;
    case 1:
        n = 8;
        tab = BadgeRowPresetTableB;
        break;
    case 2:
    case 3:
    case 4:
    case 6:
        if (LinkPlayerCount == 2 && a0 == 4) goto l60;
        if (a0 != 6) { n = 12; goto l64; }
    l60:
        n = 6;
    l64:
        tab = Rect16RowDefaultPresets;
        break;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < 2; j++) {
            s1 = D_800D0160 + j * 0x22778;
            s0 = s1 + (i * 0x14 + 0x18);
            InitHudSpritePacket(s0, tab + i * 0x14);
            if (i != 0) AddPrim(s1 + (i * 0x14 + 4), s0);
        }
    }
}
