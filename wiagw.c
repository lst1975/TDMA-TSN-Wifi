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

#define SERVER_PORT 8765

static tsn_connection_s tsn_udp_server = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT,
  .sent       = 0,
  .proces     = tsn_dlpdu_process_adgw,
  .tsn_read   = NULL,
  .tsn_write  = NULL,
};

int main(int argc, char **argv)
{
  int ret = EXIT_FAILURE;
  tsn_err_e r;

  tsn_system_init();
  TSN_HandleListInit();

  r = wia_epoll_init();
  if (r != TSN_err_none)
  {
    TSN_error("wia_epoll_init failed.\n");
    goto clean0;
  }

  r = tsn_server_init(&tsn_udp_server);
  if (r != TSN_err_none)
  {
    TSN_error("tsn_server_init failed.\n");
    goto clean1;
  }

  __gw_dmap_state_machine(NULL, NULL, DMAP_TRIGGER_T0, NULL);
  
  while (sysCfg.run)
  {
    r = wia_epoll_process_events(TSN_TIMER_INFINITE, 0);
    if (r != TSN_err_none)
    {
      TSN_warn("Failed to wia_epoll_process_events().\n");
      TSN_error("(%d): %m.\n", errno, errno);
    }
    TSN_CheckHandle();
  }
  
  ret = EXIT_SUCCESS;
  tsn_server_free();

clean1:
  wia_epoll_free();
clean0:
  return ret;
}
