/*
 * Firmware version and build date / time (see build_info.c).
 */

#pragma once

// Firmware version, shown in the GUI header and returned by /status
#define FTX1_VERSION "1.7.5"

// Build date and time of this firmware, e.g. "Oct 30 2026 21:56:13" (build machine local time)
const char *build_info_get(void);
