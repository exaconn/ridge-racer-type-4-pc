#ifndef RR_PREVIEW_RGB_H
#define RR_PREVIEW_RGB_H
#include "rr/layout.h"
/* Three defined color channels passed in one aligned ABI word. The final
 * byte is natural tail padding and has no color meaning. */
typedef struct {
    u8 r, g, b;
} __attribute__((aligned(4))) RrPreviewRgb3;
typedef struct {
    u8 r, g, b;
} RrRgbChannels;
/* Preview services pass RGB in one aligned argument word. The channel view
 * describes the three color bytes; argument adds the ABI's unused tail padding.
 * Both structures have the same common initial sequence. */
typedef union {
    RrRgbChannels channels;
    RrPreviewRgb3 argument;
} RrPreviewColor;
RR_ASSERT_SIZE(RrPreviewRgb3, 4);
RR_ASSERT_OFFSET(RrPreviewRgb3, r, 0);
RR_ASSERT_OFFSET(RrPreviewRgb3, g, 1);
RR_ASSERT_OFFSET(RrPreviewRgb3, b, 2);
RR_ASSERT_SIZE(RrRgbChannels, 3);
RR_ASSERT_SIZE(RrPreviewColor, 4);
#endif
