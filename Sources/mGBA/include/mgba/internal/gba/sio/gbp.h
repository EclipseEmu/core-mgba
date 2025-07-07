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
/* Copyright (c) 2013-2021 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GBA_GBP_H
#define GBA_GBP_H

#include <mgba-util/common.h>

CXX_GUARD_START

struct GBASIOPlayer;
struct GBASIOPlayerKeyCallback {
	struct mKeyCallback d;
	struct GBASIOPlayer* p;
};

struct GBASIOPlayer {
	struct GBASIODriver d;
	struct GBA* p;
	unsigned inputsPosted;
	int txPosition;
	struct mTimingEvent event;
	struct GBASIOPlayerKeyCallback callback;
	bool oldOpposingDirections;
	struct mKeyCallback* oldCallback;
};

void GBASIOPlayerInit(struct GBASIOPlayer* gbp);
void GBASIOPlayerReset(struct GBASIOPlayer* gbp);

struct GBAVideo;
void GBASIOPlayerUpdate(struct GBA* gba);
bool GBASIOPlayerCheckScreen(const struct GBAVideo* video);

CXX_GUARD_END

#endif
