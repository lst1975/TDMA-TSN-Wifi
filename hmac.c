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
#include "hmac.h"
#include "md5.h"
#include "sha.h"
#include "sha1.h"
#include "sha224.h"
#include "sha256.h"
#include "sha384.h"
#include "sha512.h"

/*
unsigned char*  text;        pointer to data stream
int       text_len;      length of data stream
unsigned char*  key;         pointer to authentication key
int       key_len;       length of authentication key
unsigned char*  digest;        caller digest to be filled in
*/
#define KEY_IOPAD_SIZE 64
void hmac_md5(unsigned char *key, int key_len,
  unsigned char *text, int text_len, unsigned char *hmac)
{
  MD5_CTX context;
  unsigned char k_ipad[KEY_IOPAD_SIZE];  /* inner padding - key XORd with ipad  */
  unsigned char k_opad[KEY_IOPAD_SIZE];  /* outer padding - key XORd with opad */
  int i;

  /*
  * the HMAC_MD5 transform looks like:
  *
  * MD5(K XOR opad, MD5(K XOR ipad, text))
  *
  * where K is an n byte key
  * ipad is the byte 0x36 repeated 64 times

  * opad is the byte 0x5c repeated 64 times
  * and text is the data being protected
  */

  /* start out by storing key in pads */
  tsn_memset( k_ipad, 0, sizeof(k_ipad));
  tsn_memset( k_opad, 0, sizeof(k_opad));
  if(key_len <= KEY_IOPAD_SIZE)
  {
    tsn_memcpy( k_ipad, key, key_len);
    tsn_memcpy( k_opad, key, key_len);
  }
  else
  {
    MD5Calc(key, key_len, k_ipad);
    tsn_memcpy(k_opad, k_ipad, KEY_IOPAD_SIZE);
  }
  
  /* XOR key with ipad and opad values */
  for (i = 0; i < KEY_IOPAD_SIZE; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }
  
  // perform inner MD5
  MD5Init(&context);          /* init context for 1st pass */
  MD5Update(&context, k_ipad, KEY_IOPAD_SIZE);    /* start with inner pad */
  MD5Update(&context, (unsigned char*)text, text_len); /* then text of datagram */
  MD5Final(hmac, &context);       /* finish up 1st pass */
  
  // perform outer MD5
  MD5Init(&context);           /* init context for 2nd pass */
  MD5Update(&context, k_opad, KEY_IOPAD_SIZE);   /* start with outer pad */
  MD5Update(&context, hmac, MD5_DIGEST_SIZE);   /* then results of 1st hash */
  MD5Final(hmac, &context);      /* finish up 2nd pass */
}

