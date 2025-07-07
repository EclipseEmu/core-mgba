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
#ifndef M_UPDATER_H
#define M_UPDATER_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/config.h>
#include <mgba-util/configuration.h>

struct StringList;
struct Table;

struct mUpdaterContext {
	struct Configuration manifest;
};

struct mUpdate {
	const char* path;
	size_t size;
	int rev;
	const char* version;
	const char* commit;
	const char* sha256;
};

bool mUpdaterInit(struct mUpdaterContext*, const char* manifest);
void mUpdaterDeinit(struct mUpdaterContext*);
void mUpdaterGetPlatforms(const struct mUpdaterContext*, struct StringList* out);
void mUpdaterGetUpdates(const struct mUpdaterContext*, const char* platform, struct Table* out);
void mUpdaterGetUpdateForChannel(const struct mUpdaterContext*, const char* platform, const char* channel, struct mUpdate* out);
const char* mUpdaterGetBucket(const struct mUpdaterContext*);
void mUpdateRecord(struct mCoreConfig*, const char* prefix, const struct mUpdate*);
bool mUpdateLoad(const struct mCoreConfig*, const char* prefix, struct mUpdate*);

void mUpdateRegister(struct mCoreConfig*, const char* arg0, const char* updatePath);
void mUpdateDeregister(struct mCoreConfig*);

const char* mUpdateGetRoot(const struct mCoreConfig*);
const char* mUpdateGetCommand(const struct mCoreConfig*);
const char* mUpdateGetArchiveExtension(const struct mCoreConfig*);
bool mUpdateGetArchivePath(const struct mCoreConfig*, char* out, size_t outLength);

CXX_GUARD_END

#endif
