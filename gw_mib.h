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
#ifndef _GW_MIB_H_
#define _GW_MIB_H_

struct DsnDmapMibGetRequest{
  Unsigned8 Handle;
  Unsigned8 NetworkID;
  Unsigned16 ShortAddr;
  Unsigned8  AttributeID;
  Unsigned8  MemberID;
  Unsigned16 FirstStoreIndex;
  Unsigned16 Count;
  Unsigned8  IndexValue[0];
};
typedef struct DsnDmapMibGetRequest TsnDmapMibGetRequestS;
#define INVALID_STORE_INDEX Unsigned16Max

struct TsnDmapMibGetConfirm{
  Unsigned8 Handle;
  Unsigned8  Status;
  Unsigned16 Count;
  Unsigned8  AttributeValue[0];
};
typedef struct TsnDmapMibGetConfirm TsnDmapMibGetConfirmS;
enum{
  DMAP_mib_get_confirm_SUCCESS=0,
  DMAP_mib_get_confirm_INVALID_ATTRIBUTE,
  DMAP_mib_get_confirm_INVALID_ATTRIBUTE_MEMBER,
  DMAP_mib_get_confirm_INVALID_RANGE,
  DMAP_mib_get_confirm_PERMISSION,
};

struct DmapMibSetRequest{
  Unsigned8 Handle;
  Unsigned16 ShortAddr;
  Unsigned8  AttributeID;
  Unsigned8  MemberID;
  Unsigned16 FirstStoreIndex;
  Unsigned16 Count;
  Unsigned8  AttributeValue[0];
};
typedef struct DmapMibSetRequest TsnDmapMibSetRequestS;
#define DmapMibSetRequest_FirstStoreIndex_Type 0xF000
#define DmapMibSetRequest_FirstStoreIndex_SuperframeID 1
#define DmapMibSetRequest_FirstStoreIndex_LinkID       2
#define DmapMibSetRequest_FirstStoreIndex_ChannelID    3
#define DmapMibSetRequest_FirstStoreIndex_KeyID        4
#define DmapMibSetRequest_FirstStoreIndex_VcrID        5
#define DmapMibSetRequest_FirstStoreIndex_ClassID      6
#define DmapMibSetRequest_FirstStoreIndex_UAOID        7

static inline int DMAP_mib_set_request(void *req)
{
  struct DmapMibSetRequest *r = (struct DmapMibSetRequest *)req;
}
struct DmapMibSetConfirm{
  Unsigned8 Handle;
  Unsigned8 Status;
};
typedef struct DmapMibSetConfirm TsnDmapMibSetConfirmS;
enum{
  DMAP_mib_set_confirm_SUCCESS=0,
  DMAP_mib_set_confirm_INVALID_ATTRIBUTE,
  DMAP_mib_set_confirm_INVALID_ATTRIBUTE_MEMBER,
  DMAP_mib_set_confirm_INVALID_VALUE,
  DMAP_mib_set_confirm_PERMISSION,
};
static inline int DMAP_mib_get_confirm(void *cfm)
{
  struct DmapMibSetConfirm *c = (struct DmapMibSetConfirm *)cfm;
}

#define DMAP_mib_type_none      0x00
#define DMAP_mib_type_contant   0x01
#define DMAP_mib_type_static    0x02
#define DMAP_mib_type_dynamic   0x04
#define DMAP_mib_type_mandatory 0x08
#define DMAP_mib_type_optional  0x10

#define DMAP_mib_device_all (DMAP_mib_device_gw|DMAP_mib_device_fd|DMAP_mib_device_ad)
#define DMAP_mib_device_none  0x0
#define DMAP_mib_device_gw    0x1
#define DMAP_mib_device_fd    0x2
#define DMAP_mib_device_ad    0x4

#define DMAP_mib_permission_none  0x00
#define DMAP_mib_permission_write 0x01
#define DMAP_mib_permission_read  0x02

typedef struct TSN_DMAP_mib_attribute TSN_DMAP_mib_attribute_s;

#define TSN_mib_get_type_check  0
#define TSN_mib_get_type_data   1

typedef tsn_boolean_e (*tsn_mib_get_f)(
  unsigned short networkID, 
  unsigned short memberID,
  unsigned short firstIndex,
  unsigned short count, 
  void **data
);

