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
#include <assert.h>
#include "aes.h"

static const uint8_t S_BOX[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
  0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
  0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
  0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
  0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
  0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
  0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
  0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
  0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
  0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
  0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
  0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
  0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
  0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
  0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
  0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
  0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t INV_S_BOX[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
  0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
  0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
  0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
  0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
  0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
  0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
  0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
  0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
  0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
  0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
  0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
  0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
  0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
  0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
  0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
  0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const uint8_t MIX[4][4] = {
  {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01},
  {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02}
};

static const uint8_t INV_MIX[4][4] = {
  {0x0e, 0x0b, 0x0d, 0x09}, {0x09, 0x0e, 0x0b, 0x0d},
  {0x0d, 0x09, 0x0e, 0x0b}, {0x0b, 0x0d, 0x09, 0x0e}
};

static const uint32_t RCON[10] = {
  0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010,
  0x00000020, 0x00000040, 0x00000080, 0x0000001B, 0x00000036
};

#ifndef GET_UINT32
#define GET_UINT32(n, b, i) do {          \
  (n) = ((uint32_t)(b)[(i)    ]      )  \
    | ((uint32_t)(b)[(i) + 1] <<  8)  \
    | ((uint32_t)(b)[(i) + 2] << 16)  \
    | ((uint32_t)(b)[(i) + 3] << 24); \
} while(0)
#endif

#define ROTL8(x) (((x) << 24) | ((x) >> 8))
#define ROTL16(x) (((x) << 16) | ((x) >> 16))
#define ROTL24(x) (((x) << 8) | ((x) >> 24))

#define SUB_WORD(x) (((uint32_t)S_BOX[(x) & 0xFF]) \
  | ((uint32_t)S_BOX[((x) >>  8) & 0xFF] << 8)   \
  | ((uint32_t)S_BOX[((x) >> 16) & 0xFF] << 16)  \
  | ((uint32_t)S_BOX[((x) >> 24) & 0xFF] << 24))

static void transport(uint8_t state[BLOCK_SIZE])
{
  assert(state != NULL);
  uint8_t new_state[4][4];
  int r, c;
  for (r = 0; r < 4; ++r)
    for (c = 0; c < 4; ++c)
      new_state[r][c] = state[(c << 2) + r];
  memcpy(state, new_state, sizeof(new_state));
}

static void add_round_key(uint8_t state[BLOCK_SIZE], const uint8_t key[BLOCK_SIZE])
{
  assert(state != NULL);
  assert(key != NULL);
  int i;
  for (i = 0; i < BLOCK_SIZE; ++i)
    state[i] ^= key[i];
}

static void _sub_bytes(uint8_t state[BLOCK_SIZE], const uint8_t *box)
{
  assert(state != NULL);
  assert(box != NULL);
  int i;
  for (i = 0; i < BLOCK_SIZE; ++i)
    state[i] = box[state[i]];
}

#define sub_bytes(state) _sub_bytes(state, S_BOX)
#define inv_sub_bytes(state) _sub_bytes(state, INV_S_BOX)

#define _shift_rows(state, OP1, OP2, OP3) do {                  \
  transport(state);                                           \
  *(uint32_t *)(state + 4) = OP1(*(uint32_t *)(state + 4));   \
  *(uint32_t *)(state + 8) = OP2(*(uint32_t *)(state + 8));   \
  *(uint32_t *)(state + 12) = OP3(*(uint32_t *)(state + 12)); \
  transport(state);                                           \
} while(0)

#define shift_rows(state) _shift_rows(state, ROTL8, ROTL16, ROTL24)
#define inv_shift_rows(state) _shift_rows(state, ROTL24, ROTL16, ROTL8)

static uint8_t GF_256_multiply(uint8_t a, uint8_t b)
{
  uint8_t t[8] = { a };
  uint8_t ret = 0x00;
  int i = 0;
  for (i = 1; i < 8; ++i) {
    t[i] = t[i - 1] << 1;
    if (t[i - 1] & 0x80)
      t[i] ^= 0x1b;
  }
  for (i = 0; i < 8; ++i)
    ret ^= (((b >> i) & 0x01) * t[i]);
  return ret;
}

