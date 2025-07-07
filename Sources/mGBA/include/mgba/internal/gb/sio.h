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
#ifndef GB_SIO_H
#define GB_SIO_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/log.h>
#include <mgba/core/timing.h>
#include <mgba/gb/interface.h>

#define MAX_GBS 2

extern const int GBSIOCyclesPerTransfer[2];

mLOG_DECLARE_CATEGORY(GB_SIO);

struct GB;
struct GBSIODriver;
struct GBSIO {
	struct GB* p;

	struct mTimingEvent event;
	struct GBSIODriver* driver;

	int32_t nextEvent;
	int32_t period;
	int remainingBits;

	uint8_t pendingSB;
};

DECL_BITFIELD(GBRegisterSC, uint8_t);
DECL_BIT(GBRegisterSC, ShiftClock, 0);
DECL_BIT(GBRegisterSC, ClockSpeed, 1);
DECL_BIT(GBRegisterSC, Enable, 7);

void GBSIOInit(struct GBSIO* sio);
void GBSIOReset(struct GBSIO* sio);
void GBSIODeinit(struct GBSIO* sio);
void GBSIOSetDriver(struct GBSIO* sio, struct GBSIODriver* driver);
void GBSIOWriteSC(struct GBSIO* sio, uint8_t sc);
void GBSIOWriteSB(struct GBSIO* sio, uint8_t sb);

CXX_GUARD_END

#endif
