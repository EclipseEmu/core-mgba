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
/* Copyright (c) 2013-2019 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GB_INPUT_H
#define GB_INPUT_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/input.h>

extern MGBA_EXPORT const struct mInputPlatformInfo GBInputInfo;

enum GBKey {
	GB_KEY_A = 0,
	GB_KEY_B = 1,
	GB_KEY_SELECT = 2,
	GB_KEY_START = 3,
	GB_KEY_RIGHT = 4,
	GB_KEY_LEFT = 5,
	GB_KEY_UP = 6,
	GB_KEY_DOWN = 7,
	GB_KEY_MAX,
};

CXX_GUARD_END

#endif
