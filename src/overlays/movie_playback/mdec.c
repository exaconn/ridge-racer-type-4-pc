#include "rr/overlay668_decode.h"
#include "rr/overlay668_control.h"
#include "rr/overlay668_dma.h"
#include "rr/diagnostics.h"
#include "rr/overlay668_wait.h"
#include "types.h"

void ovl668_mdec_reset(s32 mode){if(mode==0)ResetCallback();ovl668_mdec_reset_hw(mode);}

u32 *ovl668_mdec_get_env(u32 *output){
 u32 *cursor=output,*source=ovl668_mdec_quant_cmd.luma;
 s32 n;
 for(n=15;n!=-1;n--)*cursor++=*source++;
 cursor=output+16;source=ovl668_mdec_quant_cmd.chroma;
 for(n=15;n!=-1;n--)*cursor++=*source++;
 cursor=output+32;source=ovl668_mdec_scale_cmd.matrix;
 for(n=31;n!=-1;n--)*cursor++=*source++;
 return output;
}

u32 *ovl668_mdec_put_env(u32 *input){
 u32 *source=input,*destination=ovl668_mdec_quant_cmd.luma;
 s32 n;
 for(n=15;n!=-1;n--)*destination++=*source++;
 destination=ovl668_mdec_quant_cmd.chroma;source=input+16;
 for(n=15;n!=-1;n--)*destination++=*source++;
 ovl668_mdec_start_in_dma(&ovl668_mdec_quant_cmd,32);
 ovl668_mdec_start_in_dma(&ovl668_mdec_scale_cmd,32);
 return input;
}

u32 ovl668_mdec_buf_size(const StrCommandHeader *header){return header->halves[0];}

void ovl668_mdec_in(StrCommandHeader *header,u32 flags){
 if(flags&1)header->word&=~0x08000000U;else header->word|=0x08000000U;
 if(flags&2)header->word|=0x02000000U;else header->word&=~0x02000000U;
 ovl668_mdec_start_in_dma(header,header->halves[0]);
}

void ovl668_mdec_out(void *output,u32 words){ovl668_mdec_start_out_dma(output,words);}

s32 ovl668_mdec_in_sync(s32 mode){
    s32 result;
    if(mode==0)result=ovl668_mdec_wait_in_dma();
    else result=(ovl668_mdec_read_status()>>29)&1;
    return result;
}

s32 ovl668_mdec_out_sync(s32 mode){
    s32 result;
    if(mode==0)result=ovl668_mdec_wait_out_dma();
    else result=(ovl668_mdec_read_status()>>24)&1;
    return result;
}

LibInterruptCallback ovl668_mdec_in_callback(LibInterruptCallback callback){return DMACallback(0,callback);}

LibInterruptCallback ovl668_mdec_out_callback(LibInterruptCallback callback){return DMACallback(1,callback);}

extern const char ovl668_mdec_reset_error_fmt[];

void ovl668_mdec_reset_hw(s32 mode) {
    switch (mode) {
    case 0:
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x80000000);
        RR_STR_WRITE32(ovl668_dma0_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_dma1_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x60000000);
        ovl668_mdec_start_in_dma(&ovl668_mdec_quant_cmd, 32);
        ovl668_mdec_start_in_dma(&ovl668_mdec_scale_cmd, 32);
        break;
    case 1:
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x80000000);
        RR_STR_WRITE32(ovl668_dma0_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_dma1_chcr_ptr, 0);
        RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, (RR_STR_READ32(ovl668_dma1_chcr_ptr), 0x60000000));
        break;
    default:
        DebugPrintStub(ovl668_mdec_reset_error_fmt, mode);
        break;
    }
}

void ovl668_mdec_start_in_dma(const void *command,u32 words)
{
    ovl668_mdec_wait_in_dma();
    *(u32 *)ovl668_dpcr_ptr |= 0x88;
    *ovl668_dma0_madr_ptr = (const u8 *)command + 4;
    *(u32 *)ovl668_dma0_bcr_ptr = ((words >> 5) << 16) | 32;
    *(u32 *)ovl668_mdec0_cmd_ptr = *(const u32 *)command;
    *(u32 *)ovl668_dma0_chcr_ptr = 0x01000201;
}

void ovl668_mdec_start_out_dma(void *output,u32 words)
{
    ovl668_mdec_wait_out_dma();
    *(u32 *)ovl668_dpcr_ptr |= 0x88;
    *(u32 *)ovl668_dma1_chcr_ptr = 0;
    *ovl668_dma1_madr_ptr = output;
    *(u32 *)ovl668_dma1_bcr_ptr = ((words >> 5) << 16) | 32;
    *(u32 *)ovl668_dma1_chcr_ptr = 0x01000200;
}



s32 ovl668_mdec_wait_in_dma(void)
{
    volatile s32 counter = 0x100000;
    while (*(u32 *)ovl668_mdec1_ctrl_ptr & 0x20000000) {
        if (--counter == -1) {
            ovl668_mdec_handle_timeout(ovl668_mdec_in_timeout_msg);
            return -1;
        }
    }
    return 0;
}



s32 ovl668_mdec_wait_out_dma(void)
{
    volatile s32 counter = 0x100000;
    while (*(u32 *)ovl668_dma1_chcr_ptr & 0x01000000) {
        if (--counter == -1) {
            ovl668_mdec_handle_timeout(ovl668_mdec_out_timeout_msg);
            return -1;
        }
    }
    return 0;
}

extern u32 *ovl668_mdec1_ctrl_ptr;
u32 ovl668_mdec_read_status(void) { return *ovl668_mdec1_ctrl_ptr; }

extern const char ovl668_mdec_timeout_fmt[];
s32 ovl668_mdec_handle_timeout(const char *message) {
    /* Preserve the full native pointer; PAL uses32-bit pointers. */
    u32 *port;
    s32 result;
    DebugPrintStub(ovl668_mdec_timeout_fmt, message);
    RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, 0x80000000);
    RR_STR_WRITE32(ovl668_dma0_chcr_ptr, 0);
    port = ovl668_dma1_chcr_ptr;
    RR_STR_WRITE32(port, 0);
    result = 0;
    RR_STR_WRITE32(ovl668_mdec1_ctrl_ptr, (RR_STR_READ32(ovl668_dma1_chcr_ptr), 0x60000000));
    return result;
}
