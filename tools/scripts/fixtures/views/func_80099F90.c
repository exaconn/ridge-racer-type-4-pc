/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/channels.h"

void _padSaveCommand(RrChannelTransferView *channel) {
    u8 command=channel->command37;
    channel->command37=0;
    channel->previous38=command;
}
