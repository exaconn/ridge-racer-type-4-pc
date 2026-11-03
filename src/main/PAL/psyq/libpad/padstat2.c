/* GCC_VERSION: 970318 */
#include "types.h"
#include "m2c_macros.h"
#include "rr/channels.h"

void func_80098AC4(void *arg0, s32 arg1, s8 arg2);
void func_80098AD0(void *arg0, s8 arg1, s32 arg2, s8 arg3);
void func_80098AE0(u8 *channel);

void func_80098AC4(void *arg0, s32 arg1, s8 arg2) {
    M2C_FIELD(arg0, s32 *, 0x28) = arg1;
    M2C_FIELD(arg0, s8 *, 0x34) = arg2;
}

void func_80098AD0(void *arg0, s8 arg1, s32 arg2, s8 arg3) {
    M2C_FIELD(arg0, s8 *, 0x37) = arg1;
    M2C_FIELD(arg0, s32 *, 0x2C) = arg2;
    M2C_FIELD(arg0, s8 *, 0x36) = arg3;
}

void func_80098AE0(u8 *channel) {
    RrChannelStatusView *state=(RrChannelStatusView *)channel;
    switch(state->phase) {
    case 2:func_80099638(state);break;
    case 3:func_8009964C(state,state->argumentE4);break;
    case 4:func_8009968C(state,state->state47);break;
    }
}
