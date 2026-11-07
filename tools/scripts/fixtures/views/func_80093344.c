/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu_reset.h"

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define GetClut ((int (*)())GetClut)

#undef GetClut

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*)[])&D_800ABAC2)

#define GPU_printf (*(void (*(*))(const char *, s32, s32, s32))&GPU_printf)

#undef D_800ABAC2

#undef GPU_printf

/* legacy typing of shared globals in this part of the unit */
#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAB8 ((int *)D_800ABAB8)

#define DMACallback ((int (*)())DMACallback)

#undef GPU_printf

#undef D_800ABAB8

#undef DMACallback

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#define GPU_printf (*(void (*(*))())&GPU_printf)

#undef D_800ABAC2

#undef GPU_printf

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#define GPU_printf (*(void (*(*))())&GPU_printf)

#define D_800ABAB8 (*(struct { char pad[0x10]; int (*f)(); } *(*))&D_800ABAB8)

#define GPU_memset ((int (*)())GPU_memset)

#undef D_800ABAC2

#undef GPU_printf

#undef D_800ABAB8

#undef GPU_memset

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#define GPU_printf (*(M2C_UNK (*(*))(M2C_UNK *, s32))&GPU_printf)

#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABB60 (*(B (*))&D_800ABB60)

#define D_800ABAB8 ((int *)D_800ABAB8)

#undef D_800ABB60

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((RrLibGraphicsDriverPrefix *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((int *)D_800ABAB8)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

#define GPU_printf (*(int (*(*))())&GPU_printf)

#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

#undef D_800ABAB8

#undef GPU_printf

#undef D_800ABAC2

extern const char D_80011F38[];

void DrawOTag(s32 ordering_table) {
    /* ASM_BARRIER: Keep the third dispatch argument distinct from the final constant zero. */
    register s32 second;
    register s32 third;
    RrLibGraphicsDriverPrefix *driver;
    if (D_800ABAC2.level >= 2) GPU_printf(D_80011F38, ordering_table);
    second = ordering_table;
    driver = D_800ABAB8;
    /* ASM_BARRIER: Load the current driver before preparing the dispatch zeros. */
    __asm__("" : "+r"(driver) : : "$6");
    third = 0;
    /* ASM_BARRIER: Prevent reuse of the third argument when preparing the fourth zero. */
    __asm__("" : "+r"(third));
    driver->dispatch(driver->draw_ordering_table, second, third, 0);
}
