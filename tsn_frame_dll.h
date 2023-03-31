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
#ifndef __TSN_FRAME_DLL_H__
#define __TSN_FRAME_DLL_H__

/***********************************************************************************
 * GB/T26790.2-2015, 8.4.1, Page 78
 *                   General DLL frame format
 *
 ***********************************************************************************/

#define FRAME_TYPE_Beacon                            0
#define FRAME_TYPE_Data                              1
#define FRAME_TYPE_Aggregation                       2
#define FRAME_TYPE_GACK                              3
#define FRAME_TYPE_NACK                              4
#define FRAME_TYPE_JoinRequest                       5
#define FRAME_TYPE_JoinResponse                      6
#define FRAME_TYPE_LeaveRequest                      7
#define FRAME_TYPE_LeaveResponse                     8
#define FRAME_TYPE_DeviceStatusReport                9
#define FRAME_TYPE_ChannelConditionReport            10
#define FRAME_TYPE_TwoWayTimeSynchronizationRequest  11
#define FRAME_TYPE_TwoWayTimeSynchronizationResponse 12
#define FRAME_TYPE_RemoteAttributeGetRequest         13
#define FRAME_TYPE_RemoteAttributeGetResponse        14
#define FRAME_TYPE_RemoteAttributeSetRequest         15
#define FRAME_TYPE_RemoteAttributeSetResponse        16
#define FRAME_TYPE_KeyEstablishRequest               17
#define FRAME_TYPE_KeyEstablishResponse              18
#define FRAME_TYPE_KeyUpdateRequest                  19
#define FRAME_TYPE_KeyUpdateResponse                 20
#define FRAME_TYPE_SecurityAlarm                     21
#define FRAME_TYPE_MAX                               22

static inline int GetFrameType(void *dlpdu)
{
  int FrameType;
}

static inline uint16_t GetSrcAddr(void *dlpdu)
{
  uint16_t SrcAddr;
}

static inline int GetPayloadLength(void *dlpdu)
{
  int PayloadLength;
}

static inline uint8_t* GetPayload(void *dlpdu)
{
  uint8_t *Payload;
}

struct tsn_dlpdu_dllhdr_raw{
  Unsigned8 type:5;
  Unsigned8 is_segment:1;
  Unsigned8 is_preemption:1;
  Unsigned8 is_shortaddr:1;
  Unsigned8 networkID;
  Unsigned8 data[0]; 
};
typedef struct tsn_dlpdu_dllhdr_raw tsn_dlpdu_dllhdr_raw_s;

struct tsn_dlpdu_dllhdr{
  Unsigned8 type:5;
  Unsigned8 is_segment:1;
  Unsigned8 is_preemption:1;
  Unsigned8 is_shortaddr:1;
  Unsigned8 networkID;
  tsn_addr_s addr;
  Unsigned16 seq;
  Unsigned8  segment_count;
  Unsigned8  segment_seq;
  Unsigned16 length;
};
typedef struct tsn_dlpdu_dllhdr tsn_dlpdu_dllhdr_s;

struct BeaconPayload {
  Unsigned8 UpSharedTimeslotNumber:4;
  Unsigned8 DownSharedTimeslotNumber:4;
};
typedef struct BeaconPayload  tsn_beacon_payload_s;

struct BeaconInformation {
  uint16_t SuperframeLength;
  uint16_t TimeslotDuration;
  uint16_t BeaconRelativeTimeslotNum;
  uint16_t FirstSharedTimeslotNumber;
  uint8_t  SharedTimeslotNumber;
  TimeData AbsoluteTimeValue;
  tsn_beacon_payload_s BeaconPayload;
};
typedef struct BeaconInformation  tsn_beacon_information_s;

#define tsn_dlpdu_dllhdr_min_len 7

void tsn_dlpdu_dllhdr_print(tsn_dlpdu_dllhdr_s *n);
tsn_err_e tsn_dlpdu_process_dll(tsn_msg_s *msg);
  
#endif
