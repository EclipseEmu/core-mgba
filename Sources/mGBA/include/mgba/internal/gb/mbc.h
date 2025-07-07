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
#ifndef GB_MBC_H
#define GB_MBC_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/log.h>

mLOG_DECLARE_CATEGORY(GB_MBC);

struct GB;
struct GBMemory;
void GBMBCInit(struct GB* gb);
void GBMBCReset(struct GB* gb);
void GBMBCSwitchBank(struct GB* gb, int bank);
void GBMBCSwitchBank0(struct GB* gb, int bank);
void GBMBCSwitchHalfBank(struct GB* gb, int half, int bank);
void GBMBCSwitchSramBank(struct GB* gb, int bank);
void GBMBCSwitchSramHalfBank(struct GB* gb, int half, int bank);

enum GBMemoryBankControllerType GBMBCFromGBX(const void* fourcc);

enum GBCam {
	GBCAM_WIDTH = 128,
	GBCAM_HEIGHT = 112
};

struct GBMBCRTCSaveBuffer {
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t days;
	uint32_t daysHi;
	uint32_t latchedSec;
	uint32_t latchedMin;
	uint32_t latchedHour;
	uint32_t latchedDays;
	uint32_t latchedDaysHi;
	uint64_t unixTime;
};

struct GBMBCHuC3SaveBuffer {
	uint8_t regs[0x80];
	uint64_t latchedUnix;
};

struct GBMBCTAMA5SaveBuffer {
	uint8_t rtcTimerPage[0x8];
	uint8_t rtcAlarmPage[0x8];
	uint8_t rtcFreePage0[0x8];
	uint8_t rtcFreePage1[0x8];
	uint64_t latchedUnix;
};

void GBMBCRTCRead(struct GB* gb);
void GBMBCRTCWrite(struct GB* gb);

void GBMBCHuC3Read(struct GB* gb);
void GBMBCHuC3Write(struct GB* gb);

void GBMBCTAMA5Read(struct GB* gb);
void GBMBCTAMA5Write(struct GB* gb);

CXX_GUARD_END

#endif
