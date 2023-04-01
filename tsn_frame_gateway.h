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
#ifndef _TSN_FRAME_GATEWAY_H__
#define _TSN_FRAME_GATEWAY_H__

/* UAL   ---> User application layer 
 * ASL   ---> Application sub-layer 
 * UAP   ---> User application process 
 * ASLDE ---> Application sub-layer data entity 
 * ASLME ---> Application sub-layer management entity 
 */

/****************************************************************
* GB/T26790.2-2015, 9.3, Page 94
*          Cable Service Between GW and AD
****************************************************************/
enum {
  TSN_AD_JOIN_request = 0,        /* ad --> gw */
  TSN_AD_JOIN_ack,                /* gw --> ad */
  TSN_GACK_indication,            /* gw --> ad */
  TSN_NACK_indication,            /* gw --> ad */
  TSN_DLDE_DATA_request,          /* gw --> ad */
  TSN_DLDE_DATA_indication,       /* ad --> gw */
  TSN_DLME_JOIN_indication,       /* ad --> gw */
  TSN_DLME_JOIN_response,         /* gw --> ad */
  TSN_DLME_DEV_STATUS_indication, /* ad --> gw */
  TSN_DLME_CHAN_COND_indication,  /* ad --> gw */
  TSN_DLME_TIME_SYNC_indication,  /* ad --> gw */
  TSN_DLME_TIME_SYNC_response,    /* gw --> ad */
  TSN_DLME_INFO_GET_request,      /* gw --> ad */
  TSN_DLME_INFO_GET_confirm,      /* ad --> gw */
  TSN_DLME_INFO_SET_request,      /* gw --> ad */
  TSN_DLME_INFO_SET_confirm,      /* ad --> gw */
  TSN_DLME_LEAVE_request,         /* gw --> ad */
  TSN_KEY_ESTABLISH_request,      /* gw --> ad */
  TSN_KEY_ESTABLISH_confirm,      /* ad --> gw */
  TSN_KEY_UPDATE_request,         /* gw --> ad */
  TSN_KEY_UPDATE_confirm,         /* ad --> gw */
  TSN_SECURITY_ALARM_indication,  /* ad --> gw */
  TSN_dlpdu_type_max,             
};

struct AdJoinRequest {
  Unsigned8  NetworkID;
  Unsigned64 PhyAddr;
};
typedef struct AdJoinRequest tsn_ad_join_request_s;

enum{
  TSN_AD_JOIN_ACK_SUCCESS = 0,
  TSN_AD_JOIN_ACK_UNMATCHED_NetworkID,
  TSN_AD_JOIN_ACK_AUTHENTICATION_Failed,
  TSN_AD_JOIN_ACK_EXCEEDED,
  TSN_AD_JOIN_ACK_NETWORK_SCALE_ERROR,
  TSN_AD_JOIN_ACK_INVALID_DeviceID,
  TSN_AD_JOIN_ACK_SYSTEM_ERROR,
};
struct AdJoinResponse {
  Unsigned8  Status;
  Unsigned8  AdID;
  Unsigned16 ADAddr;
};
typedef struct AdJoinResponse tsn_ad_join_response_s;

static inline const char *AdJoinResponseStatus2String(int status)
{
  switch (status) {
    case TSN_AD_JOIN_ACK_SUCCESS:
      return "SUCCESS";
    case TSN_AD_JOIN_ACK_UNMATCHED_NetworkID:
      return "UNMATCHED NetworkID";
    case TSN_AD_JOIN_ACK_AUTHENTICATION_Failed:
      return "AUTHENTICATION Failed";
    case TSN_AD_JOIN_ACK_EXCEEDED:
      return "EXCEEDED";
    case TSN_AD_JOIN_ACK_NETWORK_SCALE_ERROR:
      return "NETWORK SCALE ERROR";
    case TSN_AD_JOIN_ACK_INVALID_DeviceID:
      return "INVALID DeviceID";
    case TSN_AD_JOIN_ACK_SYSTEM_ERROR:
      return "SYSTEM ERROR";
    default:
      return "<Unknown>";
  }
}
struct GACKInfo{
  tsn_addr_s Addr;
  uint16_t Seq;
};
typedef struct GACKInfo gack_info_s;

struct GAckIndication {
  Unsigned8   DeviceCount;
  gack_info_s GAckInfo[0];
};
typedef struct GAckIndication tsn_gack_indication_s;

struct NAckIndication {
  Unsigned8 RetryDeviceCount;
  tsn_addr_s DstAddressList[0];
};
typedef struct NAckIndication tsn_nack_indication_s;

tsn_err_e 
tsn_dlpdu_process_adgw(
  tsn_msg_s *msg
);

tsn_err_e
make_TSN_AD_JOIN_response(
  tsn_msg_s *msg,
  Unsigned8 Status, 
  Unsigned64 ADLongAddr,
  Unsigned8 AdID, 
  Unsigned16 ADAddr);

tsn_err_e
make_TSN_DLDE_DATA_request(
  tsn_msg_s *msg,
  Unsigned8   AdID,
  Unsigned16  DstAddr,
  Unsigned16  VCR_ID,
  Unsigned8   DataType,
  Unsigned8   Priority,
  Unsigned16  PayloadLength,
  OctetString Payload
);

tsn_err_e
make_TSN_DLME_JOIN_response(
  tsn_msg_s *msg,
  Unsigned8 AdID, 
  Unsigned8 Status, 
  Unsigned16 ShortAddr
);

tsn_err_e
make_TSN_GACK_indication(
  tsn_msg_s *msg,
  Unsigned8 count, 
  Unsigned8 AdID, 
  Unsigned8 addType,
  gack_info_s *gack
);

tsn_err_e
make_TSN_NACK_indication(
  tsn_msg_s *msg,
  Unsigned8 count, 
  Unsigned8 AdID, 
  Unsigned8 addType,
  tsn_addr_s *nack
);

tsn_err_e 
make_TSN_information_set_request(
  tsn_msg_s *msg, 
  void *_req, 
  Unsigned8 AdID, 
  Unsigned8 NetworkID, 
  tsn_buffer_s *data
);

tsn_err_e 
make_TSN_information_get_request(
  tsn_msg_s *msg, 
  void *_req, 
  Unsigned8 AdID, 
  tsn_buffer_s *data __TSN_UNUSED
);

tsn_err_e
make_TSN_DLME_LEAVE_request(
  tsn_msg_s *msg,
  Unsigned8 AdID, 
  Unsigned16 ShortAddr
);

#endif
