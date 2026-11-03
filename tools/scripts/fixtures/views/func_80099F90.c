/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/channels.h"

void func_80099F90(RrChannelTransferView *channel) {
    u8 command=channel->command37;
    channel->command37=0;
    channel->previous38=command;
}
