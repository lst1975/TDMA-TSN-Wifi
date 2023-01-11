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
		short_addr_u ShortAddr;
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

#endif
