
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
#include "tsn_private.h"

struct _res_shortaddr{
  uint16_t ShortAddr;
  list_head_s link;
};
typedef struct _res_shortaddr TsnShortAddr;

struct _res_shortaddr_s{
  uint16_t ShortAddr;
  list_head_s head;
};
typedef struct _res_shortaddr_s tsn_short_addr_s;

static TsnShortAddr tsn_shoraddr_array[TSN_ShorAddress_MAX];

static tsn_short_addr_s tsn_shoraddr_list = {
  .ShortAddr = 0,
  .head = LIST_HEAD_INIT(tsn_shoraddr_list.head),
};
 
tsn_boolean_e 
TSN_AllocateShortAddr(uint16_t *Addr)
{
  tsn_short_addr_s *s = &tsn_shoraddr_list;
  if (s->ShortAddr >= TSN_ShorAddress_MAX - 1)
  {
    TsnShortAddr *a;
    if (list_empty(&s->head))
      return TSN_FALSE;
    a = list_first_entry(&s->head, TsnShortAddr, link);
    list_del(&a->link);
    *Addr = a->ShortAddr;
  }
  else
  {
    *Addr = ++s->ShortAddr;
  }
  return TSN_TRUE;
}

void 
TSN_FreeShortAddr(uint16_t *_Addr)
{
  uint16_t Addr = *_Addr;

  if (Addr == TSN_ShorAddress_INVALID)
  {
    TSN_warn("Try to free invalid ZERO short address.\n")
    return;
  }
  
  if (Addr >= TSN_ShorAddress_MAX - 1)
  {
    TSN_warn("Try to free invalid OVERFLOWED short address.\n")
    return;
  }
  
  tsn_short_addr_s *s = &tsn_shoraddr_list;
  TsnShortAddr *a = &tsn_shoraddr_array[Addr];
  list_add_tail(&a->link, &s->head);
  *_Addr = TSN_ShorAddress_INVALID;
}

