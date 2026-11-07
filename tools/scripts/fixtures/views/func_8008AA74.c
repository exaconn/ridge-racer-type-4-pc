/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

extern int _SsVabOpenHeadWithMode();

short SsVabOpenHeadSticky(int a0, short a1, int a2) { return _SsVabOpenHeadWithMode(a0, a1, VsGetAddr, a2); }
