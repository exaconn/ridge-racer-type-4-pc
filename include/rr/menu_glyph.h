#ifndef RR_MENU_GLYPH_H
#define RR_MENU_GLYPH_H
#include "types.h"
/* Three signed-byte components of the signed glyph index. The caller supplies
 * three writable bytes; the resident helper uses truncation toward zero. */
void func_80037040(s8 *output,s32 glyph);
#endif