typedef tsn_boolean_e (*tsn_mib_set_f)(
  unsigned short networkID, 
  unsigned short memberID, 
  unsigned short firstIndex,
  unsigned short count, 
  void *data, 
  unsigned short *len
);

struct TSN_DMAP_mib_attribute{
  const char *Name;
  const char *Description;
  
  uint64_t AttributeID:10;
  uint64_t DefaultEntryCount:7;
  uint64_t MibType:5;
  uint64_t DataType:5;
  uint64_t Access:2;
  uint64_t Device:4;
  uint64_t Offset:31;

  union{
    Unsigned8     DefaultValue_Unsigned8;
    Unsigned16    DefaultValue_Unsigned16;
    Unsigned24    DefaultValue_Unsigned24;
    Unsigned32    DefaultValue_Unsigned32;
    Unsigned40    DefaultValue_Unsigned40;
    Unsigned48    DefaultValue_Unsigned48;
    Unsigned64    DefaultValue_Unsigned64;
    Unsigned80    DefaultValue_Unsigned80;
    Unsigned8     DefaultValue_BitField8;
    Unsigned16    DefaultValue_BitField16;
    Unsigned24    DefaultValue_BitField24;
    TimeData      DefaultValue_TimeData;
    KeyData       DefaultValue_KeyData;
    OctetString   DefaultValue_OctetString;
    BitsString    DefaultValue_BitsString;
    double   DefaultValue_SingleFloat;
    double   DefaultValue_DoubleFloat;
  };
  
  union{
    Unsigned8     ValueMin_Unsigned8;
    Unsigned16    ValueMin_Unsigned16;
    Unsigned24    ValueMin_Unsigned24;
    Unsigned32    ValueMin_Unsigned32;
    Unsigned40    ValueMin_Unsigned40;
    Unsigned48    ValueMin_Unsigned48;
    Unsigned64    ValueMin_Unsigned64;
    Unsigned80    ValueMin_Unsigned80;
    Unsigned8     ValueMin_BitField8;
    Unsigned16    ValueMin_BitField16;
    Unsigned24    ValueMin_BitField24;
    TimeData      ValueMin_TimeData;
    KeyData       ValueMin_KeyData;
    OctetString   ValueMin_OctetString;
    BitsString    ValueMin_BitsString;
    double   ValueMin_SingleFloat;
    double   ValueMin_DoubleFloat;
  };
  
  union{
    Unsigned8     ValueMax_Unsigned8;
    Unsigned16    ValueMax_Unsigned16;
    Unsigned24    ValueMax_Unsigned24;
    Unsigned32    ValueMax_Unsigned32;
    Unsigned40    ValueMax_Unsigned40;
    Unsigned48    ValueMax_Unsigned48;
    Unsigned64    ValueMax_Unsigned64;
    Unsigned80    ValueMax_Unsigned80;
    Unsigned8     ValueMax_BitField8;
    Unsigned16    ValueMax_BitField16;
    Unsigned24    ValueMax_BitField24;
    TimeData      ValueMax_TimeData;
    KeyData       ValueMax_KeyData;
    OctetString   ValueMax_OctetString;
    BitsString    ValueMax_BitsString;
    double   ValueMax_SingleFloat;
    double   ValueMax_DoubleFloat;
  };
  
  tsn_mib_get_f MibGet;
  tsn_mib_set_f MibSet;
  TSN_DMAP_mib_attribute_s *DefaultEntryAddr;
  union{
    TSN_DMAP_mib_attribute_s **value_List;
    TSN_DMAP_mib_attribute_s *value_Attrs;
    Unsigned8     value_Unsigned8;
    Unsigned16    value_Unsigned16;
    Unsigned24    value_Unsigned24;
    Unsigned32    value_Unsigned32;
    Unsigned40    value_Unsigned40;
    Unsigned48    value_Unsigned48;
    Unsigned64    value_Unsigned64;
    Unsigned80    value_Unsigned80;
    Unsigned8     value_BitField8;
    Unsigned16    value_BitField16;
    Unsigned24    value_BitField24;
    TimeData      value_TimeData;
    KeyData       value_KeyData;
    OctetString   value_OctetString;
    BitsString    value_BitsString;
    double        value_SingleFloat;
    double        value_DoubleFloat;
  }Value;
};

