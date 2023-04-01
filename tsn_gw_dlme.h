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
#ifndef _TSN_GW_DLME_H_
#define _TSN_GW_DLME_H_

struct DlmeJoinRequest{
  uint8_t NetworkID;
  BitField24 Channel;
  uint64_t PhyAddr;
  uint64_t SecMaterial;
};
typedef struct DlmeJoinRequest dlme_join_request_s;

struct DlmeJoinIndication{
  uint64_t PhyAddr;
  uint64_t SecMaterial;
  union {
    tsn_boolean_e AuthenResult; /* internal use */
    tsn_boolean_e AddrResult; /* internal use */
  };
};
typedef struct DlmeJoinIndication dlme_join_indication_s;

enum{
  DLME_JOIN_SUCCESS = 0,
  DLME_JOIN_UNMATCHED_NetworkID,
  DLME_JOIN_AUTHENTICATION_Failed,
  DLME_JOIN_EXCEEDED,
};
struct DlmeJoinResponse{
  uint8_t Status;
  uint16_t ShortAddr;
};
typedef struct DlmeJoinResponse dlme_join_response_s;

struct DlmeJoinConfirm{
  uint8_t Status;
  uint16_t ShortAddr;
};
typedef struct DlmeJoinConfirm dlme_join_confirm_s;


static inline const char *dlmeJoinStatus2String(int status)
{
  switch (status) {
    case DLME_JOIN_SUCCESS:
      return "SUCCESS";
    case DLME_JOIN_UNMATCHED_NetworkID:
      return "UNMATCHED NetworkID";
    case DLME_JOIN_AUTHENTICATION_Failed:
      return "AUTHENTICATION Failed";
    case DLME_JOIN_EXCEEDED:
      return "EXCEEDED";
    default:
      return "<Unknown>";
  }
}

enum{
  DLME_Device_PowerSupplyStatus_FixedPower = 0,
  DLME_Device_PowerSupplyStatus_Level_1,  /* | low, insufficiant power supply */
  DLME_Device_PowerSupplyStatus_Level_2,  /* | */
  DLME_Device_PowerSupplyStatus_Level_3,  /* | */
  DLME_Device_PowerSupplyStatus_Level_4,  /* | */
  DLME_Device_PowerSupplyStatus_Level_5,  /* | */
  DLME_Device_PowerSupplyStatus_Level_6,  /* | */
  DLME_Device_PowerSupplyStatus_Level_7,  /* | */
  DLME_Device_PowerSupplyStatus_Level_8,  /* | */
  DLME_Device_PowerSupplyStatus_Level_9,  /* | */
  DLME_Device_PowerSupplyStatus_Level_10, /* | high, sufficiant power supply */
};
static inline const char *dlmeDevPowerSupplyStatus2String(unsigned int status)
{
  const char *PowerSupplyStatus[]={
    "FixedPower", 
    "Level_1", "Level_2", "Level_3", "Level_4", "Level_5",
    "Level_6", "Level_7", "Level_8", "Level_9", "Level_10",
  };
  if (status <= DLME_Device_PowerSupplyStatus_Level_10)
    return PowerSupplyStatus[status];
  else
    return "<Invalid>";
}
struct DlmeDeviceStatusRequest{
  uint8_t PowerSupplyStatus;
};
typedef struct DlmeDeviceStatusRequest dlme_device_status_request_s;

struct DlmeDeviceStatusIndication{
  uint16_t ShortAddr;
  uint8_t PowerSupplyStatus;
};
typedef struct DlmeDeviceStatusIndication dlme_device_status_indication_s;

enum{
  DLME_device_status_confirm_SUCCESS = 0,
  DLME_device_status_confirm_FAILURE,
};
struct DlmeDeviceStatusConfirm{
  uint8_t Status;
};
typedef struct DlmeDeviceStatusConfirm dlme_device_status_confirm_s;

struct DlmeTimeSyncRequest{
  uint16_t SrcAddr;
  TimeData FieldDeviceTimeValue;
};
typedef struct DlmeTimeSyncRequest dlme_time_sync_request_s;

struct DlmeTimeSyncResponse{
  uint16_t DstAddr;
  TimeData FieldDeviceTimeValue;
  TimeData ReceiveTimeValue;
};
typedef struct DlmeTimeSyncResponse dlme_time_sync_response_s;

struct DlmeTimeSyncIndication{
  uint16_t SrcAddr;
  TimeData FieldDeviceTimeValue;
};
typedef struct DlmeTimeSyncIndication dlme_time_sync_indication_s;

