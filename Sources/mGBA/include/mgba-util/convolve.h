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
#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <mgba-util/common.h>

CXX_GUARD_START

struct ConvolutionKernel {
	float* kernel;
	size_t* dims;
	size_t rank;
};

void ConvolutionKernelCreate(struct ConvolutionKernel* kernel, size_t rank, size_t* dims);
void ConvolutionKernelDestroy(struct ConvolutionKernel* kernel);

void ConvolutionKernelFillRadial(struct ConvolutionKernel* kernel, bool normalize);
void ConvolutionKernelFillCircle(struct ConvolutionKernel* kernel, bool normalize);

// TODO: Make image container struct?
void Convolve1DPad0PackedS32(const int32_t* restrict src, int32_t* restrict dst, size_t length, const struct ConvolutionKernel* restrict kernel);

void Convolve2DClampPacked8(const uint8_t* restrict src, uint8_t* restrict dst, size_t width, size_t height, size_t stride, const struct ConvolutionKernel* restrict kernel);
void Convolve2DClampChannels8(const uint8_t* restrict src, uint8_t* restrict dst, size_t width, size_t height, size_t stride, size_t channels, const struct ConvolutionKernel* restrict kernel);

CXX_GUARD_END

#endif
