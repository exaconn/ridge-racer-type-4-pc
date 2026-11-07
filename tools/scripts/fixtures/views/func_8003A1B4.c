/* CC1_FLAGS: -fno-strength-reduce */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define func_8005CA88 ((int (*)())func_8005CA88)

#define D_800F4D70 (*(int (*))&D_800F4D70)

#define D_800F4C6A (*(short (*))&D_800F4C6A)

#undef SetDispMask

#undef func_8005CA88

#undef D_800F4D70

#undef D_800F4C6A

extern int func_8001E84C();

extern int AllocBadgeRows();

extern int AllocRect16Rows();

extern int AllocGlyphRows();

extern int AllocUiRows();

extern int func_8003FAA0();

extern int CdDirectoryBuffer;

extern char D_8001075C[];

extern int Rect16RowCount;

extern int LoadCursor;

extern int D_800ACC1C;

extern int D_800ACC24;

extern int BadgeRowCount;

extern int UiRowCount;

extern int GlyphRowCount;

extern int MenuTextRowCount;

extern int D_800F4D70[];

void InitMenuArenas()
{
  /* ASM_BARRIER: pin the 0x2D000 constant to $a2 so the addu and the call share one lui/ori like the original */
  register int c asm("$6") = 0x2D000;
  BadgeRowCount = 10;
  Rect16RowCount = 4;
  GlyphRowCount = 4;
  UiRowCount = 0x28;
  D_800ACC24 = 1;
  MenuTextRowCount = 0x37;
  D_800F4D70[1] = CdDirectoryBuffer + c;
  LoadCursor = D_800F4D70[1];
  func_8001E84C(D_8001075C, LoadCursor, 0x2D000);
  LoadCursor = LoadCursor + AllocBadgeRows(LoadCursor);
  LoadCursor = LoadCursor + AllocRect16Rows(LoadCursor);
  LoadCursor = LoadCursor + AllocGlyphRows(LoadCursor);
  LoadCursor = LoadCursor + func_8003FAA0(LoadCursor);
  LoadCursor = LoadCursor + AllocUiRows(LoadCursor);
  D_800ACC1C = LoadCursor;
  D_800F4D70[1] = LoadCursor + 0x19000;
  LoadCursor = LoadCursor + 0x19000;
}