void hmac_sha1(unsigned char *key, int key_len,
  unsigned char *text, int text_len, unsigned char *hmac) {
  SHA_State context;
  unsigned char k_ipad[KEY_IOPAD_SIZE];  /* inner padding - key XORd with ipad  */
  unsigned char k_opad[KEY_IOPAD_SIZE];  /* outer padding - key XORd with opad */
  int i;

  /* start out by storing key in pads */
  tsn_memset(k_ipad, 0, sizeof(k_ipad));
  tsn_memset(k_opad, 0, sizeof(k_opad));
  if(key_len <= KEY_IOPAD_SIZE)
  {
    tsn_memcpy( k_ipad, key, key_len);
    tsn_memcpy( k_opad, key, key_len);
  }
  else
  {
    SHA1Calc(key, key_len, k_ipad);
    tsn_memcpy(k_opad, k_ipad, KEY_IOPAD_SIZE);
  }

  /* XOR key with ipad and opad values */
  for (i = 0; i < KEY_IOPAD_SIZE; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  // perform inner SHA
  SHA_Init(&context);          /* init context for 1st pass */
  SHA_Bytes(&context, k_ipad, KEY_IOPAD_SIZE);    /* start with inner pad */
  SHA_Bytes(&context, text, text_len); /* then text of datagram */
  SHA_Final(&context, hmac);       /* finish up 1st pass */

  // perform outer SHA
  SHA_Init(&context);           /* init context for 2nd pass */
  SHA_Bytes(&context, k_opad, KEY_IOPAD_SIZE);   /* start with outer pad */
  SHA_Bytes(&context, hmac, SHA1_DIGEST_SIZE);   /* then results of 1st hash */
  SHA_Final(&context, hmac);      /* finish up 2nd pass */
}

void hmac_sha224(unsigned char *key, int key_len,
  unsigned char *text, int text_len, unsigned char *hmac) {
  SHA256_State context;
  unsigned char k_ipad[KEY_IOPAD_SIZE];  /* inner padding - key XORd with ipad  */
  unsigned char k_opad[KEY_IOPAD_SIZE];  /* outer padding - key XORd with opad */
  int i;

  /* start out by storing key in pads */
  tsn_memset(k_ipad, 0, sizeof(k_ipad));
  tsn_memset(k_opad, 0, sizeof(k_opad));
  if(key_len <= KEY_IOPAD_SIZE)
  {
    tsn_memcpy( k_ipad, key, key_len);
    tsn_memcpy( k_opad, key, key_len);
  }
  else
  {
    SHA224_Simple(key, key_len, k_ipad);
    tsn_memcpy(k_opad, k_ipad, KEY_IOPAD_SIZE);
  }

  /* XOR key with ipad and opad values */
  for (i = 0; i < KEY_IOPAD_SIZE; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  // perform inner SHA224
  SHA224_Init(&context);          /* init context for 1st pass */
  SHA224_Bytes(&context, k_ipad, KEY_IOPAD_SIZE);    /* start with inner pad */
  SHA224_Bytes(&context, (unsigned char*)text, text_len); /* then text of datagram */
  SHA224_Final(&context, hmac);       /* finish up 1st pass */

  // perform outer SHA224
  SHA224_Init(&context);           /* init context for 2nd pass */
  SHA224_Bytes(&context, k_opad, KEY_IOPAD_SIZE);   /* start with outer pad */
  SHA224_Bytes(&context, hmac, SHA224_DIGEST_SIZE);   /* then results of 1st hash */
  SHA224_Final(&context, hmac);      /* finish up 2nd pass */
}

void hmac_sha256(unsigned char *key, int key_len,
  unsigned char *text, int text_len, unsigned char *hmac) {
  SHA256_State context;
  unsigned char k_ipad[KEY_IOPAD_SIZE];  /* inner padding - key XORd with ipad  */
  unsigned char k_opad[KEY_IOPAD_SIZE];  /* outer padding - key XORd with opad */
  int i;

  /* start out by storing key in pads */
  tsn_memset(k_ipad, 0, sizeof(k_ipad));
  tsn_memset(k_opad, 0, sizeof(k_opad));
  if(key_len <= KEY_IOPAD_SIZE)
  {
    tsn_memcpy( k_ipad, key, key_len);
    tsn_memcpy( k_opad, key, key_len);
  }
  else
  {
    SHA256_Simple(key, key_len, k_ipad);
    tsn_memcpy(k_opad, k_ipad, KEY_IOPAD_SIZE);
  }

  /* XOR key with ipad and opad values */
  for (i = 0; i < KEY_IOPAD_SIZE; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  // perform inner SHA256
  SHA256_Init(&context);          /* init context for 1st pass */
  SHA256_Bytes(&context, k_ipad, KEY_IOPAD_SIZE);    /* start with inner pad */
  SHA256_Bytes(&context, text, text_len); /* then text of datagram */
  SHA256_Final(&context, hmac);       /* finish up 1st pass */

  // perform outer SHA256
  SHA256_Init(&context);           /* init context for 2nd pass */
  SHA256_Bytes(&context, k_opad, KEY_IOPAD_SIZE);   /* start with outer pad */
  SHA256_Bytes(&context, hmac, SHA256_DIGEST_SIZE);   /* then results of 1st hash */
  SHA256_Final(&context, hmac);      /* finish up 2nd pass */
}

#undef KEY_IOPAD_SIZE
#define KEY_IOPAD_SIZE 128
void hmac_sha384(unsigned char *key, int key_len,
  unsigned char *text, int text_len, unsigned char *hmac) {
  SHA512_State context;
  unsigned char k_ipad[KEY_IOPAD_SIZE];  /* inner padding - key XORd with ipad  */
  unsigned char k_opad[KEY_IOPAD_SIZE];  /* outer padding - key XORd with opad */
  int i;

  /* start out by storing key in pads */
  tsn_memset(k_ipad, 0, sizeof(k_ipad));
  tsn_memset(k_opad, 0, sizeof(k_opad));
  if(key_len <= KEY_IOPAD_SIZE)
  {
    tsn_memcpy( k_ipad, key, key_len);
    tsn_memcpy( k_opad, key, key_len);
  }
  else
  {
    SHA384_Simple(key, key_len, k_ipad);
    tsn_memcpy(k_opad, k_ipad, KEY_IOPAD_SIZE);
  }

  /* XOR key with ipad and opad values */
  for (i = 0; i < KEY_IOPAD_SIZE; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  // perform inner SHA384
  SHA384_Init(&context);          /* init context for 1st pass */
  SHA384_Bytes(&context, k_ipad, KEY_IOPAD_SIZE);    /* start with inner pad */
  SHA384_Bytes(&context, text, text_len); /* then text of datagram */
  SHA384_Final(&context, hmac);       /* finish up 1st pass */

  // perform outer SHA384
  SHA384_Init(&context);           /* init context for 2nd pass */
  SHA384_Bytes(&context, k_opad, KEY_IOPAD_SIZE);   /* start with outer pad */
  SHA384_Bytes(&context, hmac, SHA384_DIGEST_SIZE);   /* then results of 1st hash */
  SHA384_Final(&context, hmac);      /* finish up 2nd pass */
}

void hmac_sha512(unsigned char *key, int key_len,
  unsigned char *text, int text_len, unsigned char *hmac) {
  SHA512_State context;
  unsigned char k_ipad[KEY_IOPAD_SIZE];  /* inner padding - key XORd with ipad  */
  unsigned char k_opad[KEY_IOPAD_SIZE];  /* outer padding - key XORd with opad */
  int i;

  /* start out by storing key in pads */
  tsn_memset(k_ipad, 0, sizeof(k_ipad));
  tsn_memset(k_opad, 0, sizeof(k_opad));
  if(key_len <= KEY_IOPAD_SIZE)
  {
    tsn_memcpy( k_ipad, key, key_len);
    tsn_memcpy( k_opad, key, key_len);
  }
  else
  {
    SHA512_Simple(key, key_len, k_ipad);
    tsn_memcpy(k_opad, k_ipad, KEY_IOPAD_SIZE);
  }

  /* XOR key with ipad and opad values */
  for (i = 0; i < KEY_IOPAD_SIZE; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  // perform inner SHA512
  SHA512_Init(&context);          /* init context for 1st pass */
  SHA512_Bytes(&context, k_ipad, KEY_IOPAD_SIZE);    /* start with inner pad */
  SHA512_Bytes(&context, text, text_len); /* then text of datagram */
  SHA512_Final(&context, hmac);       /* fnish up 1st pass */

  // perform outer SHA512
  SHA512_Init(&context);           /* init context for 2nd pass */
  SHA512_Bytes(&context, k_opad, KEY_IOPAD_SIZE);   /* start with outer pad */
  SHA512_Bytes(&context, hmac, SHA512_DIGEST_SIZE);   /* then results of 1st hash */
  SHA512_Final(&context, hmac);      /* finish up 2nd pass */
}

