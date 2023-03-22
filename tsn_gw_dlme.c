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

/* DLME     ---> Data link-layer management entity 
 * DLME-SAP ---> Data link-layer management service access point
 */

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.2, Page 59
 *                   NETWORK discovery 
 *
 * used by field equipments to find WIA-FA network
 *
 ************************************************************************************
 *
 *  FD-DMAP                         FD-DLL              FD-PHY
 *     |                              |                   |
 *     |                              |                   |
 *     |DLME_discovery_request -----> |                   | 
 *     |                              |                   |
 *     |                              |   Trigger PHY     |
 *     |                              | Channel Scanning  |
 *     |<----- DLME_discovery_confirm |                   |
 *     |                              |                   |
 ***********************************************************************************/
static tsn_boolean_e DLME_discovery_request(BitField24 bitMap)
{
}

enum{
  DLME_DISCOVERY_CONFIRM_SUCCESS=0,
  DLME_DISCOVERY_CONFIRM_NO_BEACHON,
};
struct BeaconDescription{
  Unsigned24 ChannelIndex;
  uint16_t BeaconRelativeTimeslotNum;
  uint8_t ED;
};
struct DlmeDiscoveryConfirm{
  uint8_t  Status;
  uint8_t  BeaconCount;
  uint16_t SuperframeLength;
  uint16_t TimeslotDuration;
  uint16_t FirstSharedTimeslotNumber;
  uint8_t  SharedTimeslotNumber;
  TimeData AbsoluteTimeValue;
  struct BeaconDescription BeaconDescriptionList[0];
};
static tsn_boolean_e DLME_discovery_confirm(struct DlmeDiscoveryConfirm *cfm)
{
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.3, Page 61
 *                   TIME synchronization 
 *
 * used by field equipments to send dual-direction time synchronization request to AD
 *
 ************************************************************************************
 *
 *  FD-DMAP                 FD-DLL              AD-DLL                    AD-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-TIME-SYNC.request|                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      |   dual-direction  |                         |
 *     |                      |    time-sync req  |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |        GACK       |                         |
 *     |                      | <-----------------|DLME-TIME-SYNC.indication|
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   | DLME-TIME-SYNC.response |
 *     |                      |   dual-direction  |<------------------------|
 *     |                      |   time-sync ack   |                         |
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-TIME-SYNC.confirm|                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/
static tsn_boolean_e DLME_timesync_request(struct DlmeTimeSyncRequest *req)
{
}
static tsn_boolean_e DLME_timesync_response(struct DlmeTimeSyncResponse *rsp)
{
}
static tsn_boolean_e DLME_timesync_indication(struct DlmeTimeSyncIndication *ind)
{
}
static tsn_boolean_e DLME_timesync_confirm(struct DlmeTimeSyncResponse *cfm)
{
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.4, Page 63
 *                   DEVICE join
 *
 * used by field equipments to send WIA-FA join request
 *
 ************************************************************************************
 *
 *  FD-DMAP                 FD-DLL              AD-DLL                    GW-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |   DLME-JOIN.request  |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      | join request frame|                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |        GACK       |                         |
 *     |                      | <-----------------|  DLME-JOIN.indication   |
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   |   DLME-JOIN.response    |
 *     |                      |  dual-direction   |<------------------------|
 *     |                      |  time-sync ack    |                         |
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |   DLME-JOIN.confirm  |                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/
static tsn_boolean_e DLME_join_request(struct DlmeJoinRequest *req)
{
}
static tsn_boolean_e DLME_join_indication(struct DlmeJoinIndication *ind)
{
}
static tsn_boolean_e DLME_join_response(struct DlmeJoinResponse *rsp)
{
}
static tsn_boolean_e DLME_join_confirm(struct DlmeJoinConfirm *cfm)
{
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.5, Page 63
 *                   DEVICE status
 *
 * used by field equipments to report device status periodically
 *
 ************************************************************************************
 *
 *  FD-DMAP                 FD-DLL              AD-DLL                    GW-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-FD-STATUS.request|                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      |device status frame|                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |        NACK       |                         |
 *     |                      | <-----------------|DLME-FD-STATUS.indication|
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-FD-STATUS.confirm|                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/
static tsn_boolean_e 
DLME_device_status_request(struct DlmeDeviceStatusRequest *req)
{
}
static tsn_boolean_e 
DLME_device_status_indication(struct DlmeDeviceStatusIndication *ind)
{
}
static tsn_boolean_e 
DLME_device_status_confirm(struct DlmeDeviceStatusConfirm *cfm)
{
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.6, Page 68
 *                   CHANNEL condition
 *
 * used by field equipments to report WIA-FA channel condition
 *
 ************************************************************************************
 *
 *  FD-DMAP                 FD-DLL              AD-DLL                    GW-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-CHANNEL-CONDITION|                   |                         |
 *     |       .request       |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      | channel condition |                         |
 *     |                      |       frame       |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |        NACK       |                         |
 *     |                      | <-----------------|  DLME-CHANNEL-CONDITION |
 *     |                      |                   |       .indication       |
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-CHANNEL-CONDITION|                   |                         |
 *     |       .confirm       |                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/

static tsn_boolean_e
DLME_channel_condition_request(struct DlmeChannelConditionRequest *req)
{
}
static tsn_boolean_e 
DLME_channel_condition_indication(struct DlmeChannelConditionIndication *ind)
{
}
static tsn_boolean_e
DLME_device_status_confirm(struct DlmeChannelConditionConfirm *cfm)
{
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.7, Page 69
 *                   INFORMATION get
 *
 * used by Gateway device to get attributes from field devices
 *
 ************************************************************************************
 *
 *  GW-DMAP                 AD-DLL              FD-DLL                    FD-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-INFO-GET.request |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      | information get   |                         |
 *     |                      |  request frame    |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   | DLME-INFO-GET.indication|
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   |  DLME-INFO-GET.response |
 *     |                      |                   |<------------------------|
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |  information get  |                         |
 *     |                      |   response frame  |                         |
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |        GACK       |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |DLME-INFO-GET.confirm |                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/

static tsn_boolean_e
DLME_information_get_request(struct DlmeInformationGetRequest *req)
{
}
static tsn_boolean_e
DLME_information_get_indication(struct DlmeInformationGetIndication *ind)
{
}
static tsn_boolean_e 
DLME_information_get_response(struct DlmeInformationGetResponse *rsp)
{
}
static tsn_boolean_e 
DLME_information_get_confirm(struct DlmeInformationGetConfirm *cfm)
{
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.3.9, Page 75
 *                   LEAVE
 *
 * used by Gateway device to send LEAVE request to field devices
 *
 ************************************************************************************
 *
 *  GW-DMAP                 AD-DLL              FD-DLL                    FD-DMAP
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |  DLME-LEAVE.request  |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      |   device leave    |                         |
 *     |                      |   request frame   |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   |  DLME-LEAVE.indication  |
 *     |                      |                   |------------------------>|
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |DLME-LEAVE.response|                         |
 *     |                      |<------------------|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |  DLME-LEAVE.confirm  |                   |                         |
 *     |<-------------------- |                   |                         |
 *     |                      |                   |                         |
 ***********************************************************************************/
static tsn_boolean_e DLME_leave_request(struct DlmeLeaveRequest *req)
{
}
/*
struct DlmeLeaveIndication{
};
*/
static tsn_boolean_e DLME_leave_indication(void *ind)
{
}
/*
struct DlmeLeaveResponse{
};
*/
static tsn_boolean_e DLME_leave_response(void *rsp)
{
}
enum{
  DLME_leave_confirm_SUCCESS = 0,
  DLME_leave_confirm_FAILURE
};
struct DlmeLeaveConfirm{
  uint8_t Status;
};
static tsn_boolean_e DLME_leave_confirm(struct DlmeLeaveConfirm *cfm)
{
}
  
#endif
