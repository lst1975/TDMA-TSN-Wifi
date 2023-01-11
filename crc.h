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

