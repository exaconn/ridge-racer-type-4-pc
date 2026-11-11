#ifndef RR4_PORT_SETTINGS_OVERLAY_H
#define RR4_PORT_SETTINGS_OVERLAY_H

#include <stddef.h>

#include "preferences.h"

/* The Escape menu: Escape (or the gamepad Guide button) opens a settings
 * panel over the frozen game, as in the Parasite Eve and Rage Racer ports.
 * While it is up the game is held inside its VSync wait: no game frame runs,
 * the PsyZ host clock (root counters, sequencer tick, CD timing) is paused
 * and audio output is paused, then restored to its earlier state. Escape
 * never reaches PsyZ's quit shortcut; Quit game is a menu row. The panel is
 * painted on the CPU (settings_raster.c) and drawn over the presented frame
 * through the PsyZ SDL_GPU overlay hooks. Changes apply at once and are
 * saved to the preferences file when the panel closes.
 *
 * Interactive runs only: headless runs never install it, except to check
 * it with RR4_SETTINGS_SCRIPT=key@present,... (PSYZ_HEADLESS=gpu): the keys
 * escape up down left right enter tab backspace are pressed at those
 * presents, and opening and closing the panel are logged. Without a
 * preferences file nothing is saved. */

/* prefs is the live configuration (applied at startup by the caller); the
 * panel edits it in place. mounted is the image in the drive. */
void SettingsOverlayInstall(Rr4Prefs *prefs, const char *mounted);

/* The native file dialog for a CUE image, blocking until it is answered.
 * Returns 1 and the path in out when one was chosen. */
int SettingsChooseDisc(char *out, size_t size);

#endif
