#define M_CORE_GBA 1
#define DISABLE_THREADING 1
#define MGBA_STANDALONE 1
#define HAVE_STRDUP 1
#define HAVE_STRNDUP 1
#define HAVE_STRLCPY 1
#define HAVE_LOCALTIME_R 1
#define HAVE_LOCALE 1
#define HAVE_STRTOF_L 1
#define HAVE_SNPRINTF_L 1
#define HAVE_SETLOCALE 1
#define HAVE_XLOCALE 1
#define BUILD_STATIC 1

#define N3DS_VFS_H 1
#define N3DS_THREADING_H 1
#define POSIX_THREADING_H 1
#define SCE_VFS_H 1
#define SCE_THREADING_H 1
#define SWITCH_THREADING_H 1
#define __GETOPT_H__ 1
#define WINDOWS_THREADING_H 1
/* Copyright (c) 2013-2016 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GB_OVERRIDES_H
#define GB_OVERRIDES_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/gb/interface.h>

enum GBColorLookup {
	GB_COLORS_NONE = 0,
	GB_COLORS_CGB = 1,
	GB_COLORS_SGB = 2,
	GB_COLORS_SGB_CGB_FALLBACK = GB_COLORS_CGB | GB_COLORS_SGB
};

struct GBCartridgeOverride {
	int headerCrc32;
	enum GBModel model;
	enum GBMemoryBankControllerType mbc;

	uint32_t gbColors[12];
};

struct GBColorPreset {
	const char* name;
	uint32_t colors[12];
};

struct Configuration;
bool GBOverrideFind(const struct Configuration*, struct GBCartridgeOverride* override);
bool GBOverrideColorFind(struct GBCartridgeOverride* override, enum GBColorLookup);
void GBOverrideSave(struct Configuration*, const struct GBCartridgeOverride* override);

size_t GBColorPresetList(const struct GBColorPreset** presets);

struct GB;
void GBOverrideApply(struct GB*, const struct GBCartridgeOverride*);
void GBOverrideApplyDefaults(struct GB*);

CXX_GUARD_END

#endif
