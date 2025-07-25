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
#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <mgba-util/common.h>

CXX_GUARD_START

#if !defined(MINIMAL_CORE) || MINIMAL_CORE < 2
struct VDir;

struct mDirectorySet {
	char baseName[PATH_MAX];
	struct VDir* base;
	struct VDir* archive;
	struct VDir* save;
	struct VDir* patch;
	struct VDir* state;
	struct VDir* screenshot;
	struct VDir* cheats;
};

void mDirectorySetInit(struct mDirectorySet* dirs);
void mDirectorySetDeinit(struct mDirectorySet* dirs);

void mDirectorySetAttachBase(struct mDirectorySet* dirs, struct VDir* base);
void mDirectorySetDetachBase(struct mDirectorySet* dirs);

struct VFile* mDirectorySetOpenPath(struct mDirectorySet* dirs, const char* path, bool (*filter)(struct VFile*));
struct VFile* mDirectorySetOpenSuffix(struct mDirectorySet* dirs, struct VDir* dir, const char* suffix, int mode);

struct mCoreOptions;
void mDirectorySetMapOptions(struct mDirectorySet* dirs, const struct mCoreOptions* opts);
#endif

CXX_GUARD_END

#endif
