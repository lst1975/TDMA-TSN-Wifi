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

static tsn_connection_s tsn_udp_fd = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT,
  .sent       = 0,
  .process    = tsn_dlpdu_process_dll,
  .tsn_read   = NULL,
  .tsn_send   = NULL,
};

static tsn_connection_s tsn_udp_ad = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = CLIENT_PORT,
  .sent       = 0,
  .process    = tsn_dlpdu_process_adgw,
  .tsn_read   = NULL,
  .tsn_send   = NULL,
};

void __tsn_test_ad(int id)
{
#if TSN_TEST_AD
  //TODO
  tsn_msg_s *msg;
  msg = tsn_create_msg(&tsn_udp_ad, TSN_BUFF_LEN_MAX);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = TSN_htons(8765);
  addr.sin_addr.s_addr = TSN_htonl(INADDR_ANY);
  msg->from.sa = (struct sockaddr *)&addr;
  msg->from.slen = 16;
  switch (id)
  {
    case TSN_AD_JOIN_request:
      Unsigned64 ADAddr0    = 0x0000010001000001;
      Unsigned8  NetworkID0 = 0;
      Unsigned64 PhyAddr0   = 0x0000010001000001;
      make_TSN_AD_JOIN_request(msg, ADAddr0, NetworkID0, PhyAddr0);
      break;
    case TSN_DLME_JOIN_indication:
      Unsigned8  AdID6        = 1;
      Unsigned64 PhyAddr6     = 0x0000010002000001;
      Unsigned64 SecMaterial6 = 0;
      make_TSN_DLME_JOIN_indication(msg, AdID6, PhyAddr6, SecMaterial6);
      break;
    case TSN_DLME_DEV_STATUS_indication:
      Unsigned8  AdID8              = 2;
      Unsigned16 ShortAddr8         = 3;
      Unsigned8  PowerSupplyStatus8 = 4;
      make_TSN_DLME_DEV_STATUS_indication(msg, AdID8, ShortAddr8, PowerSupplyStatus8);
      break;

    case TSN_DLME_CHAN_COND_indication:
      Unsigned8  AdID9    = 2;
      Unsigned16 SrcAddr9 = 3;
      Unsigned8  Count9   = 4;
      channel_condition_infomation_s ChannelConditionInfo[4];
      for (int i = 0;i < 4;i++)
      {
          ChannelConditionInfo[i].ChannelID = i;
          ChannelConditionInfo[i].LinkQuality = i*2;
          float cf = i*0.5;
          ChannelConditionInfo[i].PacketLossRate = *(SingleFloat *)&cf;
          ChannelConditionInfo[i].RetryNumber = i*i;
      }
      make_TSN_DLME_CHAN_COND_indication(msg, AdID9, SrcAddr9, Count9, ChannelConditionInfo);
      break;

    case TSN_DLME_TIME_SYNC_indication:
      Unsigned8  AdID10                 = 2;
      Unsigned16 SrcAddr10              = 3;
      Unsigned64 FieldDeviceTimeValue10 = 4;
      make_TSN_DLME_TIME_SYNC_indication(msg, AdID10, SrcAddr10, FieldDeviceTimeValue10);
      break;

    case TSN_DLME_INFO_GET_confirm:
      dlme_information_get_confirm_s cfm13;
      cfm13.Handle = 10;
      cfm13.SrcAddr = 23;
      cfm13.Status = 0;
      cfm13.AttributeID = 234;
      cfm13.MemberID = 34;
      cfm13.FirstStoreIndex = 123;
      cfm13.Count = 3;
      Unsigned8 AdID13 = 1;
      tsn_buffer_s *data = malloc(1);
      Unsigned8 abc = 123;
      tsn_buffer_init(data, &abc, 1);
      data->len += 1;
      make_TSN_DLME_INFO_GET_confirm(msg, &cfm13, AdID13, data);
      break;

    case TSN_DLME_INFO_SET_confirm:
      dlme_information_set_confirm_s cfm15;
      cfm15.Handle = 10;
      cfm15.Status = 0;
      Unsigned8 AdID15 = 1;
      make_TSN_DLME_INFO_SET_confirm(msg, &cfm15, AdID15);
      break;

    default:
      break;
  }
  tsn_send_udp_msg(msg);
#endif
}

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

  r = tsn_server_init(&tsn_udp_ad);
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

  __tsn_test_ad(TSN_DLME_CHAN_COND_indication);

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
  tsn_server_free(&tsn_udp_ad);
clean1:
  wia_epoll_free();
clean0:
  return ret;
}
