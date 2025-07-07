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
#ifndef M_CORE_REWIND_H
#define M_CORE_REWIND_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba-util/vector.h>
#ifndef DISABLE_THREADING
#include <mgba-util/threading.h>
#endif

DECLARE_VECTOR(mCoreRewindPatches, struct PatchFast);

struct VFile;
struct mCoreRewindContext {
	struct mCoreRewindPatches patchMemory;
	size_t current;
	size_t size;
	struct VFile* previousState;
	struct VFile* currentState;

#ifndef DISABLE_THREADING
	bool onThread;
	Thread thread;
	Condition cond;
	Mutex mutex;
	bool ready;
#endif
};

void mCoreRewindContextInit(struct mCoreRewindContext*, size_t entries, bool onThread);
void mCoreRewindContextDeinit(struct mCoreRewindContext*);

struct mCore;
void mCoreRewindAppend(struct mCoreRewindContext*, struct mCore*);
bool mCoreRewindRestore(struct mCoreRewindContext*, struct mCore*);

CXX_GUARD_END

#endif
