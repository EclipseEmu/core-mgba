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
/* Copyright (c) 2013-2018 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GBA_MATRIX_H
#define GBA_MATRIX_H

#include <mgba-util/common.h>

CXX_GUARD_START

#define GBA_MATRIX_MAPPINGS_MAX 16

struct GBAMatrix {
	uint32_t cmd;
	uint32_t paddr;
	uint32_t vaddr;
	uint32_t size;

	uint32_t mappings[GBA_MATRIX_MAPPINGS_MAX];
};

struct GBA;
void GBAMatrixReset(struct GBA*);
void GBAMatrixWrite(struct GBA*, uint32_t address, uint32_t value);
void GBAMatrixWrite16(struct GBA*, uint32_t address, uint16_t value);

struct GBASerializedState;
void GBAMatrixSerialize(const struct GBA* memory, struct GBASerializedState* state);
void GBAMatrixDeserialize(struct GBA* memory, const struct GBASerializedState* state);

CXX_GUARD_END

#endif
