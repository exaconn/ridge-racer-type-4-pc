#include "rr/channel_gates.h"
#include "psyq/libpad/padchk.h"
void _padAllocGates(void *argument) {
    PadChannel *record=argument;
    s32 i,j,found,mask;
    s32 value;
    s32 one;
    s32 length;
    u8 *mapping,*data,*gates;
    PsyqBzero((record->output54.fields.gates+3),6);
    if(record->pending && record->data28) {
        length=6;
        if(record->length34<7)length=record->length34;
        i=0;
        if(record->entry_countE9) {
            one=1;
            do {
                found=0;
                mask=1;
                if(record->entries04[i].all_bits02)mask=255;
                mapping=record->output54.fields.payload;
                data=record->data28;
                j=0;
                if(length) {
scan:
                    if(*mapping==i) {
                        if(!(*data&mask))goto miss;
                        goto matched;
                    } else {
miss:
                        mapping++;j++;data++;
                        if(j<length)goto scan;
                    }
                }
scan_done:
                if(found) {
                    value=((u32)PadTotalGateCost+record->entries04[i].cost03);
                    if(value<61) {
                        PadTotalGateCost=value;
                        goto reserved;
matched:
                        found=1;goto scan_done;
                    } else found=0;
reserved:
                    if(found) {
                        mapping=record->output54.fields.payload;
                        gates=(u8 *)record;
                        gates+=RR_FIELD_OFFSET(PadChannel,output54.fields.gates[3]);
                        j=0;
                        if(length) {
                            do {
                                s32 selected=*mapping;
                                mapping++;
                                if(selected==i)*gates=one;
                                j++;gates++;
                            } while(j<length);
                        }
                    }
                }
                i++;
            } while(i<record->entry_countE9);
        }
        goto out;
    }
    value=record->responseE8;
    if((value-4U)<2U || value==7) {
        if(!record->pending && record->length34>=2) {
            data=record->data28;
            if((data[0]&0xC0)==0x40 && (data[1]&1) && (s32)(PadTotalGateCost+10U)<61) {
                s32 enable=1;
                (record->output54.fields.gates+3)[1]=enable;
                (record->output54.fields.gates+3)[0]=enable;
                PadTotalGateCost=PadTotalGateCost+10U;
            }
            goto out;
        }
    }
    if(((volatile PadChannel *)record)->responseE8==3) {(record->output54.fields.gates+3)[0]=1;goto out;}
    if(!record->pending) {
        s32 fill=1;
        j=5;
        gates=(u8 *)record+5;
        do {gates[RR_FIELD_OFFSET(PadChannel,output54.fields.gates[3])]=fill;j--;gates--;}while(j>=0);
    }
out:
    ;
}
