#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/gtemat4.h"
/* CC1_FLAGS: -fno-strength-reduce */
#include "rr/overlay665_matrix.h"

void ovl665_func_80116358(const PreviewMatrixWorkspace *arg0, const PreviewLightInput36 *input) {
    PreviewMatrixWorkspace mat[2];
    s32 i;
    u16 *dst1,*dst2;
    const u16 *src2;
    const u8 *arg1=input->bytes;
    i=0;
    dst2=mat[1].halves;
    mat[0]=*arg0;
    dst1=LightMatrix.halves;
    src2=input->halves;
    do {
        dst1[0]=src2[6];
        dst1[1]=src2[7];
        dst1[2]=src2[8];
        i++;
        src2+=4;
        dst2[0]=arg1[0]<<4;
        dst1+=3;
        dst2[3]=arg1[1]<<4;
        dst2[6]=arg1[2]<<4;
        arg1+=4;
        dst2++;
    } while (i<3);
    MulMatrix2(&LightMatrix.matrix, &mat[0].matrix);
    SetLightMatrix(&mat[0].matrix);
}
