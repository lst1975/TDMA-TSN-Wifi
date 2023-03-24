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

#define SERVER_PORT 8766
#define CLIENT_PORT 8767

#if 0
static tsn_connection_s tsn_udp_fd = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT,
  .sent       = 0,
  .proces     = fd_tsn_dlpdu_process_adgw,
  .tsn_read   = __fd_read_udp_msg,
  .tsn_write  = __fd_send_udp_msg,
};
#endif

static tsn_connection_s tsn_udp_gw = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = CLIENT_PORT,
  .sent       = 0,
  .proces     = tsn_dlpdu_process_adgw,
  .tsn_read   = __read_udp_msg,
  .tsn_write  = __send_udp_msg,
};

int main(int argc, char **argv)
{
  int ret = EXIT_FAILURE;
  tsn_err_e r;

  r = wia_epoll_init();
  if (r != TSN_err_none)
  {
    TSN_error("wia_epoll_init failed.\n");
    goto clean0;
  }

  r = tsn_server_init(&tsn_udp_gw);
  if (r != TSN_err_none)
  {
    TSN_error("tsn_server_init failed.\n");
    goto clean1;
  }

  r = tsn_server_init(&tsn_udp_fd);
  if (r != TSN_err_none)
  {
    TSN_error("tsn_server_init failed.\n");
    goto clean2;
  }

  while (sysCfg.run)
  {
    r = wia_epoll_process_events(200, 0);
    if (r != TSN_err_none)
    {
      TSN_warn("Failed to wia_epoll_process_events().\n");
    }
  }
  
  ret = EXIT_SUCCESS;

clean2:
  tsn_server_free(&tsn_udp_gw);
clean1:
  wia_epoll_free();
clean0:
  return ret;
}
