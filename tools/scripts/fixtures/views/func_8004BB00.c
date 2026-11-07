/* legacy typing of shared globals in this part of the unit */
#define PlayerControllers (*(T_1 (*)[])&PlayerControllers)

#undef PlayerControllers

/* legacy typing of shared globals in this part of the unit */
#define PlayerControllers (*(T_2 (*)[])&PlayerControllers)

#undef PlayerControllers

/* legacy typing of shared globals in this part of the unit */
#define PlayerControllers (*(T_3 (*)[])&PlayerControllers)

#undef PlayerControllers

/* legacy typing of shared globals in this part of the unit */
#define PlayerControllers (*(Pad4BC5C (*)[])&PlayerControllers)

#undef PlayerControllers

/* legacy typing of shared globals in this part of the unit */
#define PlayerControllers (*(W10_T (*)[])&PlayerControllers)

#undef PlayerControllers

typedef struct { char pad[0x49]; char f49; char pad2[8]; short f52; char pad3[8]; } T;

typedef struct { char pad[0x49]; char f49; char pad2[6]; short f50; short f52; char pad3[8]; } T_1;

extern T PlayerControllers[];

void PadRumbleStart(int i) {
    T_1 *t = &PlayerControllers[i];
    t->f50 = 1;
    PadSetRumbleFlags(i, 2);
    t->f49 = 0xF;
}