static inline void *
TSN_DMAP_mib_entry_get_data(TSN_DMAP_mib_attribute_s *mib)
{
  switch (mib->DataType)
  {
    case DATA_TYPE_TimeData:
      return &mib->Value.value_TimeData;
    case DATA_TYPE_Unsigned8:
      return &mib->Value.value_Unsigned8;
    case DATA_TYPE_Unsigned16:
      return &mib->Value.value_Unsigned16;
    case DATA_TYPE_Unsigned24:
      return &mib->Value.value_Unsigned24;
    case DATA_TYPE_Unsigned32:
      return &mib->Value.value_Unsigned32;
    case DATA_TYPE_Unsigned48:
      return &mib->Value.value_Unsigned48;
    case DATA_TYPE_Unsigned64:
      return &mib->Value.value_Unsigned64;
    case DATA_TYPE_Unsigned80:
      return &mib->Value.value_Unsigned80;
    case DATA_TYPE_SingleFloat:
      return &mib->Value.value_SingleFloat;
    case DATA_TYPE_DoubleFloat:
      return &mib->Value.value_DoubleFloat;
    case DATA_TYPE_OctetString:
      return &mib->Value.value_OctetString;
    case DATA_TYPE_BitsString:
      return &mib->Value.value_BitsString;
    case DATA_TYPE_KeyData:
      return &mib->Value.value_KeyData;
    case DATA_TYPE_List:
      return mib->Value.value_List;
    case DATA_TYPE_Struct:
      return mib->Value.value_Attrs;
    default:
      return NULL;
  }
}

#define __DECL_MIB_ATTR_OFFSET(prefix,id,dataType,mi,mx,permission,\
  mibType,dftValue,devices,description,offset,get,set) { \
  .Name         = #id, \
  .Description  = description, \
  .AttributeID  = prefix ## id, \
  .MibType      = mibType, \
  .DataType     = DATA_TYPE_ ## dataType, \
  .Access       = permission, \
  .Device       = devices, \
  .ValueMin_##dataType     = mi, \
  .ValueMax_##dataType     = mx, \
  .Value.value_##dataType  = dftValue, \
  .DefaultValue_##dataType = dftValue, \
  .DefaultEntryAddr   = NULL, \
  .DefaultEntryCount  = 0, \
  .Offset             = offset, \
  .MibGet             = get, \
  .MibSet             = set, \
}
    
#define __DECL_MIB_ATTR_OFFSET_LIST(defaultEntry,prefix,id,dataType,mi,mx,\
  permission,mibType,dftValue,devices,description,offset,get,set) { \
  .Name         = #id, \
  .Description  = description, \
  .AttributeID  = prefix ## id, \
  .MibType      = mibType, \
  .DataType     = DATA_TYPE_ ## dataType, \
  .Access       = permission, \
  .Device       = devices, \
  .DefaultEntryAddr   = defaultEntry, \
  .DefaultEntryCount  = sizeof(defaultEntry)/sizeof(TSN_DMAP_mib_attribute_s), \
  .Offset             = offset, \
  .MibGet             = get, \
  .MibSet             = set, \
}
        
