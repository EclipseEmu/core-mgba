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
#ifndef M_CPU_H
#define M_CPU_H

#include <mgba-util/common.h>

CXX_GUARD_START

enum mCPUComponentType {
	CPU_COMPONENT_DEBUGGER,
	CPU_COMPONENT_CHEAT_DEVICE,
	CPU_COMPONENT_MISC_1,
	CPU_COMPONENT_MISC_2,
	CPU_COMPONENT_MISC_3,
	CPU_COMPONENT_MISC_4,
	CPU_COMPONENT_MAX
};

struct mCPUComponent {
	uint32_t id;
	void (*init)(void* cpu, struct mCPUComponent* component);
	void (*deinit)(struct mCPUComponent* component);
};

CXX_GUARD_END

#endif
