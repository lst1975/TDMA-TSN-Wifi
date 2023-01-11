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
*					Cable Service Between GW and AD
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
	TSN_DLME_INFO_SET_request,		  /* gw --> ad */
	TSN_DLME_INFO_SET_confirm,	    /* ad --> gw */
	TSN_DLME_LEAVE_request,	        /* gw --> ad */
	TSN_KEY_ESTABLISH_request,      /* gw --> ad */
	TSN_KEY_ESTABLISH_confirm,	    /* ad --> gw */
	TSN_KEY_UPDATE_request,         /* gw --> ad */
	TSN_KEY_UPDATE_confirm,	        /* ad --> gw */
	TSN_SECURITY_ALARM_indication,  /* ad --> gw */
  TSN_dlpdu_type_max,             
};

struct tsn_gw_dlpdu_normal{
	Unsigned8   ServID;
	Unsigned8   AdID;
	Unsigned64  ADAddr;
	Unsigned16  AttrLen;
	Unsigned8  *Attrs;
};
typedef struct tsn_gw_dlpdu_normal tsn_gw_dlpdu_normal_s;

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

struct GAckInformation_addru8{
	Unsigned8  DstAddr8;
	Unsigned16 SequenceNumber;
};
typedef struct GAckInformation_addru8 tsn_gack_info_addru8_s;

struct GAckInformation_addru16{
	Unsigned16 DstAddr16;
	Unsigned16 SequenceNumber;
};
typedef struct GAckInformation_addru16 tsn_gack_info_addru16_s;

struct GAckInformation_addru64{
	Unsigned64 DstAddr64;
	Unsigned16 SequenceNumber;
};
typedef struct GAckInformation_addru64 tsn_gack_info_addru64_s;

struct GAckIndication_addru8 {
	Unsigned8       DeviceCount;
	tsn_gack_info_addru8_s GAckInfo[0];
};
struct GAckIndication_addru16 {
	Unsigned8       DeviceCount;
	tsn_gack_info_addru16_s GAckInfo[0];
};
struct GAckIndication_addru64 {
	Unsigned8       DeviceCount;
	tsn_gack_info_addru64_s GAckInfo[0];
};
typedef struct GAckIndication_addru8  tsn_gack_indication_addru8_s;
typedef struct GAckIndication_addru16 tsn_gack_indication_addru16_s;
typedef struct GAckIndication_addru64 tsn_gack_indication_addru64_s;

struct NAckIndication_addru8 {
	Unsigned8 RetryDeviceCount;
	Unsigned8 DstAddressList[0];
};
struct NAckIndication_addru16 {
	Unsigned8  RetryDeviceCount;
	Unsigned16 DstAddressList[0];
};
typedef struct NAckIndication_addru8  tsn_nack_indication_addru8_s;
typedef struct NAckIndication_addru16 tsn_nack_indication_addru16_s;

#endif
