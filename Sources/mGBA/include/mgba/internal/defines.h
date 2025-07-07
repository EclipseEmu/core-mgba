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
/* Copyright (c) 2013-2022 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef M_INTERNAL_DEFINES_H
#define M_INTERNAL_DEFINES_H

#define mSAVEDATA_CLEANUP_THRESHOLD 15

enum {
    mSAVEDATA_DIRT_NONE = 0,
	mSAVEDATA_DIRT_NEW = 1,
	mSAVEDATA_DIRT_SEEN = 2,
};

static inline bool mSavedataClean(int* dirty, uint32_t* dirtAge, uint32_t frameCount) {
    if (*dirty & mSAVEDATA_DIRT_NEW) {
        *dirtAge = frameCount;
        *dirty &= ~mSAVEDATA_DIRT_NEW;
        if (!(*dirty & mSAVEDATA_DIRT_SEEN)) {
            *dirty |= mSAVEDATA_DIRT_SEEN;
        }
    } else if ((*dirty & mSAVEDATA_DIRT_SEEN) && frameCount - *dirtAge > mSAVEDATA_CLEANUP_THRESHOLD) {
        *dirty = 0;
        return true;
    }
    return false;
}

#endif
