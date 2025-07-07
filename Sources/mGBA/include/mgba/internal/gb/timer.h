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
#ifndef GB_TIMER_H
#define GB_TIMER_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/timing.h>

DECL_BITFIELD(GBRegisterTAC, uint8_t);
DECL_BITS(GBRegisterTAC, Clock, 0, 2);
DECL_BIT(GBRegisterTAC, Run, 2);

enum {
	GB_DMG_DIV_PERIOD = 16
};

struct GB;
struct GBTimer {
	struct GB* p;

	struct mTimingEvent event;
	struct mTimingEvent irq;

	uint32_t internalDiv;
	int32_t nextDiv;
	uint32_t timaPeriod;
};

void GBTimerReset(struct GBTimer*);
void GBTimerDivReset(struct GBTimer*);
uint8_t GBTimerUpdateTAC(struct GBTimer*, GBRegisterTAC tac);

struct GBSerializedState;
void GBTimerSerialize(const struct GBTimer* timer, struct GBSerializedState* state);
void GBTimerDeserialize(struct GBTimer* timer, const struct GBSerializedState* state);

CXX_GUARD_END

#endif
