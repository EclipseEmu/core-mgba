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
#ifndef FORMATTING_H
#define FORMATTING_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include "locale.h"

#ifdef HAVE_XLOCALE
#include <xlocale.h>
#elif !defined(HAVE_LOCALE)
typedef const char* locale_t;
#endif

int ftostr_l(char* restrict str, size_t size, float f, locale_t locale);

#ifndef HAVE_STRTOF_L
float strtof_l(const char* restrict str, char** restrict end, locale_t locale);
#endif

int ftostr_u(char* restrict str, size_t size, float f);
float strtof_u(const char* restrict str, char** restrict end);

#ifndef HAVE_LOCALTIME_R
struct tm* localtime_r(const time_t* timep, struct tm* result);
#endif

CXX_GUARD_END

#endif
