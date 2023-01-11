#ifndef _TSN_GW_DLDE_H__
#define _TSN_GW_DLDE_H__

/****************************************************************
* GB/T26790.2-2015, 8.2, Page 56
*					Data Link Layer Data Service
****************************************************************/
#define TSN_DLDE_DATA_TRANSMIT_REQUEST_DataType_DATA 0
#define TSN_DLDE_DATA_TRANSMIT_REQUEST_DataType_NACK 1
#define TSN_DLDE_DATA_TRANSMIT_REQUEST_DataType_GACK 2

#define TSN_DLDE_DATA_REQUEST_Priority_RT0 0
#define TSN_DLDE_DATA_REQUEST_Priority_RT1 1
#define TSN_DLDE_DATA_REQUEST_Priority_RT2 2
#define TSN_DLDE_DATA_REQUEST_Priority_RT3 3
#define TSN_DLDE_DATA_REQUEST_Priority_NRT 4

/***********************************************************************************
 * GB/T26790.2-2015, 8.2.4, Page 57
 *                   Data Service Timings
 *
 * The basic process of data transmission, receiving and confirming
 *
 ************************************************************************************
 *
 *  GW-ASL                  AD-DLL              AD-DLL                    FD-ASL
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |   DLDE-DATA.request  |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      |     Data Frame    |                         |
 *     |                      |     GACK/NACK     |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |   DLDE-DATA.indication  |
 *     |                      |         GACK      |------------------------>|
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *
 *                                ##############
 *                                #  GW---->FD #
 *                                ##############
 *
 *
 *  FD-ASL                  FD-DLL              AD-DLL                    AD-ASL
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |   DLDE-DATA.request  |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      |  Periodical Data  |                         |
 *     |                      |        Frame      |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |   DLDE-DATA.indication  |
 *     |                      |         NACK      |------------------------>|
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *
 *                                ##############
 *                                #  FD---->GW #
 *                                ##############
 *
 ************************************************************************************
 *
 *  FD-ASL                  FD-DLL              AD-DLL                    AD-ASL
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |   DLDE-DATA.request  |                   |                         |
 *     |   -----------------> |                   |                         |
 *     |                      |     Other Data    |                         |
 *     |                      |        Frame      |                         |
 *     |                      | ----------------->|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |   DLDE-DATA.indication  |
 *     |                      |         GACK      |------------------------>|
 *     |                      | <-----------------|                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *     |                      |                   |                         |
 *
 *                                ##############
 *                                #  FD---->GW #
 *                                ##############
 *
 ***********************************************************************************/
#define TSN_DLDE_DATA_Request_Priority_RT0 0
#define TSN_DLDE_DATA_Request_Priority_RT1 1
#define TSN_DLDE_DATA_Request_Priority_RT2 2
#define TSN_DLDE_DATA_Request_Priority_RT3 3
#define TSN_DLDE_DATA_Request_Priority_NRT 4
struct DldeDataTransmitRequest{
	Unsigned16  DstAddr;
	Unsigned16  VCR_ID;
	Unsigned8   DataType;
	Unsigned8   Priority;
	Unsigned16  PayloadLength;
	OctetString Payload;
};
typedef struct DldeDataTransmitRequest tsn_dlde_data_transmit_request_s;

#define TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_DATA 0
#define TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_NACK 1
#define TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_GACK 2
struct DldeDataTransmitIndication{
	Unsigned16  srcAddr;
	Unsigned8   DataType;
	Unsigned16  PayloadLength;
	OctetString Payload;
};
typedef struct DldeDataTransmitIndication tsn_dlde_data_transmit_indication_s;

static inline const char *
get_DLDE_DataType(int type)
{
  switch (type)
  {
    case TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_DATA:
      return "DATA";
    case TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_NACK:
      return "NACK";
    case TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_GACK:
      return "GACK";
    default:
      return "UNKNOWN";
  }
}

static inline const char *
get_DLDE_DataPriority(unsigned int priority)
{
  static const char *priStr[]={
      "RT0","RT1","RT2","RT3","NRT"
    };
  if (priority <= TSN_DLDE_DATA_Request_Priority_NRT)
    return priStr[priority];
  else
    return "UNKNOWN";
}

#endif
