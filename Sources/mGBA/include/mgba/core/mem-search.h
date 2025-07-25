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
/* Copyright (c) 2013-2017 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CORE_MEM_SEARCH_H
#define CORE_MEM_SEARCH_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba-util/vector.h>

enum mCoreMemorySearchType {
	mCORE_MEMORY_SEARCH_INT,
	mCORE_MEMORY_SEARCH_STRING,
	mCORE_MEMORY_SEARCH_GUESS,
};

enum mCoreMemorySearchOp {
	mCORE_MEMORY_SEARCH_EQUAL,
	mCORE_MEMORY_SEARCH_GREATER,
	mCORE_MEMORY_SEARCH_LESS,
	mCORE_MEMORY_SEARCH_ANY,
	mCORE_MEMORY_SEARCH_DELTA,
	mCORE_MEMORY_SEARCH_DELTA_POSITIVE,
	mCORE_MEMORY_SEARCH_DELTA_NEGATIVE,
	mCORE_MEMORY_SEARCH_DELTA_ANY,
};

struct mCoreMemorySearchParams {
	int memoryFlags;
	enum mCoreMemorySearchType type;
	enum mCoreMemorySearchOp op;
	int align;
	int width;
	union {
		const char* valueStr;
		int32_t valueInt;
	};
};

struct mCoreMemorySearchResult {
	uint32_t address;
	int segment;
	uint32_t guessDivisor;
	uint32_t guessMultiplier;
	enum mCoreMemorySearchType type;
	int width;
	int32_t oldValue;
};

DECLARE_VECTOR(mCoreMemorySearchResults, struct mCoreMemorySearchResult);

struct mCore;
void mCoreMemorySearch(struct mCore* core, const struct mCoreMemorySearchParams* params, struct mCoreMemorySearchResults* out, size_t limit);
void mCoreMemorySearchRepeat(struct mCore* core, const struct mCoreMemorySearchParams* params, struct mCoreMemorySearchResults* inout);

CXX_GUARD_END

#endif
