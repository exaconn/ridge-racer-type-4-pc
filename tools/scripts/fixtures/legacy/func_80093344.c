/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern const char D_80011F38[];
extern LibGraphicsDriverPrefix *GpuFuncTable;
void DrawOTag(s32 ordering_table) {
    /* ASM_BARRIER: Keep the third dispatch argument distinct from the final constant zero. */
    register s32 second;
    register s32 third;
    LibGraphicsDriverPrefix *driver;
    if (D_800ABAC2.level >= 2) GPU_printf(D_80011F38, ordering_table);
    second = ordering_table;
    driver = GpuFuncTable;
    /* ASM_BARRIER: Load the current driver before preparing the dispatch zeros. */
    __asm__("" : "+r"(driver) : : "$6");
    third = 0;
    /* ASM_BARRIER: Prevent reuse of the third argument when preparing the fourth zero. */
    __asm__("" : "+r"(third));
    driver->dispatch(driver->draw_ordering_table, second, third, 0);
}
