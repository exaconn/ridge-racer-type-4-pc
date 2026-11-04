#ifndef RR_GAME_SYSTEM_MODESLOT_H
#define RR_GAME_SYSTEM_MODESLOT_H

/* 0x24-byte slots of the mode/menu work table at 0x800F7C0x; the union of the fields the
 * mode setup (modeinit), the pad handler (modepad) and the menu work code (menuwork) touch. */
typedef struct { char pad0[0xC]; char b0C; char pad1[7]; unsigned char b14; char pad2[0xB]; short s20; char pad3[2]; } W7C0_E;

typedef struct { char p0[2]; short s2; char p4[4]; short s8; short sA; char pC[4]; short s10; } W7C0_F;

#endif
