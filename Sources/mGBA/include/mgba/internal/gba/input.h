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
#ifndef GBA_INPUT_H
#define GBA_INPUT_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/input.h>

extern MGBA_EXPORT const struct mInputPlatformInfo GBAInputInfo;

enum GBAKey {
	GBA_KEY_A = 0,
	GBA_KEY_B = 1,
	GBA_KEY_SELECT = 2,
	GBA_KEY_START = 3,
	GBA_KEY_RIGHT = 4,
	GBA_KEY_LEFT = 5,
	GBA_KEY_UP = 6,
	GBA_KEY_DOWN = 7,
	GBA_KEY_R = 8,
	GBA_KEY_L = 9,
	GBA_KEY_MAX,
	GBA_KEY_NONE = -1
};

CXX_GUARD_END

#endif
