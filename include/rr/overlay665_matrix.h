#ifndef RR_OVERLAY665_MATRIX_H
#define RR_OVERLAY665_MATRIX_H
#include "rr/matrix_workspace.h"
#include "rr/lighting_types.h"

/* Input is halfword-aligned; matrix source is word-aligned. Capture all32
 * source bytes before the ordered basis/color writes, including aliasing. */
void ovl665_setup_preview_light_matrix(const PreviewMatrixWorkspace *source,
    const PreviewLightInput36 *input);

#endif
