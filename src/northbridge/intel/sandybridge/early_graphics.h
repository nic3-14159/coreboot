/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef SANDYBRIDGE_GRAPHICS_EARLY_H
#define SANDYBRIDGE_GRAPHICS_EARLY_H

#include <stdbool.h>

/*
 * SoC override
 *
 * All new SoC must implement below functionality.
 */

/* Configure display panel */
void early_graphics_panel_init(void);

/*
 * Early graphics module API
 * Graphics at this stage is limited to VGA text mode.
 */

/* Initialize graphics.  Return true if VGA text mode is ready to use. */
bool early_graphics_init(void);

/* Clear graphics configuration, turn off the displays. */
void early_graphics_stop(void);

#endif	/* SANDYBRIDGE_GRAPHICS_EARLY_H */

