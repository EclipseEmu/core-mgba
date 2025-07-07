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
/* Copyright (c) 2013-2014 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef ARM_DECODER_INLINES_H
#define ARM_DECODER_INLINES_H

#include "decoder.h"

#include "arm.h"

#include <stdio.h>
#include <string.h>

#define LOAD_CYCLES    \
	info->iCycles = 1; \
	info->nDataCycles = 1;

#define STORE_CYCLES              \
	info->sInstructionCycles = 0; \
	info->nInstructionCycles = 1; \
	info->nDataCycles = 1;

static inline bool ARMInstructionIsBranch(enum ARMMnemonic mnemonic) {
	switch (mnemonic) {
		case ARM_MN_B:
		case ARM_MN_BL:
		case ARM_MN_BX:
			// TODO: case: ARM_MN_BLX:
			return true;
		default:
			return false;
	}
}

#endif