static inline tsn_boolean_e
TSN_DMAP_mib_entry_isreadable(const TSN_DMAP_mib_attribute_s *mib)
{
  return (mib->Access & DMAP_mib_permission_read) ? TSN_TRUE : TSN_FALSE;
}
static inline tsn_boolean_e
TSN_DMAP_mib_entry_iswritable(const TSN_DMAP_mib_attribute_s *mib)
{
  return (mib->Access & DMAP_mib_permission_write) ? TSN_TRUE : TSN_FALSE;
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.7, Page 32 
 *                   Management Information and Services 
 *
 * GB/T26790.2-2015, 6.7.1.2.1, Page 33 
 *                   Non-Struct Attributes 
 ************************************************************************************/
#define DMAP_mib_id_static_MIN                                0
#define DMAP_mib_id_static_PriGwFailureTime                  (DMAP_mib_id_static_MIN+ 0)
#define DMAP_mib_id_static_AddressTypeFlag                   (DMAP_mib_id_static_MIN+ 1) 
  #define DMAP_mib_id_static_AddressTypeFlag_u8                 0
  #define DMAP_mib_id_static_AddressTypeFlag_u16                1 
  #define DMAP_mib_id_static_AddressTypeFlag_u64                2 
  #define DMAP_mib_id_static_AddressTypeFlag_NONE               3 
#define DMAP_mib_id_static_MaxPayloadLength                  (DMAP_mib_id_static_MIN+ 2) 
#define DMAP_mib_id_static_NACKCount                         (DMAP_mib_id_static_MIN+ 3) 
#define DMAP_mib_id_static_NetworkID                         (DMAP_mib_id_static_MIN+ 4) 
#define DMAP_mib_id_static_BitMap                            (DMAP_mib_id_static_MIN+ 5) 
  #define DMAP_mib_id_static_BitMap_Modulation_method_MASK            0x00000F
  #define DMAP_mib_id_static_BitMap_Channel_status_MASK               0x03FFF0 
#define DMAP_mib_id_static_DeviceStatusReportCycle           (DMAP_mib_id_static_MIN+ 6) 
#define DMAP_mib_id_static_ChannelStatusReportCycle          (DMAP_mib_id_static_MIN+ 7) 
#define DMAP_mib_id_static_LossConnectDuration               (DMAP_mib_id_static_MIN+ 8) 
#define DMAP_mib_id_static_KeyUpdateDuration                 (DMAP_mib_id_static_MIN+ 9) 
#define DMAP_mib_id_static_TimeSlotDuration                  (DMAP_mib_id_static_MIN+10) 
#define DMAP_mib_id_static_TwoWayTimeSynchronization         (DMAP_mib_id_static_MIN+11) 
#define DMAP_mib_id_static_TwoWayTimeSynchronizationTimeout  (DMAP_mib_id_static_MIN+12) 
#define DMAP_mib_id_static_AccessDeviceTeamNumber            (DMAP_mib_id_static_MIN+13) 
#define DMAP_mib_id_static_ExpectedLossRate                  (DMAP_mib_id_static_MIN+14) 
#define DMAP_mib_id_static_LossRate                          (DMAP_mib_id_static_MIN+15) 
#define DMAP_mib_id_static_MaxRetry                          (DMAP_mib_id_static_MIN+16) 
#define DMAP_mib_id_static_SecurityLevel                     (DMAP_mib_id_static_MIN+17) 
  #define DMAP_mib_id_static_SecurityLevel_None                                0
  #define DMAP_mib_id_static_SecurityLevel_Authentication                     1
  #define DMAP_mib_id_static_SecurityLevel_Authentication_MIC32               2
  #define DMAP_mib_id_static_SecurityLevel_Authentication_MIC64               3
  #define DMAP_mib_id_static_SecurityLevel_Authentication_MIC128              4
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription          5
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription_MIC32    6
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription_MIC64    7
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription_MIC128   8
#define DMAP_mib_id_static_AttackStatisticsDuration          (DMAP_mib_id_static_MIN+18) 
#define DMAP_mib_id_static_MaxKeyAttackedNumber              (DMAP_mib_id_static_MIN+19) 
#define DMAP_mib_id_static_AlarmReportDuration               (DMAP_mib_id_static_MIN+20) 
#define DMAP_mib_id_static_ChannelNumber                     (DMAP_mib_id_static_MIN+21) 
#define DMAP_mib_id_static_AggregationEnableFlag             (DMAP_mib_id_static_MIN+22) 
#define DMAP_mib_id_static_MAX                               (DMAP_mib_id_static_MIN+23) 

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.1, Page 35 
 *                   Struct Attributes 
 *
 ************************************************************************************/
#define DMAP_mib_id_list_MIN                                  128 
#define DMAP_mib_id_list_SuperframeList                      (DMAP_mib_id_list_MIN+0) 
#define DMAP_mib_id_list_DllLinkList                         (DMAP_mib_id_list_MIN+1) 
#define DMAP_mib_id_list_ChannelConditionList                (DMAP_mib_id_list_MIN+2) 
#define DMAP_mib_id_list_DeviceList                          (DMAP_mib_id_list_MIN+3) 
#define DMAP_mib_id_list_KeyList                             (DMAP_mib_id_list_MIN+4) 
#define DMAP_mib_id_list_VCRList                             (DMAP_mib_id_list_MIN+5) 
#define DMAP_mib_id_list_SupportedUAOList                    (DMAP_mib_id_list_MIN+6) 
#define DMAP_mib_id_list_ConfiguredUAOList                   (DMAP_mib_id_list_MIN+7) 
#define DMAP_mib_id_list_MAX                                 (DMAP_mib_id_list_MIN+8) 

#define DMAP_mib_id_INVALID                                   -1
#define DMAP_mib_id_ALL                                       255

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 35 
 *           Definations of Superframe Struct Attributes
 *
 ************************************************************************************/
#define DMAP_mib_id_superframe_SuperframeID                      0
#define DMAP_mib_id_superframe_NumberSlots                       1 
#define DMAP_mib_id_superframe_ActiveFlag                        2 
#define DMAP_mib_id_superframe_ActiveSlot                        3 

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 36 
 *           Link Struct Attributes
 *
 ************************************************************************************/
#define DMAP_mib_id_link_LinkID                            0
#define DMAP_mib_id_link_LinkType                          1 
  #define DMAP_mib_id_link_LinkType_TxType_Unicast                   0x00 
  #define DMAP_mib_id_link_LinkType_TxType_Broadcast                 0x80 
#define DMAP_mib_id_link_LinkType_TxType_MASK                        0x80

  #define DMAP_mib_id_link_LinkType_TxDirection_Transmit             0x00 
  #define DMAP_mib_id_link_LinkType_TxDirection_SharedTransmission   0x20
  #define DMAP_mib_id_link_LinkType_TxDirection_Retransmit           0x40
  #define DMAP_mib_id_link_LinkType_TxDirection_Receive               0x60
#define DMAP_mib_id_link_LinkType_TxDirection_MASK                   0x60

  #define DMAP_mib_id_link_LinkType_TimeslotType_Beacon                   0x00
  #define DMAP_mib_id_link_LinkType_TimeslotType_NACK                    0x04
  #define DMAP_mib_id_link_LinkType_TimeslotType_GACK                    0x08
  #define DMAP_mib_id_link_LinkType_TimeslotType_ManagementTimeslot       0x0C
  #define DMAP_mib_id_link_LinkType_TimeslotType_DataTimeslot            0x10
  #define DMAP_mib_id_link_LinkType_TimeslotType_ManagementDataTimeslot  0x14
#define DMAP_mib_id_link_LinkType_TimeslotType_MASK                      0x1C

#define DMAP_mib_id_link_ActiveSlot                        2 
#define DMAP_mib_id_link_PeerAddr                          3 
#define DMAP_mib_id_link_RelativeSlotNumber                4
#define DMAP_mib_id_link_ChannelIndex                      5
#define DMAP_mib_id_link_SuperframeID                      6

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 35 
 *                   Channel Condition Struct Attributes 
 *
 ************************************************************************************/
#define DMAP_mib_id_channel_condition_ChannelID                         0
#define DMAP_mib_id_channel_condition_LinkQuality                       1 
#define DMAP_mib_id_channel_condition_PacketLossRate                    2 
#define DMAP_mib_id_channel_condition_RetryNumber                       3 

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 37 
 *                   Device Struct Attributes 
 *
 ************************************************************************************/
#define DMAP_mib_id_device_Version                           0
#define DMAP_mib_id_device_LongAddress                       1 
#define DMAP_mib_id_device_AggregationSupportFlag            2 
#define DMAP_mib_id_device_NumberOfSuperframeUAO             3 
#define DMAP_mib_id_device_NumberOfConfiguredUAO             4 
#define DMAP_mib_id_device_TransmitDelay                     5 
#define DMAP_mib_id_device_ProbeTime                         6 
#define DMAP_mib_id_device_TimeValue                         7 
#define DMAP_mib_id_device_RedundantDeviceFlag               8 
#define DMAP_mib_id_device_AccessDeviceID                    9 
#define DMAP_mib_id_device_DeviceShortAddress                10 
#define DMAP_mib_id_device_PowerSupplyStatus                 11 
  #define DMAP_mib_id_device_PowerSupplyStatus_Fixed             0 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel1         1 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel2         2 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel3         3 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel4         4 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel5         5 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel6         6 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel7         7 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel8         8 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel9         9 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel10        10 
  #define DMAP_mib_id_device_PowerSupplyStatus_MAX               11 
#define DMAP_mib_id_device_DeviceState                       12 
  #define DMAP_mib_id_device_DeviceState_Detached            0 
  #define DMAP_mib_id_device_DeviceState_Joining             1 
  #define DMAP_mib_id_device_DeviceState_Authenticating      2 
  #define DMAP_mib_id_device_DeviceState_Configuring         3 
  #define DMAP_mib_id_device_DeviceState_AllocatingResources 4 
  #define DMAP_mib_id_device_DeviceState_Running             5 
  #define DMAP_mib_id_device_DeviceState_MAX                 6 
#define DMAP_mib_id_device_MAX                               12 
static inline const char *dlmeDeviceState2String(unsigned int state)
{
  const char *DeviceState[] = {
    "Detached", "Joining", "Authenticating", "Configuring", "AllocatingResources", "Running",
  };
  if (state < DMAP_mib_id_device_DeviceState_MAX)
    return DeviceState[state];
  else
    return "<Invalid>";
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 37 
 *           Key Struct Attributes 
 *
 ************************************************************************************/
#define DMAP_mib_id_key_KeyID                                0
#define DMAP_mib_id_key_PeerAddr                             1 
#define DMAP_mib_id_key_KeyType                              2 
  #define DMAP_mib_id_key_KeyType_KJ_KEY_JOIN                             0 
  #define DMAP_mib_id_key_KeyType_KS_KEY_SHARED                           1 
  #define DMAP_mib_id_key_KeyType_KEK_KEY_Eencription_KEY                 2 
  #define DMAP_mib_id_key_KeyType_KEDU_Eencription_Key_Data_Unicast       3 
  #define DMAP_mib_id_key_KeyType_KEDB_Eencription_Key_Data_Broadcast     4 
#define DMAP_mib_id_key_KeyDataValue                         3 
#define DMAP_mib_id_key_KeyActiveSlot                        4 
#define DMAP_mib_id_key_KeyAttackCount                       5 
#define DMAP_mib_id_key_AlarmFlag                            6 
  #define DMAP_mib_id_key_AlarmFlag_Alarm_of_KEY_attaction                0 
  #define DMAP_mib_id_key_AlarmFlag_Alarm_of_KEY_update_timeout      1 
#define DMAP_mib_id_key_KeyState                             7 
  #define DMAP_mib_id_key_KeyState_Backup                                 0 
  #define DMAP_mib_id_key_KeyState_Using                                  1 
  #define DMAP_mib_id_key_KeyState_Expired                                2 
  #define DMAP_mib_id_key_KeyState_Invalid                                3 

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 40 
 *                   Process Data Description Struct 
 *
************************************************************************************/
#define DMAP_mib_id_uao_ParameterDataType                  0
  #define DMAP_mib_id_uao_ParameterDataType_Unsigned8      0x8000
  #define DMAP_mib_id_uao_ParameterDataType_Unsigned16     0x4000
  #define DMAP_mib_id_uao_ParameterDataType_Unsigned32     0x2000
  #define DMAP_mib_id_uao_ParameterDataType_SingleFloat    0x1000
  #define DMAP_mib_id_uao_ParameterDataType_DoubleFloat    0x0800
  #define DMAP_mib_id_uao_ParameterDataType_Bitstring      0x0400
  #define DMAP_mib_id_uao_ParameterDataType_Count_MASK     0x03ff

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 39 
 *                   UAO Class Description Struct 
 *
***********************************************************************************/
#define DMAP_mib_id_supported_uao_ClassID                                0
#define DMAP_mib_id_supported_uao_UAOType                                1 
   #define DMAP_mib_id_supported_uao_UAOType_AI                0 
   #define DMAP_mib_id_supported_uao_UAOType_AO                1 
   #define DMAP_mib_id_supported_uao_UAOType_DI                2 
   #define DMAP_mib_id_supported_uao_UAOType_DO                3 
#define DMAP_mib_id_supported_uao_MaxInputDatalen                        2 
#define DMAP_mib_id_supported_uao_MaxOutputDatalen                       3 
#define DMAP_mib_id_supported_uao_MinDataUpdateRate                      4 
#define DMAP_mib_id_supported_uao_SupperInputType                        5 
#define DMAP_mib_id_supported_uao_SupperOutputType                       6 

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 38 
 *           VcrEP Struct 
 *
 ************************************************************************************/
#define DMAP_mib_id_vcrEP_VcrID                                0
#define DMAP_mib_id_vcrEP_VcrType                              1 
  #define DMAP_mib_id_vcrEP_VcrType_CLIENT                              0 
  #define DMAP_mib_id_vcrEP_VcrType_SERVER                1 
  #define DMAP_mib_id_vcrEP_VcrType_PUBLISHER                2 
  #define DMAP_mib_id_vcrEP_VcrType_SUBSCRIBER              3 
  #define DMAP_mib_id_vcrEP_VcrType_REPORT_SOURCE              4 
  #define DMAP_mib_id_vcrEP_VcrType_REPORT_SINK              5
#define DMAP_mib_id_vcrEP_UapID                                2 
#define DMAP_mib_id_vcrEP_PeerAddr                             3 
#define DMAP_mib_id_vcrEP_VcrActiveTime                        4 
#define DMAP_mib_id_vcrEP_DataUpdateRate                       5 
#define DMAP_mib_id_vcrEP_Deadline                             6 
#define DMAP_mib_id_vcrEP_WatchdogTime                         7 
  
/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 40 
 *                   Process Data Description Struct 
 *
 ************************************************************************************/
#define DMAP_mib_id_uao_ParameterDescriptionProcessDataType    0
  #define DMAP_mib_id_uao_ParameterDescriptionProcessDataType_Unsigned8    0x8000
  #define DMAP_mib_id_uao_ParameterDescriptionProcessDataType_Unsigned16   0x4000
  #define DMAP_mib_id_uao_ParameterDescriptionProcessDataType_Unsigned32   0x2000
  #define DMAP_mib_id_uao_ParameterDescriptionProcessDataType_SingleFloat  0x1000
  #define DMAP_mib_id_uao_ParameterDescriptionProcessDataType_DoubleFloat  0x0800
  #define DMAP_mib_id_uao_ParameterDescriptionProcessDataType_Bitstring    0x0400
  #define DMAP_mib_id_uao_ParameterDescriptionIODataType_Count_MASK        0x03ff

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 39 
 *           UAO Class Description Struct 
 *
 ***********************************************************************************/
#define DMAP_mib_id_supported_uao_ClassID                                0
#define DMAP_mib_id_supported_uao_UAOType                                1 
   #define DMAP_mib_id_supported_uao_UAOType_AI                                0 
   #define DMAP_mib_id_supported_uao_UAOType_AO                                1 
   #define DMAP_mib_id_supported_uao_UAOType_DI                                2 
   #define DMAP_mib_id_supported_uao_UAOType_DO                                3 
#define DMAP_mib_id_supported_uao_MaxInputDatalen                        2 
#define DMAP_mib_id_supported_uao_MaxOutputDatalen                       3 
#define DMAP_mib_id_supported_uao_MinDataUpdateRate                      4 
#define DMAP_mib_id_supported_uao_SupperInputType                        5 
#define DMAP_mib_id_supported_uao_SupperOutputType                       6 

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 41 
 *                   UAO Instance Description Struct 
 *
 ************************************************************************************/
#define DMAP_mib_id_uao_instance_UAOID                                  0
#define DMAP_mib_id_uao_instance_ClassID                                1 
#define DMAP_mib_id_uao_instance_UAPID                                  2 
#define DMAP_mib_id_uao_instance_AckFlag                                3 
  #define DMAP_mib_id_uao_instance_AckFlag_Nessisary_to_be_confirmed         0
  #define DMAP_mib_id_uao_instance_AckFlag_Unnessisary_to_be_confirmed       1
#define DMAP_mib_id_uao_instance_NumberOfInputData                      4 
#define DMAP_mib_id_uao_instance_NumberOfOutputData                     5 
#define DMAP_mib_id_uao_instance_ConfiguredInputDataList                6 
#define DMAP_mib_id_uao_instance_ConfiguredOutputDataList               7 

tsn_mibid_t 
TSN_DMAP_mib_id2index(
  tsn_mibid_t mibID
);

TSN_DMAP_mib_attribute_s *
TSN_DMAP_mib_device_find_ByShortAddr(
  Unsigned16 ShortAddress
);

struct tsn_superframe{
  Unsigned8 SuperframeID;
  Unsigned16 NumberSlots;
  Unsigned8  ActiveFlag;
  Unsigned48 ActiveSlot;
};
typedef struct tsn_superframe tsn_superframe_s;

struct tsn_dll_link{
  Unsigned16  LinkID;
  Unsigned8   LinkType;
  Unsigned48  ActiveSlot;
  Unsigned16  PeerAddr;
  Unsigned16  RelativeSlotNumber;
  Unsigned8   ChannelIndex;
  Unsigned8   SuperframeID;
};
typedef struct tsn_dll_link tsn_dll_link_s;

struct tsn_channel{
  Unsigned8   ChannelID;
  Unsigned8   LinkQuality;
  SingleFloat PacketLossRate;
  Unsigned8   RetryNumber;
};
typedef struct tsn_channel tsn_channel_s;

struct tsn_key{
  Unsigned16  KeyID;
  Unsigned16  PeerAddr;
  Unsigned8   KeyType;
  KeyData     KeyDataValue;
  Unsigned48  KeyActiveSlot;
  Unsigned16  KeyAttackCount;
  Unsigned8   AlarmFlag;
  Unsigned8   KeyState;
};
typedef struct tsn_key tsn_key_s;

struct tsn_vcrEP{
  Unsigned16  VcrID;
  Unsigned8   VcrType;
  Unsigned8   UapID;
  Unsigned16  PeerAddr;
  TimeData    VcrActiveTime;
  Unsigned32  DataUpdateRate;
  Unsigned8   Deadline;
  Unsigned32  WatchdogTime;
};
typedef struct tsn_vcrEP tsn_vcrEP_s;

struct tsn_uao_parameter_datatype{
  BitField16  ParameterDataType;
};
typedef struct tsn_uao_parameter_datatype tsn_uao_parameter_datatype_s;

struct tsn_supported_uao_description{
  Unsigned8   ClassID;
  Unsigned8   UAOType;
  Unsigned16  MaxInputDatalen;
  Unsigned16  MaxOutputDatalen;
  Unsigned32  MinDataUpdateRate;
  tsn_uao_parameter_datatype_s SupperInputType;
  tsn_uao_parameter_datatype_s SupperOutputType;
};
typedef struct tsn_supported_uao_description tsn_supported_uao_description_s;

struct tsn_uao_instance_description{
  Unsigned8   UAOID;
  Unsigned8   ClassID;
  Unsigned8   UAPID;
  Unsigned16  AckFlag;
  Unsigned8   NumberOfInputData;
  Unsigned8   NumberOfOutputData;
  tsn_uao_parameter_datatype_s ConfiguredInputDataList;
  tsn_uao_parameter_datatype_s ConfiguredOutputDataList;
};
typedef struct tsn_uao_instance_description tsn_uao_instance_description_s;

struct tsn_static_config{
  TimeData    PriGwFailureTime;
  Unsigned8   AddressTypeFlag;
  Unsigned16  MaxPayloadLength;
  Unsigned8   NACKCount;
  Unsigned8   NetworkID;
  Unsigned24  BitMap;
  Unsigned16  DeviceStatusReportCycle;
  Unsigned16  ChannelStatusReportCycle;
  Unsigned8   LossConnectDuration;
  Unsigned8   KeyUpdateDuration;
  Unsigned16  TimeSlotDuration;
  Unsigned8   TwoWayTimeSynchronization;
  Unsigned8   TwoWayTimeSynchronizationTimeout;
  Unsigned8   AccessDeviceTeamNumber;
  SingleFloat ExpectedLossRate;
  SingleFloat LossRate;
  Unsigned8   MaxRetry;
  Unsigned8   SecurityLevel;
  Unsigned16  AttackStatisticsDuration;
  Unsigned8   MaxKeyAttackedNumber;
  Unsigned8   AlarmReportDuration;
  Unsigned8   ChannelNumber;
  Unsigned8   AggregationEnableFlag;
  char pad0[0] ____TSN_ALIGN___;
  tsn_superframe_s *SuperframeList;
  tsn_dll_link_s   *DllLinkList;
  tsn_channel_s    *ChannelConditionList;
  tsn_device_s     *DeviceList;
  tsn_key_s        *KeyList;
  tsn_vcrEP_s      *VCRList;
  tsn_supported_uao_description_s *SupportedUAOList;
  tsn_uao_instance_description_s  *ConfiguredUAOList;
  char pad1[0] ____TSN_ALIGN___;
};
typedef struct tsn_static_config tsn_static_config_s;

const char *dmap_mib_AttributeID2string(int attrID);
const char *dmap_mib_MemberID2string(int attrID, int memberID);

#endif


