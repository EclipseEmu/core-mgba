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
#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include <mgba-util/common.h>

CXX_GUARD_START

#ifndef HAVE_POPCOUNT32
static inline uint32_t popcount32(unsigned bits) {
	bits = bits - ((bits >> 1) & 0x55555555);
	bits = (bits & 0x33333333) + ((bits >> 2) & 0x33333333);
	return (((bits + (bits >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
}
#endif

static inline unsigned clz32(uint32_t bits) {
#if defined(__GNUC__) || __clang__
	if (!bits) {
		return 32;
	}
	return __builtin_clz(bits);
#else
	static const int table[256] = {
		8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	if (bits & 0xFF000000) {
		return table[bits >> 24];
	} else if (bits & 0x00FF0000) {
		return table[bits >> 16] + 8;
	} else if (bits & 0x0000FF00) {
		return table[bits >> 8] + 16;
	}
	return table[bits] + 24;
#endif
}

static inline uint32_t toPow2(uint32_t bits) {
	if (!bits) {
		return 0;
	}
	unsigned lz = clz32(bits - 1);
	return 1 << (32 - lz);
}

static inline int reduceFraction(int* num, int* den) {
	int n = *num;
	int d = *den;
	while (d != 0) {
		int temp = n % d;
		n = d;
		d = temp;
	}
	*num /= n;
	*den /= n;
	return n;
}

#define TYPE_GENERICIZE(MACRO) \
	MACRO(int, Int) \
	MACRO(unsigned, UInt)

#define LOCK_ASPECT_RATIO(T, t) \
	static inline void lockAspectRatio ## t(T refW, T refH, T* w, T* h) { \
		if (*w * refH > *h * refW) { \
			*w = *h * refW / refH; \
		} else if (*w * refH < *h * refW) { \
			*h = *w * refH / refW; \
		} \
	}

TYPE_GENERICIZE(LOCK_ASPECT_RATIO)
#undef LOCK_ASPECT_RATIO

#define LOCK_INTEGER_RATIO(T, t) \
	static inline void lockIntegerRatio ## t(T ref, T* val) { \
		if (*val >= ref) { \
			*val -= *val % ref; \
		} \
	}

TYPE_GENERICIZE(LOCK_INTEGER_RATIO)
#undef LOCK_INTEGER_RATIO

#undef TYPE_GENERICIZE
CXX_GUARD_END

#endif
