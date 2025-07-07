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
#ifndef M_LIBRARY_H
#define M_LIBRARY_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/core.h>
#include <mgba-util/vector.h>

struct mLibraryEntry {
	const char* base;
	const char* filename;
	const char* title;
	char internalTitle[17];
	char internalCode[9];
	enum mPlatform platform;
	size_t filesize;
	uint32_t crc32;
};

#ifdef USE_SQLITE3

DECLARE_VECTOR(mLibraryListing, struct mLibraryEntry);

struct mLibrary;
struct mLibrary* mLibraryCreateEmpty(void);
struct mLibrary* mLibraryLoad(const char* filename);
void mLibraryDestroy(struct mLibrary*);

struct VDir;
struct VFile;
void mLibraryLoadDirectory(struct mLibrary* library, const char* base, bool recursive);
void mLibraryClear(struct mLibrary* library);

size_t mLibraryCount(struct mLibrary* library, const struct mLibraryEntry* constraints);
size_t mLibraryGetEntries(struct mLibrary* library, struct mLibraryListing* out, size_t numEntries, size_t offset, const struct mLibraryEntry* constraints);
void mLibraryEntryFree(struct mLibraryEntry* entry);
struct VFile* mLibraryOpenVFile(struct mLibrary* library, const struct mLibraryEntry* entry);

struct NoIntroDB;
void mLibraryAttachGameDB(struct mLibrary* library, const struct NoIntroDB* db);

#endif

CXX_GUARD_END

#endif