enum{
  DLME_timesync_confirm_SUCCESS=0,
  DLME_timesync_confirm_OVERTIME,
};
struct DlmeTimeSyncConfirm{
  uint8_t Status;
  TimeData FieldDeviceTimeValue;
  TimeData ReceiveTimeValue;
};
typedef struct DlmeTimeSyncConfirm dlme_time_sync_confirm_s;

struct DlmeInformationGetRequest{
  uint8_t  Handle;
  uint16_t DstAddr;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint16_t Count;
};
typedef struct DlmeInformationGetRequest dlme_information_get_request_s;

struct DlmeInformationGetIndication{
  uint16_t SrcAddr;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint16_t Count;
};
typedef struct DlmeInformationGetIndication dlme_information_get_indication_s;

enum{
  DLME_information_get_response_SUCCESS = 0,
  DLME_information_get_response_UNSUPPORTED_ATTRIBUTE,
};
struct DlmeInformationGetResponse{
  uint16_t DstAddr;
  uint8_t  Status;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint16_t Count;
  uint8_t  AttributeValue[0];
};
typedef struct DlmeInformationGetResponse dlme_information_get_response_s;

static inline const char *dlmeInformationGetResponseStatus2String(int status)
{
  switch (status) {
    case DLME_information_get_response_SUCCESS:
      return "SUCCESS";
    case DLME_information_get_response_UNSUPPORTED_ATTRIBUTE:
      return "UNSUPPORTED ATTRIBUTE";
    default:
      return "<Unknown>";
  }
}

enum{
  DLME_channel_condition_confirm_SUCCESS = 0,
  DLME_channel_condition_confirm_FAILURE,
};
struct DlmeChannelConditionConfirm{
  uint8_t Status;
};

struct ChannelConditionInfomation{
  uint8_t ChannelID;
  uint8_t LinkQuality;
  SingleFloat PacketLossRate;
  uint8_t RetryNumber;
};
typedef struct ChannelConditionInfomation channel_condition_infomation_s;

struct DlmeChannelConditionIndication{
  uint16_t SrcAddr;
  uint8_t Count;
  channel_condition_infomation_s ChannelConditionInfo[0];
};
typedef struct DlmeChannelConditionIndication dlme_channel_condition_indication_s;

struct DlmeChannelConditionRequest{
  uint8_t Count;
  channel_condition_infomation_s ChannelConditionInfo[0];
};
typedef struct DlmeChannelConditionRequest dlme_channel_condition_request_s;


enum{
  DLME_information_get_confirm_SUCCESS = 0,
  DLME_information_get_confirm_UNSUPPORTED_ATTRIBUTE,
};
struct DlmeInformationGetConfirm{
  uint8_t  Handle;
  uint16_t SrcAddr;
  uint8_t  Status;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint16_t Count;
  uint8_t  AttributeValue[0];
};
typedef struct DlmeInformationGetConfirm dlme_information_get_confirm_s;

