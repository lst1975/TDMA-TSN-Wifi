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
/* taken from RFC-1321/Appendices A.1/A.2 */

/* PROTOTYPES should be set to one if and only if the compiler supports
function argument prototyping.
The following makes PROTOTYPES default to 0 if it has not already
been defined with C compiler flags.
*/
#ifndef _SYS_MD5_H_
#define _SYS_MD5_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

  typedef unsigned int UINT4;
  typedef unsigned char UCHAR;

  /* POINTER defines a generic pointer type */
  typedef unsigned char *POINTER;

  /* UINT2 defines a two byte word */
  typedef unsigned short int UINT2;

  /* UINT4 defines a four byte word */
  /* typedef unsigned long int UINT4; */

  /* MD5.H - header file for MD5C.C
  */

  /* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
  rights reserved.
  License to copy and use this software is granted provided that it
  is identified as the "RSA Data Security, Inc. MD5 Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  License is also granted to make and use derivative works provided
  that such works are identified as "derived from the RSA Data
  Security, Inc. MD5 Message-Digest Algorithm" in all material
  mentioning or referencing the derived work.
  RSA Data Security, Inc. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
  These notices must be retained in any copies of any part of this
  documentation and/or software.
  */

  /* MD5 context. */
  typedef struct {
    UINT4 state[4];                   /* state (ABCD) */
    UINT4 count[2];    /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];             /* input buffer */
  } MD5_CTX;

  void MD5Init(MD5_CTX *);
  void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
  void MD5Final (unsigned char [16], MD5_CTX *);

  // The function to calculate the message digest string
  // of a given string based on the MD5 algrithm.
  void MD5Calc(const unsigned char *input, unsigned int inlen, unsigned char *output);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _SYS_MD5_H_ */
