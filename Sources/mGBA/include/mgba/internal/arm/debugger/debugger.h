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
/* Copyright (c) 2013-2014 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef ARM_DEBUGGER_H
#define ARM_DEBUGGER_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/debugger/debugger.h>

#include <mgba/internal/arm/arm.h>
#include <mgba-util/vector.h>

struct ParseTree;
struct ARMDebugBreakpoint {
	struct mBreakpoint d;
	struct {
		uint32_t opcode;
		enum ExecutionMode mode;
	} sw;
};

DECLARE_VECTOR(ARMDebugBreakpointList, struct ARMDebugBreakpoint);

struct ARMDebugger {
	struct mDebuggerPlatform d;
	struct ARMCore* cpu;

	struct ARMDebugBreakpointList breakpoints;
	struct ARMDebugBreakpointList swBreakpoints;
	struct mWatchpointList watchpoints;
	struct ARMMemory originalMemory;

	ssize_t nextId;
	uint32_t stackTraceMode;

	void (*entered)(struct mDebugger*, enum mDebuggerEntryReason, struct mDebuggerEntryInfo*);

	bool (*setSoftwareBreakpoint)(struct ARMDebugger*, uint32_t address, enum ExecutionMode mode, uint32_t* opcode);
	void (*clearSoftwareBreakpoint)(struct ARMDebugger*, const struct ARMDebugBreakpoint*);
};

struct mDebuggerPlatform* ARMDebuggerPlatformCreate(void);
ssize_t ARMDebuggerSetSoftwareBreakpoint(struct mDebuggerPlatform* debugger, uint32_t address, enum ExecutionMode mode);

CXX_GUARD_END

#endif
