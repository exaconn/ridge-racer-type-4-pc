#ifndef RR4_PORT_SETTINGS_RASTER_H
#define RR4_PORT_SETTINGS_RASTER_H

#include <stdint.h>

#include "settings_model.h"

/* Paints the settings panel into a premultiplied RGBA image on the CPU with
 * the bundled IBM Plex Sans, as the Rage Racer and Parasite Eve ports do. It
 * knows the model and the layout, not SDL or the GPU. */

typedef struct SettingsSurface {
    uint8_t *pixels;
    int w;
    int h; /* includes one extra bottom row: a solid texel for the backdrop */
} SettingsSurface;

int SettingsSurfaceResize(SettingsSurface *surface, int width, int height);
void SettingsSurfaceRelease(SettingsSurface *surface);

/* The typeface loads on first use. */
int SettingsFontReady(void);

void SettingsPaint(SettingsSurface *surface, const SettingsModel *model, const SettingsLayout *layout);

/* Writes the panel as a PPM over mid-grey, for looking at it. */
int SettingsSurfaceDump(const SettingsSurface *surface, const char *path);

#endif
