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
#ifndef PNG_IO_H
#define PNG_IO_H

#include <mgba-util/common.h>

CXX_GUARD_START

#ifdef USE_PNG

// png.h defines its own version of restrict which conflicts with mGBA's.
#ifdef restrict
#undef restrict
#endif
#include <png.h>

struct VFile;

enum {
	PNG_HEADER_BYTES = 8
};

png_structp PNGWriteOpen(struct VFile* source);
png_infop PNGWriteHeader(png_structp png, unsigned width, unsigned height);
png_infop PNGWriteHeaderA(png_structp png, unsigned width, unsigned height);
png_infop PNGWriteHeader8(png_structp png, unsigned width, unsigned height);
bool PNGWritePalette(png_structp png, png_infop info, const uint32_t* palette, unsigned entries);
bool PNGWritePixels(png_structp png, unsigned width, unsigned height, unsigned stride, const void* pixels);
bool PNGWritePixelsA(png_structp png, unsigned width, unsigned height, unsigned stride, const void* pixels);
bool PNGWritePixels8(png_structp png, unsigned width, unsigned height, unsigned stride, const void* pixels);
bool PNGWriteCustomChunk(png_structp png, const char* name, size_t size, void* data);
void PNGWriteClose(png_structp png, png_infop info);

typedef int (*ChunkHandler)(png_structp, png_unknown_chunkp);

bool isPNG(struct VFile* source);
png_structp PNGReadOpen(struct VFile* source, unsigned offset);
bool PNGInstallChunkHandler(png_structp png, void* context, ChunkHandler handler, const char* chunkName);
bool PNGReadHeader(png_structp png, png_infop info);
bool PNGReadPixels(png_structp png, png_infop info, void* pixels, unsigned width, unsigned height, unsigned stride);
bool PNGReadPixelsA(png_structp png, png_infop info, void* pixels, unsigned width, unsigned height, unsigned stride);
bool PNGReadPixels8(png_structp png, png_infop info, void* pixels, unsigned width, unsigned height, unsigned stride);
bool PNGIgnorePixels(png_structp png, png_infop info);
bool PNGReadFooter(png_structp png, png_infop end);
void PNGReadClose(png_structp png, png_infop info, png_infop end);

#endif

CXX_GUARD_END

#endif