static inline const char *dlmeInformationGetConfirmStatus2String(int status)
{
  switch (status) {
    case DLME_information_get_confirm_SUCCESS:
      return "SUCCESS";
    case DLME_information_get_confirm_UNSUPPORTED_ATTRIBUTE:
      return "UNSUPPORTED ATTRIBUTE";
    default:
      return "<Unknown>";
  }
}
/***********************************************************************************
 * GB/T26790.2-2015, 8.3.8, Page 73
 *                   INFORMATION set
 *
 * used by Gateway device to set attributes for field devices
 *
 ************************************************************************************
 *
 *  GW-DMAP                 AD-DLL              FD-DLL                    FD-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-INFO-SET.request |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      | information set   |                         |
 *     |                      |  request frame    |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   | DLME-INFO-SET.indication|
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   |  DLME-INFO-SET.response |
 *     |                      |                   |<------------------------|
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |  information set  |                         |
 *     |                      |   response frame  |                         |
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |        GACK       |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-INFO-SET.confirm |                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/
enum{
  DLME_information_set_option_ADD=0,
  DLME_information_set_option_DELETE,
  DLME_information_set_option_UPDATE,
};

static inline const char *dlme_info_op2string(int id)
{
  switch (id) {
    case DLME_information_set_option_ADD:
      return "ADD";
    case DLME_information_set_option_DELETE:
      return "DELETE";
    case DLME_information_set_option_UPDATE:
      return "UPDATE";
    default:
      return "<Unknown>";
  }
}

struct DlmeInformationSetRequest{
  uint8_t  Handle;
  uint16_t DstAddr;
  uint8_t  AttributeOption;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint16_t Count;
  uint8_t  AttributeValue[0];
};
typedef struct DlmeInformationSetRequest dlme_information_set_request_s;

struct DlmeInformationSetIndication{
  uint8_t  Handle;
  uint16_t SrcAddr;
  uint8_t  AttributeOption;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint16_t Count;
  uint8_t  AttributeValue[0];
};
typedef struct DlmeInformationSetIndication dlme_information_set_indication_s;

enum{
  DLME_information_set_response_SUCCESS = 0,
  DLME_information_set_response_UNSUPPORTED_ATTRIBUTE,
  DLME_information_set_response_INVALID_PARAMETER,
};
struct DlmeInformationSetResponse{
  uint8_t  Handle;
  uint16_t SrcAddr;
  uint8_t  AttributeOption;
  uint8_t  AttributeID;
  uint8_t  MemberID;
  uint16_t FirstStoreIndex;
  uint8_t  Count;
  uint8_t  Status;
};
typedef struct DlmeInformationSetResponse dlme_information_set_response_s;

static const char *dlme_info_set_res_status2string(uint8_t Status)
{
  switch (Status)
  {
    case DLME_information_set_response_SUCCESS:
      return "SUCCESS";
    case DLME_information_set_response_UNSUPPORTED_ATTRIBUTE:
      return "UNSUPPORTED ATTRIBUTE";
    case DLME_information_set_response_INVALID_PARAMETER:
      return "INVALID PARAMETER";
    default:
      return "<UNKNOWN>";
  }
}
enum{
  DLME_information_set_confirm_SUCCESS = 0,
  DLME_information_set_confirm_UNSUPPORTED_ATTRIBUTE,
  DLME_information_set_confirm_INVALID_PARAMETER,
};
struct DlmeInformationSetConfirm{
  uint8_t Handle;
  uint8_t Status;
};
typedef struct DlmeInformationSetConfirm dlme_information_set_confirm_s;

static const char *dlme_info_set_cfm_status2string(uint8_t Status)
{
  switch (Status)
  {
    case DLME_information_set_confirm_SUCCESS:
      return "SUCCESS";
    case DLME_information_set_confirm_UNSUPPORTED_ATTRIBUTE:
      return "UNSUPPORTED ATTRIBUTE";
    case DLME_information_set_confirm_INVALID_PARAMETER:
      return "INVALID PARAMETER";
    default:
      return "<UNKNOWN>";
  }
}

static inline tsn_boolean_e 
DLME_information_set_request(tsn_msg_s *_msg, 
  dlme_information_set_request_s *req, Unsigned8 AdID, 
  Unsigned8 NetworkID, tsn_buffer_s *b)
{
  int AttrLen;
  tsn_msg_s *msg;

  AttrLen = sizeof(dlme_information_set_request_s)+TSN_BUFFER_LEN(b);
  msg = tsn_duplicate_msg(_msg, 4+AttrLen);
  if (msg == NULL)
    return TSN_FALSE;

  if (TSN_AllocateHandle(msg) == TSN_FALSE)
  {
    tsn_free_msg(msg);
    return TSN_FALSE;
  }
  req->Handle = msg->handle;
  
  if (TSN_err_none != make_TSN_information_set_request(msg, 
    req, AdID, NetworkID, b))
  {
    tsn_free_msg(msg);
    return TSN_FALSE;
  }

  tsn_send_udp_msg(msg);
  return TSN_TRUE;
}

static inline tsn_boolean_e
DLME_information_set_indication(tsn_msg_s *msg,
  dlme_information_set_indication_s *ind, Unsigned8 AdID,
  tsn_buffer_s *b)
{
  return TSN_FALSE;
}
static inline tsn_boolean_e
DLME_information_set_response(tsn_msg_s *msg,
  dlme_information_set_response_s *rsp, Unsigned8 AdID,
  tsn_buffer_s *b)
{
  return TSN_FALSE;
}
static inline tsn_boolean_e
DLME_information_set_confirm(tsn_msg_s *msg,
  dlme_information_set_confirm_s *cfm, Unsigned8 AdID,
  tsn_buffer_s *b)
{
  return TSN_FALSE;
}

struct DlmeLeaveRequest{
  union{
    uint8_t  ShortAddr8;
    uint16_t ShortAddr16;
  };
};
typedef struct DlmeLeaveRequest dlme_leave_request_s;

tsn_err_e 
make_TSN_DLME_TIME_SYNC_response(
  tsn_msg_s *msg, 
  uint8_t AdID, 
  uint16_t SrcAddr, 
  TimeData FieldDeviceTimeValue);

#endif
