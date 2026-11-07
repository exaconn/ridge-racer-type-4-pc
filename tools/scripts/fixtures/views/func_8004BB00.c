/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(T_1 (*)[])&D_800F3A38)

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(T_2 (*)[])&D_800F3A38)

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(T_3 (*)[])&D_800F3A38)

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(Pad4BC5C (*)[])&D_800F3A38)

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(W10_T (*)[])&D_800F3A38)

#undef D_800F3A38

typedef struct { char pad[0x49]; char f49; char pad2[8]; short f52; char pad3[8]; } T;

typedef struct { char pad[0x49]; char f49; char pad2[6]; short f50; short f52; char pad3[8]; } T_1;

extern T D_800F3A38[];

void PadRumbleStart(int i) {
    T_1 *t = &D_800F3A38[i];
    t->f50 = 1;
    PadSetRumbleFlags(i, 2);
    t->f49 = 0xF;
}
