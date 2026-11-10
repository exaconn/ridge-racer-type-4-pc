#include "game/system/pad.h"
#include "types.h"
#include "psyq/libpad/padctl.h"
#include "game/car/carstat.h"
#include "rr/bios_devices.h"
#include "psyq/libpad/padgate.h"
#include "rr/runtime_settings.h"
#include "rr/race_setup.h"
#include "rr/tables.h"
#include "rr/menu_pulse.h"

extern unsigned short PadButtonLayoutPresetsA[][8];
extern unsigned short PadButtonLayoutPresetsB[][8];
extern unsigned short PadDefaultButtonPresets[][8];
extern unsigned short PadButtonMap[][3][8];
extern unsigned short ActivePadButtonLayouts[][2][8];

void InitGamePads(void) {
    PsyqBzero(PlayerControllers, sizeof(PlayerControllers));
    PsyqBzero(PadPulseState, sizeof(PadPulseState));
    PlayerControllers[0].protocol10 = -1;
    PlayerControllers[1].protocol10 = -1;
    PadInitDirect((u8 *)&PadPorts[0], (u8 *)&PadPorts[1]);
    PadStartCom();
}

void SetPadButtonMapLayout(int a, int b, int c, int d) {
    int i;
    for (i = 0; i < 8; i++) {
        PadButtonMap[a][0][i] = PadButtonLayoutPresetsA[b][i];
        PadButtonMap[a][1][i] = PadButtonLayoutPresetsB[c][i];
        PadButtonMap[a][2][i] = PadDefaultButtonPresets[d][i];
        ActivePadButtonLayouts[0][0][i] = PadDefaultButtonPresets[b][i];
        ActivePadButtonLayouts[0][1][i] = PadButtonLayoutPresetsB[c][i];
    }
}

void ApplyOptionsPadButtonMaps(void) {
    SetPadButtonMapLayout(0, Options.config.f0[0].buttonLayout00, Options.config.p[0].buttonLayout0C, Options.config.w[0].buttonLayout03);
    SetPadButtonMapLayout(1, Options.config.f0[1].buttonLayout00, Options.config.p[1].buttonLayout0C, Options.config.w[1].buttonLayout03);
}
