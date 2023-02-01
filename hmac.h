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
#ifndef _CIPHER_HMAC_ALL_H
#define _CIPHER_HMAC_ALL_H
#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */

#define   SHA1_DIGEST_SIZE  20
#define SHA224_DIGEST_SIZE  28
#define SHA256_DIGEST_SIZE  32
#define SHA384_DIGEST_SIZE  48
#define SHA512_DIGEST_SIZE  64
#define    MD5_DIGEST_SIZE  16
    void hmac_md5(unsigned char *key, int key_len,
        unsigned char *text, int text_len, unsigned char *hmac);
    void hmac_sha1(unsigned char *key, int key_len,
        unsigned char *text, int text_len, unsigned char *hmac);
    void hmac_sha224(unsigned char *key, int key_len,
        unsigned char *text, int text_len, unsigned char *hmac);
    void hmac_sha256(unsigned char *key, int key_len,
        unsigned char *text, int text_len, unsigned char *hmac);
    void hmac_sha384(unsigned char *key, int key_len,
        unsigned char *text, int text_len, unsigned char *hmac);
    void hmac_sha512(unsigned char *key, int key_len,
        unsigned char *text, int text_len, unsigned char *hmac);

#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif /* _CIPHER_HMAC_ALL_H */
