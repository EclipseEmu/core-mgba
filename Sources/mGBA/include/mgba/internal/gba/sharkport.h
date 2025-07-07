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
#ifndef GBA_SHARKPORT_H
#define GBA_SHARKPORT_H

#include <mgba-util/common.h>

CXX_GUARD_START

struct GBA;
struct VFile;

int GBASavedataSharkPortPayloadSize(struct VFile* vf);
void* GBASavedataSharkPortGetPayload(struct VFile* vf, size_t* size, uint8_t* header, bool testChecksum);
bool GBASavedataImportSharkPort(struct GBA* gba, struct VFile* vf, bool testChecksum);
bool GBASavedataExportSharkPort(const struct GBA* gba, struct VFile* vf);

int GBASavedataGSVPayloadSize(struct VFile* vf);
void* GBASavedataGSVGetPayload(struct VFile* vf, size_t* size, uint8_t* ident, bool testChecksum);
bool GBASavedataImportGSV(struct GBA* gba, struct VFile* vf, bool testChecksum);

CXX_GUARD_END

#endif
