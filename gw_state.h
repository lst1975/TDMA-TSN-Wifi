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
#ifndef __GW_STATE_H__
#define __GW_STATE_H__

#define TSN_FieldDevice_MAX 1024

enum tsn_dmap_state{
  DMAP_STATE_init,
  DMAP_STATE_active,
  DMAP_STATE_join,
  DMAP_STATE_join_authenticating,
  DMAP_STATE_join_allocatingShortAddress,
  DMAP_STATE_resource_allocation,
  DMAP_STATE_operation,
  DMAP_STATE_leave,
  DMAP_STATE_end
};
typedef enum tsn_dmap_state tsn_dmap_state_e;

enum tsn_dmap_trigger{
  DMAP_TRIGGER_T0,
  DMAP_TRIGGER_T1_join,
  DMAP_TRIGGER_T1_auth,
  DMAP_TRIGGER_T2_addr,
  DMAP_TRIGGER_T3,
  DMAP_TRIGGER_T4_ResAlloc,
  DMAP_TRIGGER_T5_infoSetConfirm,
  DMAP_TRIGGER_T6,
  DMAP_TRIGGER_T7,
  DMAP_TRIGGER_T8,
  DMAP_TRIGGER_T9,
  DMAP_TRIGGER_T10,
  DMAP_TRIGGER_T11,
  DMAP_TRIGGER_T12,
  DMAP_TRIGGER_T13,
  DMAP_TRIGGER_T14,
  DMAP_TRIGGER_T15,
  DMAP_TRIGGER_T16,
  DMAP_TRIGGER_MAX,
};
typedef enum tsn_dmap_trigger tsn_dmap_trigger_e;

enum ad_state{
  AD_STATE_init,
  AD_STATE_idle,
  AD_STATE_s1,
  AD_STATE_s2,
  AD_STATE_s3,
  AD_STATE_s4,
  AD_STATE_s5,
  AD_STATE_s6,
  AD_STATE_s7,
  AD_STATE_s8,
};

#endif
