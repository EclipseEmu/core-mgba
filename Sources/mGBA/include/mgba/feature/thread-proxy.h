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
#ifndef VIDEO_THREAD_PROXY_H
#define VIDEO_THREAD_PROXY_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/internal/gba/video.h>
#include "video-logger.h"
#include <mgba-util/threading.h>
#include <mgba-util/ring-fifo.h>

enum mVideoThreadProxyState {
	PROXY_THREAD_STOPPED = 0,
	PROXY_THREAD_IDLE,
	PROXY_THREAD_BUSY
};

struct mVideoThreadProxy {
	struct mVideoLogger d;

	Thread thread;
	Condition fromThreadCond;
	Condition toThreadCond;
	Mutex mutex;
	enum mVideoThreadProxyState threadState;
	enum mVideoLoggerEvent event;

	struct RingFIFO dirtyQueue;
};

void mVideoThreadProxyCreate(struct mVideoThreadProxy* renderer);

CXX_GUARD_END

#endif
