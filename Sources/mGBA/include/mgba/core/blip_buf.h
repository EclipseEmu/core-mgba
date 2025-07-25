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
/** \file
Sample buffer that resamples from input clock rate to output sample rate */

/* blip_buf 1.1.0 */
#ifndef BLIP_BUF_H 
#define BLIP_BUF_H

#ifdef __cplusplus
	extern "C" {
#endif

/** First parameter of most functions is blip_t*, or const blip_t* if nothing
is changed. */
typedef struct blip_t blip_t;

/** Creates new buffer that can hold at most sample_count samples. Sets rates
so that there are blip_max_ratio clocks per sample. Returns pointer to new
buffer, or NULL if insufficient memory. */
blip_t* blip_new( int sample_count );

/** Sets approximate input clock rate and output sample rate. For every
clock_rate input clocks, approximately sample_rate samples are generated. */
void blip_set_rates( blip_t*, double clock_rate, double sample_rate );

enum { /** Maximum clock_rate/sample_rate ratio. For a given sample_rate,
clock_rate must not be greater than sample_rate*blip_max_ratio. */
blip_max_ratio = 0x100000 };

/** Clears entire buffer. Afterwards, blip_samples_avail() == 0. */
void blip_clear( blip_t* );

/** Adds positive/negative delta into buffer at specified clock time. */
void blip_add_delta( blip_t*, unsigned int clock_time, int delta );

/** Same as blip_add_delta(), but uses faster, lower-quality synthesis. */
void blip_add_delta_fast( blip_t*, unsigned int clock_time, int delta );

/** Length of time frame, in clocks, needed to make sample_count additional
samples available. */
int blip_clocks_needed( const blip_t*, int sample_count );

enum { /** Maximum number of samples that can be generated from one time frame. */
blip_max_frame = 4000 };

/** Makes input clocks before clock_duration available for reading as output
samples. Also begins new time frame at clock_duration, so that clock time 0 in
the new time frame specifies the same clock as clock_duration in the old time
frame specified. Deltas can have been added slightly past clock_duration (up to
however many clocks there are in two output samples). */
void blip_end_frame( blip_t*, unsigned int clock_duration );

/** Number of buffered samples available for reading. */
int blip_samples_avail( const blip_t* );

/** Reads and removes at most 'count' samples and writes them to 'out'. If
'stereo' is true, writes output to every other element of 'out', allowing easy
interleaving of two buffers into a stereo sample stream. Outputs 16-bit signed
samples. Returns number of samples actually read.  */
int blip_read_samples( blip_t*, short out [], int count, int stereo );

/** Frees buffer. No effect if NULL is passed. */
void blip_delete( blip_t* );


/* Deprecated */
typedef blip_t blip_buffer_t;

#ifdef __cplusplus
	}
#endif

#endif
