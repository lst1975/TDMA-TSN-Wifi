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

tsn_boolean_e tsn_crc_init(void)
{
  return TSN_TRUE;
}

uint64_t tsn_get_fcs(char *p)
{
  if (sysCfg.fcsLength==1)
    return p[0];
  else if (sysCfg.fcsLength==2)
    return TSN_ntohs(*(uint16_t*)p);
  else if (sysCfg.fcsLength==4)
    return TSN_ntohl(*(uint32_t*)p);
  else 
    return 0;
}

int tsn_set_fcs(char *p, uint64_t fcs)
{
  if (sysCfg.fcsLength==1)
  {
    p[0] = (uint8_t)fcs;
    return 1;
  }
  else if (sysCfg.fcsLength==2)
  {
    *(uint16_t*)p = TSN_htons((uint16_t)fcs);
    return 2;
  }
  else if (sysCfg.fcsLength==4)
  {
    *(uint32_t*)p = TSN_htonl((uint32_t)fcs);
    return 4;
  }
  else 
    return 0;
}

uint64_t
tsn_calc_fcs(char *p, int length)
{
  return crc32(p, length);
}

tsn_boolean_e
tsn_check_fcs(char *p, int length, uint64_t fcs)
{
  return crc32(p, length) == fcs;
}

