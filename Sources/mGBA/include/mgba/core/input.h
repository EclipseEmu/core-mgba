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
#ifndef M_INPUT_H
#define M_INPUT_H

#include <mgba-util/common.h>

CXX_GUARD_START

struct Configuration;

enum mInputHat {
	M_INPUT_HAT_NEUTRAL = 0,
	M_INPUT_HAT_UP = 1,
	M_INPUT_HAT_RIGHT = 2,
	M_INPUT_HAT_DOWN = 4,
	M_INPUT_HAT_LEFT = 8
};

struct mInputHatBindings {
	int up;
	int right;
	int down;
	int left;
};

struct mInputPlatformInfo {
	const char* platformName;
	const char** keyId;
	size_t nKeys;
	struct mInputHatBindings hat;
};

struct mInputMap {
	struct mInputMapImpl* maps;
	size_t numMaps;
	const struct mInputPlatformInfo* info;
};

struct mInputAxis {
	int highDirection;
	int lowDirection;
	int32_t deadHigh;
	int32_t deadLow;
};

void mInputMapInit(struct mInputMap*, const struct mInputPlatformInfo* info);
void mInputMapDeinit(struct mInputMap*);

int mInputMapKey(const struct mInputMap*, uint32_t type, int key);
int mInputMapKeyBits(const struct mInputMap* map, uint32_t type, uint32_t bits, unsigned offset);
void mInputBindKey(struct mInputMap*, uint32_t type, int key, int input);
int mInputQueryBinding(const struct mInputMap*, uint32_t type, int input);
void mInputUnbindKey(struct mInputMap*, uint32_t type, int input);

int mInputMapAxis(const struct mInputMap*, uint32_t type, int axis, int value);
int mInputClearAxis(const struct mInputMap*, uint32_t type, int axis, int keys);
void mInputBindAxis(struct mInputMap*, uint32_t type, int axis, const struct mInputAxis* description);
void mInputUnbindAxis(struct mInputMap*, uint32_t type, int axis);
void mInputUnbindAllAxes(struct mInputMap*, uint32_t type);
const struct mInputAxis* mInputQueryAxis(const struct mInputMap*, uint32_t type, int axis);
void mInputEnumerateAxes(const struct mInputMap*, uint32_t type, void (handler(int axis, const struct mInputAxis* description, void* user)), void* user);

int mInputMapHat(const struct mInputMap*, uint32_t type, int id, int direction);
void mInputBindHat(struct mInputMap*, uint32_t type, int id, const struct mInputHatBindings* bindings);
bool mInputQueryHat(const struct mInputMap*, uint32_t type, int id, struct mInputHatBindings* bindings);
void mInputUnbindHat(struct mInputMap*, uint32_t type, int id);
void mInputUnbindAllHats(struct mInputMap*, uint32_t type);

bool mInputMapLoad(struct mInputMap*, uint32_t type, const struct Configuration*);
void mInputMapSave(const struct mInputMap*, uint32_t type, struct Configuration*);

bool mInputProfileLoad(struct mInputMap*, uint32_t type, const struct Configuration*, const char* profile);
void mInputProfileSave(const struct mInputMap*, uint32_t type, struct Configuration*, const char* profile);

const char* mInputGetPreferredDevice(const struct Configuration*, const char* platformName, uint32_t type, int playerId);
void mInputSetPreferredDevice(struct Configuration*, const char* platformName, uint32_t type, int playerId, const char* deviceName);

const char* mInputGetCustomValue(const struct Configuration* config, const char* platformName, uint32_t type, const char* key,
                                 const char* profile);
void mInputSetCustomValue(struct Configuration* config, const char* platformName, uint32_t type, const char* key, const char* value,
                          const char* profile);

CXX_GUARD_END

#endif
