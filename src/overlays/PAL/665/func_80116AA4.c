#include "rr/overlay665_strokes.h"

void ovl665_draw_callout_strokes(const StrokeStep4 *const *paths,s32 factor) {
    u16 xy[4];
    StrokeColor4 color=ovl665_callout_stroke_color;
    const StrokeStep4 *step;
    u32 sum;
    s32 limit,n,i;

    if (factor) {
        for (i=0;i<5;i++) {
            xy[0]=ovl665_callout_endpoint_coords[i][0];
            xy[1]=ovl665_callout_endpoint_coords[i][1];
            step=paths[i];
            sum=0;
            while (step->direction!=-1) {
                sum=(sum+step->length);
                step++;
            }
            limit=(s32)(sum*factor)/16;
            step=paths[i];
            sum=0;
            while (step->direction!=-1) {
                if (limit>=(s32)(sum+step->length)) {
                    n=step->length;
                    sum=(sum+step->length);
                } else {
                    n=(limit-sum);
                    sum=(sum+n);
                }
                switch (step->direction) {
                case 0:
                    xy[2]=xy[0];
                    xy[3]=xy[1]-n;
                    AddLineF2Prim((const s16 *)xy,color.bytes,0,0);
                    break;
                case 1:
                    xy[2]=xy[0];
                    xy[3]=xy[1]+(u16)n;
                    AddLineF2Prim((const s16 *)xy,color.bytes,0,0);
                    break;
                case 2:
                    xy[2]=xy[0]-n;
                    xy[3]=xy[1];
                    AddLineF2Prim((const s16 *)xy,color.bytes,0,0);
                    xy[1]++;
                    xy[3]++;
                    AddLineF2Prim((const s16 *)xy,color.bytes,0,0);
                    break;
                case 3:
                    xy[2]=xy[0]+(u16)n;
                    xy[3]=xy[1];
                    AddLineF2Prim((const s16 *)xy,color.bytes,0,0);
                    xy[1]++;
                    xy[3]++;
                    AddLineF2Prim((const s16 *)xy,color.bytes,0,0);
                    break;
                }
                xy[0]=xy[2];
                xy[1]=xy[3];
                step++;
            }
        }
    }
}
