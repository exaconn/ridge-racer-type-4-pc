#include "game/gfx/model.h"
#include "game/camera/viewcam.h"
#include "scratchpad.h"
#include "rr/camera_motion.h"
#include "rr/cpu_md_leaf.h"
#include "game/camera/cammove.h"
#include "rr/overlay665_render.h"

#define W_ADD(a,b) ((u32)(a)+(b))
#define W_SUB(a,b) ((u32)(a)-(b))
/* Constant products below expand to shifts/adds; variable MD operations use
 * the typed HI/LO interface with an independent native arithmetic reference. */
#define W_MUL(a,b) ((u32)(a)*(b))

void UpdateCameraMotion(MotionClock20 *input) {
    MotionClock20 *arg0=input;
    PresentationContextPrefix *root=&RaceContext;
    PresentationController *object=root->controller3D8;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_5;
    /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
    register s32 temp_v1_3 __asm__("$3");
    CameraMotion70 *temp_a3;
    CameraKey16 *temp_v1_4;

    temp_a3 = &object->motion74;
    temp_v1 = temp_a3->mode04;
    switch (temp_v1) {
    case 0:
        temp_v1_2 = object->motion74.phase00;
        switch (temp_v1_2) {
        case 0:
            if (arg0->active == 0) {
copy_start:
                temp_a3->current40[0] = temp_a3->start10[0];
                temp_a3->current40[1] = temp_a3->start10[1];
                temp_a3->current40[2] = temp_a3->start10[2];
                return;
            }
            {
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 value __asm__("$2"),start __asm__("$3");
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 first __asm__("$5"),second __asm__("$4"),third;
                value= arg0->tempo;start=object->scale4C;
                value=W_SUB(W_MUL(value,2),300);RR_MD_MULT_S(value,start);
                value = temp_a3->keys0C[temp_a3->key08].x;
                temp_a3->target20[0] = value;
                value = W_SUB(value, temp_a3->start10[0]);
                first = W_MUL(W_ADD(W_MUL(value, 2), value), 8);
                RR_MD_LO_ACCUM_SHL2(temp_lo, first, value);
                RR_MD_DIV_S(first, first, temp_lo);
                value = temp_a3->keys0C[temp_a3->key08].y;
                temp_a3->target20[1] = value;
                value = W_SUB(value, temp_a3->start10[1]);
                second = W_MUL(value, 100);
                RR_MD_DIV_S(second, second, temp_lo);
                value = temp_a3->keys0C[temp_a3->key08].z;
                /* ASM_BARRIER: retain the original read/publish phase before this physical is reused. */
                temp_a3->target20[2]=value;__asm__("" : "=r"(value) : "0"(value) : "memory");start=value;value=temp_a3->start10[2];start=W_SUB(start,value);
                third=W_MUL(start,100);
                RR_MD_DIV_S(third,third,temp_lo);
                temp_a3->delta30[0]=first;
                temp_a3->delta30[1]=second;
                temp_a3->delta30[2]=third;
            }
            {
                PresentationController *settings=root->controller3D8;
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 result __asm__("$2"),product __asm__("$3");
                result=settings->scale4C;product= arg0->tempo;
                result=W_MUL(result,2);RR_MD_MULT_S(product,result);RR_MD_LO(product);
                result=0x51EB851FU;RR_MD_MULT_U(product,result);

                product=(u16)settings->scale4C;
                temp_a3->counter50=0;
                product+=1;

                RR_MD_HI(result);result=(u32)result>>5;
                result=W_SUB(result,product);temp_a3->horizon52=(s16)result;
            }
            object->motion74.phase00 = W_ADD(object->motion74.phase00,1);
        case 1:
            temp_a3->current40[0] = W_ADD(temp_a3->current40[0], temp_a3->delta30[0]);
            temp_a3->current40[1] = W_ADD(temp_a3->current40[1], temp_a3->delta30[1]);
            temp_a3->current40[2] = W_ADD(temp_a3->current40[2], temp_a3->delta30[2]);
            if (temp_a3->delta30[0] > 0) {
                if (temp_a3->current40[0] > temp_a3->target20[0]) {
                    temp_a3->current40[0] = temp_a3->target20[0];
                }
            } else {
                if (temp_a3->current40[0] < temp_a3->target20[0]) {
                    temp_a3->current40[0] = temp_a3->target20[0];
                }
            }
            if (temp_a3->delta30[1] > 0) {
                if (temp_a3->current40[1] > temp_a3->target20[1]) {
                    temp_a3->current40[1] = temp_a3->target20[1];
                }
            } else {
                if (temp_a3->current40[1] < temp_a3->target20[1]) {
                    temp_a3->current40[1] = temp_a3->target20[1];
                }
            }
            if (temp_a3->delta30[2] > 0) {
                if (temp_a3->current40[2] > temp_a3->target20[2]) {
                    temp_a3->current40[2] = temp_a3->target20[2];
                }
            } else {
                if (temp_a3->current40[2] < temp_a3->target20[2]) {
                    temp_a3->current40[2] = temp_a3->target20[2];
                }
            }
            {
                s32 multiplier=0x51EB0000;
                s32 first,second,third;
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 highFirst __asm__("$10"),highSecond __asm__("$8"),highThird __asm__("$9");
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 quotient __asm__("$2");
                first=temp_a3->current40[0];RR_MD_MULT_S_FINISH16(first,multiplier,0x851F);RR_MD_HI(highFirst);
                second=temp_a3->current40[1];RR_MD_MULT_S(second,multiplier);
                temp_v1_3=temp_a3->counter50;RR_MD_HI(highSecond);
                third=temp_a3->current40[2];temp_v1_3++;
                RR_MD_MULT_S(third,multiplier);
                first>>=31;temp_a3->counter50=(u16)temp_v1_3;
                temp_v1_3 = (temp_v1_3 * 0x10000U);second>>=31;
                quotient=highFirst>>5;quotient=W_SUB(quotient,first);temp_a3->angles64[0]=(s16)quotient;
                quotient=highSecond>>5;quotient=W_SUB(quotient,second);third>>=31;temp_a3->angles64[1]=(s16)quotient;
                RR_MD_HI(highThird);quotient=highThird>>5;quotient=W_SUB(quotient,third);temp_a3->angles64[2]=(s16)quotient;
            }
            if ((temp_v1_3>>16) >= temp_a3->horizon52) {
                temp_v0_4 = W_ADD(temp_a3->key08,1);
                temp_a3->key08 = temp_v0_4;
                if (temp_v0_4 >= 3) {
                    temp_v1_4 = temp_a3->keys0C;
                    temp_v0_5 = ((volatile CameraMotion70 *)temp_a3)->keys0C->x;
                    temp_a3->current40[0] = temp_v0_5;
                    temp_a3->start10[0] = temp_v0_5;
                    temp_v0_6 = temp_v1_4->y;
                    temp_a3->current40[1] = temp_v0_6;
                    temp_a3->start10[1] = temp_v0_6;
                    temp_v1_5 = temp_v1_4->z;
                    temp_a3->key08 = 1;
                    temp_a3->current40[2] = temp_v1_5;
                    temp_a3->start10[2] = temp_v1_5;
                } else {
                    temp_a3->start10[0] = temp_a3->target20[0];
                    temp_a3->start10[1] = temp_a3->target20[1];
                    temp_a3->start10[2] = temp_a3->target20[2];
                }
                temp_a3->counter50 = 0U;
                temp_a3->phase00 = 0;
                return;
            }
            return;
        }
        break;
    case 1:
        switch (object->motion74.phase00) {
        case 0:
            if (arg0->active == 0) {
                goto copy_start;
            }
            {
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 value __asm__("$2"),start __asm__("$3");
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 first __asm__("$5"),second __asm__("$4"),third;
                value= arg0->tempo;start=object->scale4C;
                value=W_SUB(W_MUL(value,2),300);RR_MD_MULT_S(value,start);
                value = temp_a3->keys0C[temp_a3->key08].x;
                temp_a3->target20[0] = value;
                value = W_SUB(value, temp_a3->start10[0]);
                first = W_MUL(W_ADD(W_MUL(value, 2), value), 8);
                RR_MD_LO_ACCUM_SHL2(temp_lo_2, first, value);
                RR_MD_DIV_S(first, first, temp_lo_2);
                value = temp_a3->keys0C[temp_a3->key08].y;
                temp_a3->target20[1] = value;
                value = W_SUB(value, temp_a3->start10[1]);
                second = W_MUL(value, 100);
                RR_MD_DIV_S(second, second, temp_lo_2);
                value = temp_a3->keys0C[temp_a3->key08].z;
                /* ASM_BARRIER: retain the original read/publish phase before this physical is reused. */
                temp_a3->target20[2]=value;__asm__("" : "=r"(value) : "0"(value) : "memory");start=value;value=temp_a3->start10[2];start=W_SUB(start,value);
                third=W_MUL(start,100);
                RR_MD_DIV_S(third,third,temp_lo_2);
                temp_a3->delta30[0]=first;
                temp_a3->delta30[1]=second;
                temp_a3->delta30[2]=third;
            }
            object->motion74.phase00 = W_ADD(object->motion74.phase00,1);
        case 1:
            {
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 value __asm__("$2"),delta;
                s32 first,second,third;
                /* ASM_BARRIER: retain the initialized value or pointer in the original phase. */
                register s32 highFirst __asm__("$9"),highSecond __asm__("$8"),highThird __asm__("$6");
                s32 multiplier=0x51EB851F;
                /* ASM_BARRIER: retain the original read/publish phase before this physical is reused. */
                value=temp_a3->current40[0];delta=temp_a3->delta30[0];value=W_ADD(value,delta);temp_a3->current40[0]=value;__asm__("" : "=r"(value) : "0"(value) : "memory");first=value;
                RR_MD_MULT_S(first,multiplier);
                /* ASM_BARRIER: retain the original read/publish phase before this physical is reused. */
                value=temp_a3->current40[1];delta=temp_a3->delta30[1];value=W_ADD(value,delta);RR_MD_HI(highFirst);temp_a3->current40[1]=value;__asm__("" : "=r"(value) : "0"(value) : "memory");second=value;
                RR_MD_MULT_S(second,multiplier);
                /* ASM_BARRIER: retain the original read/publish phase before this physical is reused. */
                value=temp_a3->current40[2];delta=temp_a3->delta30[2];value=W_ADD(value,delta);RR_MD_HI(highSecond);temp_a3->current40[2]=value;__asm__("" : "=r"(value) : "0"(value) : "memory");third=value;
                RR_MD_MULT_S(third,multiplier);
                first>>=31;value=highFirst>>5;value=W_SUB(value,first);second>>=31;temp_a3->angles64[0]=(s16)value;
                value=highSecond>>5;value=W_SUB(value,second);third>>=31;temp_a3->angles64[1]=(s16)value;
                RR_MD_HI(highThird);value=highThird>>5;value=W_SUB(value,third);temp_a3->angles64[2]=(s16)value;
            }
            break;
        }
        break;
    }
}

typedef struct {
    u8 reserved00[0x78];
    s32 w78;
} CamMoveScratch;

void DrawPresentationMotionModel(void)
{
    MatrixWords32 modelMatrix;
    MatrixWords32 cameraMatrix;
    Vec3i scale;
    s32 value;

    SetupObjectLightMatrices((char *)&RaceContext.controller3D8->motion74.modelPosition54,
        (char *)&RaceContext.controller3D8->lighting20, &modelMatrix, &cameraMatrix);
    value = RaceContext.controller3D8->motion74.modelScale6C;
    scale.z = value;
    scale.y = value;
    scale.x = value;
    ScaleMatrix((u32 *)modelMatrix.w, &scale.x);
    SelectModelBank(7);
    SetGteObjectTransform(
        (ObjectTransform38 *)(SCRATCHPAD + 0x7C),
        (s16 *)&RaceContext.controller3D8->motion74.modelPosition54,
        modelMatrix.w);
    ((CamMoveScratch *)SCRATCHPAD)->w78 = 0;
    SubmitModelTwoTri((PreviewRenderScratch *)SCRATCHPAD, ModelBankFormat < 1, 0, 0x100);
}
