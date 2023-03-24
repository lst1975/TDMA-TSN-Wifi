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
#ifndef __TSN_STATE_GW_H__
#define __TSN_STATE_GW_H__

tsn_err_e 
gw_dmap_T1_receive_dlme_join_indication(
  tsn_msg_s *msg, 
  tsn_device_s *dmap, 
  dlme_join_indication_s *ind);

tsn_boolean_e 
gw_dmap_T1_receive_authentication_response(
 tsn_msg_s *msg, 
 tsn_device_s *dmap, 
 dlme_join_indication_s *ind);

tsn_boolean_e 
gw_dmap_T5_receive_information_set_confirm(
  tsn_msg_s *msg, 
  tsn_device_s *dmap, 
  dlme_information_set_confirm_s *cfm);

tsn_boolean_e 
__gw_dmap_state_machine(
  tsn_msg_s *msg, 
  tsn_device_s *dmap, 
  tsn_dmap_trigger_e trigger, 
  void *dlpdu);

tsn_boolean_e TSN_IsDmapInitializationDone(void);
void TSN_SetDmapInitializationDone(void);

#endif
