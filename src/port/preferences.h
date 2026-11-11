#ifndef RR4_PORT_PREFERENCES_H
#define RR4_PORT_PREFERENCES_H

#include <stddef.h>

/* Desktop preferences: presentation, audio, input bindings and the disc
 * image, kept as TOML in ridge-racer-type-4.toml. The file is chosen as in
 * the Parasite Eve port:
 *   1. RR4_CONFIG_HOME/ridge-racer-type-4.toml when RR4_CONFIG_HOME is set;
 *   2. a portable ridge-racer-type-4.toml next to the executable (or in the
 *      working directory) when one already exists;
 *   3. the per-user configuration directory (XDG_CONFIG_HOME or
 *      ~/.config/ridge-racer-type-4 on Linux, the SDL preference path on
 *      macOS and Windows).
 * A file next to the executable and one in the working directory are read
 * first, so the user file overrides them. Headless runs never read or write
 * preferences. */

enum {
    RR4_PREFS_BUTTONS = 16, /* PsyZ pad bit order: L2 R2 L1 R1 ... Left */
    RR4_PREFS_PATH_MAX = 4096,
};

typedef enum Rr4VsyncPref {
    RR4_VSYNC_AUTO = 0,
    RR4_VSYNC_ON = 1,
    RR4_VSYNC_OFF = 2, /* PsyZ frame limiter */
} Rr4VsyncPref;

typedef struct Rr4Prefs {
    /* [graphics] */
    unsigned scale; /* internal resolution: 1, 2, 4, 6 or 8 */
    int fullscreen;
    int vsync; /* Rr4VsyncPref */
    int square_pixels;
    int disable_dithering;
    int antialiasing;
    int texture_filtering;
    int geometry_correction;
    int frame_interpolation;
    /* [audio] */
    unsigned volume; /* 0..100 */
    int mute;
    /* [disc] */
    char disc[RR4_PREFS_PATH_MAX];
    /* [keyboard] SDL scancodes, [controller] SDL gamepad buttons (plus
     * SDL_GAMEPAD_BUTTON_COUNT / +1 for the left / right trigger). */
    int keys[RR4_PREFS_BUTTONS];
    int buttons[RR4_PREFS_BUTTONS];
} Rr4Prefs;

/* Lower-case pad button names, the TOML keys ("l2" ... "left"). */
const char *Rr4PrefsButtonKey(int button);
/* Display names ("L2" ... "Left"). */
const char *Rr4PrefsButtonLabel(int button);

/* Records the bindings PsyZ starts with as the defaults. Call before
 * anything changes them; later calls do nothing. */
void Rr4PrefsCaptureDefaults(void);
/* The defaults: original presentation, full volume, PsyZ's bindings. */
const Rr4Prefs *Rr4PrefsDefaults(void);

/* Reads one TOML file over prefs. A missing file is not an error; invalid
 * lines are reported on stderr and skipped. Returns 1 if the file existed. */
int Rr4PrefsLoadFile(Rr4Prefs *prefs, const char *path);
/* Writes prefs atomically (temporary file and rename). Returns 1 on success. */
int Rr4PrefsSaveFile(const Rr4Prefs *prefs, const char *path);

/* Resolves the preferences file as described above (creating its directory)
 * and loads the defaults, then every layer, into prefs. The chosen path is
 * kept for Rr4PrefsSave; returns 0 if no location could be determined. */
int Rr4PrefsLoad(Rr4Prefs *prefs);
int Rr4PrefsSave(const Rr4Prefs *prefs);
const char *Rr4PrefsPath(void);

/* Applies prefs to PsyZ. Window-dependent options (fullscreen) only take
 * effect when window_ready is non-zero. */
void Rr4PrefsApply(const Rr4Prefs *prefs, int window_ready);

/* Human-readable binding names, never NULL. */
const char *Rr4PrefsKeyName(int scancode);
const char *Rr4PrefsGamepadName(int binding);

#endif
