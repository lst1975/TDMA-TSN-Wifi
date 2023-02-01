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
#ifndef _TSN_FRAME_DLL_H_
#define _TSN_FRAME_DLL_H_

/***********************************************************************************
 * GB/T26790.2-2015, 8.4.1, Page 78
 *                   General DLL frame format
 *
 ***********************************************************************************/

#define FRAME_TYPE_Beacon                            0x00000
#define FRAME_TYPE_Data                              0x00001
#define FRAME_TYPE_Aggregation                       0x00010
#define FRAME_TYPE_GACK                              0x00011
#define FRAME_TYPE_NACK                              0x00100
#define FRAME_TYPE_JoinRequest                       0x00101
#define FRAME_TYPE_JoinResponse                      0x00110
#define FRAME_TYPE_LeaveRequest                      0x00111
#define FRAME_TYPE_LeaveResponse                     0x01000
#define FRAME_TYPE_DeviceStatusReport                0x01001
#define FRAME_TYPE_ChannelConditionReport            0x01010
#define FRAME_TYPE_TwoWayTimeSynchronizationRequest  0x01011
#define FRAME_TYPE_TwoWayTimeSynchronizationResponse 0x01100
#define FRAME_TYPE_RemoteAttributeGetRequest         0x01101
#define FRAME_TYPE_RemoteAttributeGetResponse        0x01110
#define FRAME_TYPE_RemoteAttributeSetRequest         0x01111
#define FRAME_TYPE_RemoteAttributeSetResponse        0x10000
#define FRAME_TYPE_KeyEstablishRequest               0x10001
#define FRAME_TYPE_KeyEstablishResponse              0x10010
#define FRAME_TYPE_KeyUpdateRequest                  0x10011
#define FRAME_TYPE_KeyUpdateResponse                 0x10100
#define FRAME_TYPE_SecurityAlarm                     0x10101
#define FRAME_TYPE_MAX                               0x10110

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
  tsn_addr_u addr;
  Unsigned16 seq;
  Unsigned8  segment_count;
  Unsigned8  segment_seq;
  Unsigned16 length;
};
typedef struct tsn_dlpdu_dllhdr tsn_dlpdu_dllhdr_s;

#define tsn_dlpdu_dllhdr_min_len 7

void tsn_dlpdu_dllhdr_print(tsn_dlpdu_dllhdr_s *n);
  
#endif
