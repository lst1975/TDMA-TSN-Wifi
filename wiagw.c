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

static tsn_connection_s tsn_udp_gw = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT,
  .sent       = 0,
  .process    = tsn_dlpdu_process_adgw,
  .tsn_read   = NULL,
  .tsn_send   = NULL,
};

void __tsn_test_gw(int id)
{
#if TSN_TEST_GW
  //TODO
  tsn_msg_s *msg;
  msg = tsn_create_msg(&tsn_udp_gw, TSN_BUFF_LEN_MAX);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = TSN_htons(8767);
  addr.sin_addr.s_addr = TSN_htonl(INADDR_ANY);
  tsn_sockaddr_s c = {
      .sa = (struct sockaddr *)&addr,
      .slen = 16,
  };
  tsn_memcpy(&msg->from, &c, sizeof(msg->from));
  switch (id)
  {
    case TSN_AD_JOIN_ack:
      Unsigned8 Status1      = 0;
      Unsigned64 ADLongAddr1 = 0x0000010001000001;
      Unsigned8 AdID1        = 1;
      Unsigned16 ADAddr1     = 0x02;
      make_TSN_AD_JOIN_response(msg, Status1, ADLongAddr1, AdID1, ADAddr1);
      break;

    case TSN_GACK_indication:
      Unsigned8 count2     = 2;
      Unsigned8 AdID2      = 6;
      Unsigned8 addType2   = DMAP_mib_id_static_AddressTypeFlag_u8;
      gack_info_s *gack2   = malloc(count2 * sizeof(gack_info_s));
      gack2[0].Addr.AddrU8 = 0x01;
      gack2[0].Seq         = 1;
      gack2[1].Addr.AddrU8 = 0x03;
      gack2[1].Seq         = 2;
      make_TSN_GACK_indication(msg, count2, AdID2, addType2, gack2);
      break;

    case TSN_NACK_indication:
      Unsigned8 count3    = 3;
      Unsigned8 AdID3     = 6;
      Unsigned8 addType3  = DMAP_mib_id_static_AddressTypeFlag_u8;
      tsn_addr_s *nack3   = malloc(sizeof(tsn_addr_s) * count3);
      nack3[0].AddrU8     = 0x02;
      nack3[1].AddrU8     = 0x04;
      nack3[2].AddrU8     = 0x09;
      make_TSN_NACK_indication(msg, AdID3, addType3, nack3);
      break;

    case TSN_DLDE_DATA_request:
      Unsigned8   AdID4          = 1;
      Unsigned16  DstAddr4       = 0x02;
      Unsigned16  VCR_ID4        = 24;
      Unsigned8   DataType4      = 0;
      Unsigned8   Priority4      = 2;
      Unsigned16  PayloadLength4 = 2;
      OctetString *Payload4      = malloc(sizeof(OctetString) + PayloadLength4 * sizeof(Unsigned8));
      make_TSN_DLDE_DATA_request(msg, AdID4, DstAddr4, VCR_ID4, DataType4, Priority4, PayloadLength4, *Payload4);
      break;

    case TSN_DLME_INFO_GET_request:
      dlme_information_get_request_s req12;
      req12.Handle          = 1;
      req12.DstAddr         = 0x03;
      req12.AttributeID     = 2;
      req12.MemberID        = 4;
      req12.FirstStoreIndex = 2423;
      req12.Count           = 3;
      Unsigned8 AdID12      = 3;
      make_TSN_information_get_request(msg, &req12, AdID12, NULL);
      break;

    case TSN_DLME_INFO_SET_request:
      dlme_information_set_request_s req14;
      req14.Handle          = 1;
      req14.DstAddr         = 0x03;
      req14.AttributeOption = 0;
      req14.AttributeID     = 2;
      req14.MemberID        = 4;
      req14.FirstStoreIndex = 2423;
      req14.Count           = 3;
      Unsigned8 AdID14      = 3;
      tsn_buffer_s *data = malloc(1);
      Unsigned8 abc = 123;
      tsn_buffer_init(data, &abc, 1);
      data->len += 1;
      make_TSN_information_set_request(msg, &req14, AdID14, data);
      break;

    case TSN_DLME_LEAVE_request:
      dlme_leave_request_s req16;
      Unsigned8 AdID16       = 2;
      Unsigned16 ShortAddr16 = 0x06;
      make_TSN_DLME_LEAVE_request(msg, AdID16, ShortAddr16);
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

  tsn_system_init();
  TSN_HandleListInit();

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

  TSN_SetDmapInitializationDone();
  
  __tsn_test_gw(TSN_NACK_indication);

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
  tsn_server_free(&tsn_udp_gw);

clean1:
  wia_epoll_free();
clean0:
  return ret;
}