static void _mix_columns(uint8_t state[BLOCK_SIZE], const uint8_t matrix[][4])
{
  assert(state != NULL);
  assert(matrix != NULL);
  uint8_t new_state[BLOCK_SIZE] = { 0 };
  int r, c, i;
  for (r = 0; r < 4; ++r)
    for (c = 0; c < 4; ++c)
      for (i = 0; i < 4; ++i)
        new_state[(c << 2) + r] ^=
          GF_256_multiply(matrix[r][i], state[(c << 2) + i]);
  memcpy(state, new_state, sizeof(new_state));
}

#define mix_columns(state) _mix_columns(state, MIX)
#define inv_mix_columns(state) _mix_columns(state, INV_MIX)

static void aes_round(uint8_t state[BLOCK_SIZE], const uint8_t rk[BLOCK_SIZE])
{
  sub_bytes(state);
  shift_rows(state);
  mix_columns(state);
  add_round_key(state, rk);
}

static void aes_inv_round(uint8_t state[BLOCK_SIZE], const uint8_t inv_rk[BLOCK_SIZE])
{
  inv_shift_rows(state);
  inv_sub_bytes(state);
  add_round_key(state, inv_rk);
  inv_mix_columns(state);
}

static void aes_final_round(uint8_t state[BLOCK_SIZE], const uint8_t rk[BLOCK_SIZE])
{
  sub_bytes(state);
  shift_rows(state);
  add_round_key(state, rk);
}

static void inv_final_round(uint8_t state[BLOCK_SIZE], const uint8_t inv_rk[BLOCK_SIZE])
{
  inv_shift_rows(state);
  inv_sub_bytes(state);
  add_round_key(state, inv_rk);
}

static void key_expansion(aes_context *ctx, const uint8_t *key)
{
  assert(ctx != NULL);
  assert(key != NULL);
  uint32_t Nk = ctx->nr - 6;
  uint32_t Ek = (ctx->nr + 1) << 2;
  uint32_t *RK = ctx->rk;

  uint32_t i = 0;
  do {
    GET_UINT32(RK[i], key, i << 2);
  } while(++i < Nk);
  do {
    uint32_t t = RK[i - 1];
    if ((i % Nk) == 0)
      t = SUB_WORD(ROTL8(t)) ^ RCON[i / Nk -1];
    else if (Nk == 8 && (i % Nk) == 4)
      t = SUB_WORD(t);
    RK[i] = RK[i - Nk] ^ t;
  } while(++i < Ek);
}

int aes_set_key(aes_context *ctx, const uint8_t *key, uint32_t key_bit)
{
  if (ctx == NULL || key == NULL)
    return PARM_ERROR;
  switch (key_bit) {
    case 128: ctx->nr = 10; break;
    case 192: ctx->nr = 12; break;
    case 256: ctx->nr = 14; break;
    default: return PARM_ERROR;
  }
  ctx->rk = ctx->buf;
  key_expansion(ctx, key);
  return SUCCESS;
}

int aes_encrypt_block(aes_context *ctx,
  uint8_t cipher_text[BLOCK_SIZE], const uint8_t text[BLOCK_SIZE])
{
  if (ctx == NULL || cipher_text == NULL || text == NULL)
    return PARM_ERROR;
  if (ctx->rk != ctx->buf)
    return NOT_INIT_KEY;
  uint32_t Nr = ctx->nr;
  uint32_t *RK = ctx->rk;
  uint8_t *state = cipher_text;
  memcpy(state, text, BLOCK_SIZE);

  add_round_key(state, (const uint8_t *)RK);
  uint32_t i;
  for (i = 1; i < Nr; ++i)
    aes_round(state, (const uint8_t *)(RK + (i << 2)));
  aes_final_round(state, (const uint8_t *)(RK + (Nr << 2)));

  return SUCCESS;
}

int aes_decrypt_block(aes_context *ctx,
  uint8_t text[BLOCK_SIZE], const uint8_t cipher_text[BLOCK_SIZE])
{
  if (ctx == NULL || text == NULL || cipher_text == NULL)
    return PARM_ERROR;
  if (ctx->rk != ctx->buf)
    return NOT_INIT_KEY;
  uint32_t Nr = ctx->nr;
  uint32_t *INV_RK = ctx->rk;
  uint8_t *state = text;
  memcpy(state, cipher_text, BLOCK_SIZE);

  add_round_key(state, (const uint8_t *)(INV_RK + (Nr << 2)));
  uint32_t i;
  for (i = Nr - 1; i > 0; --i)
    aes_inv_round(state, (const uint8_t *)(INV_RK + (i << 2)));
  inv_final_round(state, (const uint8_t *)INV_RK);

  return SUCCESS;
}

