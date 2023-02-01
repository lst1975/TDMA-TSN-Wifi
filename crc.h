/**************************************************************************************
 *               TDMA Time-Sensitive-Network Wifi V1.0.1
 * Copyright (C) 2022 Songtao Liu, 980680431@qq.com.  All Rights Reserved.
 **************************************************************************************
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN ALL
 * COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. WHAT'S MORE, A DECLARATION OF 
 * NGRTOS MUST BE DISPLAYED IN THE FINAL SOFTWARE OR PRODUCT RELEASE. NGRTOS HAS 
 * NOT ANY LIMITATION OF CONTRIBUTIONS TO IT, WITHOUT ANY LIMITATION OF CODING STYLE, 
 * DRIVERS, CORE, APPLICATIONS, LIBRARIES, TOOLS, AND ETC. ANY LICENSE IS PERMITTED 
 * UNDER THE ABOVE LICENSE. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF 
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO 
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES 
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *
 **************************************************************************************
 *                              
 *                    https://github.com/lst1975/TDMA-TSN-Wifi
 *                              
 **************************************************************************************
 */
#ifndef __CRC_H__
#define __CRC_H__

#define _mm_prefetch(P, I) \
  __builtin_prefetch ((P), 0, (I))

#define PREFETCH(location) __builtin_prefetch(location)

//#define CRC32_WORD   // Lookup table consumes 1KB
//#define CRC32_DWORD  // Lookup table consumes 2KB
#define CRC32_QWORD    // Lookup table consumes 4KB

#ifdef CRC32_WORD
uint32_t crc32_4bytes(const void *data, size_t length);
#endif

#ifdef CRC32_DWORD
uint32_t crc32_8bytes(const void *data, size_t length);
uint32_t crc32_4x8bytes(const void *data, size_t length);
#endif

#ifdef CRC32_QWORD
uint32_t crc32_16bytes(const void *data, size_t length);
uint32_t crc32_16bytes_prefetch(const void *data, size_t length,
                                size_t prefetchAhead);
#endif

uint32_t crc32_fast(const void *data, size_t length);

uint16_t crc16(const char *buf, int len);
uint32_t crc32(const char *buf, int len);
uint64_t crc64(const char *buf, int len);

tsn_boolean_e tsn_crc_init(void);
uint64_t tsn_get_fcs(char *p);
int tsn_set_fcs(char *p, uint64_t fcs);
uint64_t tsn_calc_fcs(char *p, int length);
tsn_boolean_e tsn_check_fcs(char *p, int length, uint64_t fcs);

#endif

