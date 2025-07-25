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
/* Copyright (c) 2013-2015 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef N3DS_THREADING_H
#define N3DS_THREADING_H

#include <mgba-util/common.h>

#include <3ds.h>
#include <malloc.h>

#define THREAD_ENTRY void
#define THREAD_EXIT(RES) return
typedef ThreadFunc ThreadEntry;

typedef LightLock Mutex;
typedef CondVar Condition;

static inline int MutexInit(Mutex* mutex) {
	LightLock_Init(mutex);
	return 0;
}

static inline int MutexDeinit(Mutex* mutex) {
	UNUSED(mutex);
	return 0;
}

static inline int MutexLock(Mutex* mutex) {
	LightLock_Lock(mutex);
	return 0;
}

static inline int MutexTryLock(Mutex* mutex) {
	return LightLock_TryLock(mutex);
}

static inline int MutexUnlock(Mutex* mutex) {
	LightLock_Unlock(mutex);
	return 0;
}

static inline int ConditionInit(Condition* cond) {
	CondVar_Init(cond);
	return 0;
}

static inline int ConditionDeinit(Condition* cond) {
	UNUSED(cond);
	return 0;
}

static inline int ConditionWait(Condition* cond, Mutex* mutex) {
	CondVar_Wait(cond, mutex);
	return 0;
}

static inline int ConditionWaitTimed(Condition* cond, Mutex* mutex, int32_t timeoutMs) {
	return CondVar_WaitTimeout(cond, mutex, timeoutMs * 10000000LL);
}

static inline int ConditionWake(Condition* cond) {
	CondVar_Signal(cond);
	return 0;
}

static inline int ThreadCreate(Thread* thread, ThreadEntry entry, void* context) {
	if (!entry || !thread) {
		return 1;
	}
	*thread = threadCreate(entry, context, 0x8000, 0x18, 2, false);
	return !*thread;
}

static inline int ThreadJoin(Thread* thread) {
	Result res = threadJoin(*thread, U64_MAX);
	threadFree(*thread);
	return res;
}

static inline void ThreadSetName(const char* name) {
	UNUSED(name);
	// Unimplemented
}

#endif
