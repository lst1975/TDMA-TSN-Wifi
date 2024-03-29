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
#ifndef __TSN_NETWORK_H__
#define __TSN_NETWORK_H__

#define TSN_ADID_MAX 32
#define TSN_ADID_INVALID TSN_ADID_MAX
#define TSN_ShorAddress_MAX 1024
#define TSN_ShorAddress_INVALID ((uint16_t)(0))
#define TSN_NetworkID_MAX 8

typedef struct tsn_network tsn_network_s;

struct tsn_network{
  uint64_t AdID:8;
  uint64_t FDNumber:24;
  uint64_t Active:1;
  uint64_t Index:10;
  uint64_t DevCount:16;
  uint64_t *AcceptedPhyAddr;
  tsn_device_s *Devices[TSN_ShorAddress_MAX];
  list_head_s Ads;
  list_head_s Fds;
  list_head_s All;
  list_head_s link;
  tsn_sockaddr_s ads[TSN_ADID_MAX];
};

tsn_err_e 
tsn_network_find(
  tsn_network_s **net, 
  unsigned int network);

tsn_err_e
tsn_network_init(
  tsn_network_s *net, 
  int index
);

tsn_err_e
tsn_network_release(
  unsigned int network
);

tsn_err_e
tsn_network_actiate(
  unsigned int network
);

#endif
