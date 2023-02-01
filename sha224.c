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
#include <string.h>
#include "sha224.h"

void SHA224_Core_Init(SHA256_State *s) {
  s->h[0] = 0xc1059ed8ul;
  s->h[1] = 0x367cd507ul;
  s->h[2] = 0x3070dd17ul;
  s->h[3] = 0xf70e5939ul;
  s->h[4] = 0xffc00b31ul;
  s->h[5] = 0x68581511ul;
  s->h[6] = 0x64f98fa7ul;
  s->h[7] = 0xbefa4fa4ul;
}

#define SHA224_DIGEST_SIZE  28

void SHA224_Init(SHA256_State *s) {
  SHA224_Core_Init(s);
  s->blkused = 0;
  s->lenhi = s->lenlo = 0;
}

void SHA224_Bytes(SHA256_State *s, const void *p, int len) {
  SHA256_Bytes(s, p, len);
}

void SHA224_Final(SHA256_State *s, unsigned char *digest) {
  SHA256_Final(s, digest);
  digest[SHA224_DIGEST_SIZE] = 0x00;
}

void SHA224_Simple(const void *p, int len, unsigned char *output) {
  SHA256_State s;

  SHA224_Init(&s);
  SHA224_Bytes(&s, p, len);
  SHA224_Final(&s, output);
}
