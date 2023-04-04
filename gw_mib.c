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

static TSN_DMAP_mib_attribute_s *
__TSN_DMAP_mib_create(const TSN_DMAP_mib_attribute_s *srcMib, int size);

static tsn_boolean_e tsn_mib_get(
  unsigned short networkID __TSN_UNUSED, 
  unsigned short memberID __TSN_UNUSED, 
  unsigned short firstIndex __TSN_UNUSED, 
  unsigned short count __TSN_UNUSED, 
  void **data __TSN_UNUSED
)
{
  return TSN_FALSE;
}
static tsn_boolean_e tsn_mib_set(
  unsigned short networkID __TSN_UNUSED, 
  unsigned short memberID __TSN_UNUSED, 
  unsigned short firstIndex __TSN_UNUSED, 
  unsigned short count __TSN_UNUSED,
  void *data __TSN_UNUSED, 
  unsigned short *len __TSN_UNUSED
)
{
  return TSN_FALSE;
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 35 
 *           Superframe Struct Attributes
 *
#define DMAP_mib_id_superframe_SuperframeID                      0
#define DMAP_mib_id_superframe_NumberSlots                       1 
#define DMAP_mib_id_superframe_ActiveFlag                        2 
#define DMAP_mib_id_superframe_ActiveSlot                        3 
 ************************************************************************************/
  static TSN_DMAP_mib_attribute_s superframe_mibs[4]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_superframe_,
      SuperframeID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The ID of superframe assigned by WIA-FA network administrator.",
      tsn_offsetof(tsn_superframe_s,SuperframeID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_superframe_,
      NumberSlots,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The number of timeslots of superframe on WIA-FA network.",
      tsn_offsetof(tsn_superframe_s,NumberSlots),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_superframe_,
      ActiveFlag,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The flag of whether superframe is activated or not."
      "<br/>  0: Deactivated"
      "<br/>  1: Activated",
      tsn_offsetof(tsn_superframe_s,ActiveFlag),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_superframe_,
      ActiveSlot,
      Unsigned48,
      0,
      Unsigned48Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The activated ASN (absolute timeslot number), "
      "which is equal [Time-Value/TimeSlotDuration].",
      tsn_offsetof(tsn_superframe_s,ActiveSlot),
      tsn_mib_get,
      tsn_mib_set
      ),
  };
  
/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 36 
 *           Link Struct Attributes
 *
#define DMAP_mib_id_link_LinkID                            0
#define DMAP_mib_id_link_LinkType                          1 
  #define DMAP_mib_id_link_LinkType_TxType_Unicast                       0x00 
  #define DMAP_mib_id_link_LinkType_TxType_Broadcast                     0x80 
  #define DMAP_mib_id_link_LinkType_TxDirection_Transmit                 0x00 
  #define DMAP_mib_id_link_LinkType_TxDirection_SharedTransmission       0x20
  #define DMAP_mib_id_link_LinkType_TxDirection_Retransmit               0x40
  #define DMAP_mib_id_link_LinkType_TxDirection_Receive                  0x60
  #define DMAP_mib_id_link_LinkType_TimeslotType_Beacon                  0x00
  #define DMAP_mib_id_link_LinkType_TimeslotType_NACK                    0x04
  #define DMAP_mib_id_link_LinkType_TimeslotType_GACK                    0x08
  #define DMAP_mib_id_link_LinkType_TimeslotType_ManagementTimeslot      0x0C
  #define DMAP_mib_id_link_LinkType_TimeslotType_DataTimeslot            0x10
  #define DMAP_mib_id_link_LinkType_TimeslotType_ManagementDataTimeslot  0x14
#define DMAP_mib_id_link_ActiveSlot                        2 
#define DMAP_mib_id_link_PeerAddr                          3 
#define DMAP_mib_id_link_RelativeSlotNumber                4
#define DMAP_mib_id_link_ChannelIndex                      5
#define DMAP_mib_id_link_SuperframeID                      6
*************************************************************************************/
  TSN_DMAP_mib_attribute_s dll_link_mibs[7]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      LinkID,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The id of DLL.",
      tsn_offsetof(tsn_dll_link_s,LinkID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      LinkType,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "Bit0:   transmission type."
      "<br/>      0: Unicast."
      "<br/>      1: Broadcast."
      "<br/>Bit1-2: transmission direction."
      "<br/>      00: Transmit."
      "<br/>      01: Shared Transmission."
      "<br/>      10: Retransmit."
      "<br/>      11: Receive."
      "<br/>Bit3-5: Timeslot type."
      "<br/>      000: Beacon."
      "<br/>      001: NACK."
      "<br/>      010: GACK."
      "<br/>      011: Management timeslot."
      "<br/>      100: Data timeslot."
      "<br/>      101: Management and Data timeslot.",
      tsn_offsetof(tsn_dll_link_s,LinkType),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      ActiveSlot,
      Unsigned48,
      0,
      Unsigned48Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "Absolute timeslot for data link (TimeCale/TimeSlotDuration).",
      tsn_offsetof(tsn_dll_link_s,ActiveSlot),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      PeerAddr,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The short address of peer device.",
      tsn_offsetof(tsn_dll_link_s,PeerAddr),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      RelativeSlotNumber,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The relative timeslot number.",
      tsn_offsetof(tsn_dll_link_s,RelativeSlotNumber),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      ChannelIndex,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The index of current channel.",
      tsn_offsetof(tsn_dll_link_s,ChannelIndex),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_link_,
      SuperframeID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The superframe ID.",
      tsn_offsetof(tsn_dll_link_s,SuperframeID),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 35 
 *                   Channel Condition Struct Attributes 
 *
#define DMAP_mib_id_channel_condition_ChannelID                         0
#define DMAP_mib_id_channel_condition_LinkQuality                       1 
#define DMAP_mib_id_channel_condition_PacketLossRate                    2 
#define DMAP_mib_id_channel_condition_RetryNumber                       3 
 ************************************************************************************/
  TSN_DMAP_mib_attribute_s channel_condition_mibs[4]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_channel_condition_,
      ChannelID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The channel index.",
      tsn_offsetof(tsn_channel_s,ChannelID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_channel_condition_,
      LinkQuality,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The link quality of channel.",
      tsn_offsetof(tsn_channel_s,LinkQuality),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_channel_condition_,
      PacketLossRate,
      SingleFloat,
      0,
      1,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The packet loss rate of channel.",
      tsn_offsetof(tsn_channel_s,PacketLossRate),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_channel_condition_,
      RetryNumber,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The maximum frame retransmission times of channel.",
      tsn_offsetof(tsn_channel_s,RetryNumber),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 37 
 *                   Device Struct Attributes 
 *
#define DMAP_mib_id_device_Version                           0
#define DMAP_mib_id_device_LongAddress                       1 
  #define TSN_LongAddr_VendorID_Mask   0xffffff0000000000
  #define TSN_LongAddr_DeviceType_Mask 0x000000ffff000000
    #define TSN_LongAddr_DeviceType_GateWay  0x0000000000000000
    #define TSN_LongAddr_DeviceType_Access   0x0000000001000000
    #define TSN_LongAddr_DeviceType_Field    0x0000000002000000
    #define TSN_LongAddr_DeviceType_HandHeld 0x0000000003000000
  #define TSN_LongAddr_DeviceID_Mask   0x0000000000ffffff
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
  #define DMAP_mib_id_device_PowerSupplyStatus_Fixed              0 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel1          1 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel2          2 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel3          3 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel4          4 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel5          5 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel6          6 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel7          7 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel8          8 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel9          9 
  #define DMAP_mib_id_device_PowerSupplyStatus_QeqLevel10         10 
  
#define DMAP_mib_id_device_DeviceState                       12 
  #define DMAP_mib_id_device_DeviceState_Detached                 0 
  #define DMAP_mib_id_device_DeviceState_Joining                  1 
  #define DMAP_mib_id_device_DeviceState_Authenticating           2 
  #define DMAP_mib_id_device_DeviceState_Configuring              3 
  #define DMAP_mib_id_device_DeviceState_AllocatingResources      4 
  #define DMAP_mib_id_device_DeviceState_Running                  5 
 ************************************************************************************/
TSN_DMAP_mib_attribute_s device_mibs[13]={
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    Version,
    Unsigned16,
    0,
    Unsigned16Max,
    DMAP_mib_permission_read,
    DMAP_mib_type_contant,
    2013,
    DMAP_mib_device_all,
    "The version of device.",
    tsn_offsetof(tsn_device_s,Version),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    LongAddress,
    Unsigned64,
    0,
    Unsigned64Max,
    DMAP_mib_permission_read,
    DMAP_mib_type_contant,
    0,
    DMAP_mib_device_all,
    "The global unique address."
      "<br/>Bit4-5:"
      "<br/>  0: GD, Gateway Device"
      "<br/>  1: AD, Access Device"
      "<br/>  2: FD, Field Device"
      "<br/>  3: HD, Handheld Device",
    tsn_offsetof(tsn_device_s,LongAddress),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    AggregationSupportFlag,
    Unsigned8,
    0,
    1,
    DMAP_mib_permission_read,
    DMAP_mib_type_static,
    0,
    DMAP_mib_device_all,
    "The flag of whether aggregation is supported or not."
    "<br/>  0: Unsupported;"
    "<br/>  1: Supported",
    tsn_offsetof(tsn_device_s,AggregationSupportFlag),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    NumberOfSuperframeUAO,
    Unsigned16,
    0,
    Unsigned16Max,
    DMAP_mib_permission_read,
    DMAP_mib_type_static,
    1,
    DMAP_mib_device_all,
    "The number UAOs supported by field device.",
    tsn_offsetof(tsn_device_s,NumberOfSuperframeUAO),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    NumberOfConfiguredUAO,
    Unsigned16,
    0,
    Unsigned16Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_static,
    1,
    DMAP_mib_device_all,
    "The number of configured UAOs on field device.",
    tsn_offsetof(tsn_device_s,NumberOfConfiguredUAO),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    TransmitDelay,
    Unsigned16,
    0,
    Unsigned16Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_dynamic,
    1200,
    DMAP_mib_device_all,
    "The delay time (in us) of frame ransmission.",
    tsn_offsetof(tsn_device_s,TransmitDelay),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    ProbeTime,
    Unsigned8,
    0,
    Unsigned8Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_static,
    2,
    DMAP_mib_device_all,
    "The maximum frame retransmission count of channel (in default superframe).",
    tsn_offsetof(tsn_device_s,ProbeTime),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    TimeValue,
    TimeData,
    0,
    TimeDataMax,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_dynamic,
    0,
    DMAP_mib_device_all,
    "The absolute time (in us) counter from zero.",
    tsn_offsetof(tsn_device_s,TimeValue),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    RedundantDeviceFlag,
    Unsigned8,
    0,
    Unsigned8Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_static,
    0,
    DMAP_mib_device_all,
    "The flag of whether it is a redundant device or not."
    "<br/>  0: Yes;"
    "<br/>  1: Not",
    tsn_offsetof(tsn_device_s,RedundantDeviceFlag),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    AccessDeviceID,
    Unsigned8,
    0,
    Unsigned8Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_static,
    0,
    DMAP_mib_device_gw|DMAP_mib_device_ad,
    "The ID of access device.",
    tsn_offsetof(tsn_device_s,AccessDeviceID),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    DeviceShortAddress,
    Unsigned16,
    0,
    Unsigned16Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_static,
    0,
    DMAP_mib_device_all,
    "The short address of device.",
    tsn_offsetof(tsn_device_s,DeviceShortAddress),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    PowerSupplyStatus,
    Unsigned8,
    0,
    Unsigned8Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_static,
    10,
    DMAP_mib_device_all,
    "The status of powersupply."
    "<br/>  0: Fixed powersupply;"
    "<br/>  1: Battery alert level 1."
    "<br/>  2: Battery alert level 2."
    "<br/>  3: Battery alert level 3."
    "<br/>  4: Battery alert level 4."
    "<br/>  5: Battery alert level 5."
    "<br/>  6: Battery alert level 6."
    "<br/>  7: Battery alert level 7."
    "<br/>  8: Battery alert level 8."
    "<br/>  9: Battery alert level 9."
    "<br/>  10: Battery alert level 10.",
    tsn_offsetof(tsn_device_s,PowerSupplyStatus),
    tsn_mib_get,
    tsn_mib_set
    ),
    
  __DECL_MIB_ATTR_OFFSET(
    DMAP_mib_id_device_,
    DeviceState,
    Unsigned8,
    0,
    Unsigned8Max,
    DMAP_mib_permission_read|DMAP_mib_permission_write,
    DMAP_mib_type_dynamic,
    0,
    DMAP_mib_device_all,
    "The state of device."
    "<br/>  0: Detached"
    "<br/>  1: Joining"
    "<br/>  2: Authenticating"
    "<br/>  1: Configuring"
    "<br/>  1: Allocating Resources"
    "<br/>  1: Running",
    tsn_offsetof(tsn_device_s,DeviceState),
    tsn_mib_get,
    tsn_mib_set
    ),
};

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 37 
 *                   Key Struct Attributes 
 *
#define DMAP_mib_id_key_KeyID                                0
#define DMAP_mib_id_key_PeerAddr                             1 
#define DMAP_mib_id_key_KeyType                              2 
  #define DMAP_mib_id_key_KeyType_KJ_KEY_JOIN                             0 
  #define DMAP_mib_id_key_KeyType_KS_KEY_SHARED              1 
  #define DMAP_mib_id_key_KeyType_KEK_KEY_Eencription_KEY          2 
  #define DMAP_mib_id_key_KeyType_KEDU_Eencription_Key_Data_Unicast    3 
  #define DMAP_mib_id_key_KeyType_KEDB_Eencription_Key_Data_Broadcast    4 
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
 ************************************************************************************/
  TSN_DMAP_mib_attribute_s key_mibs[8]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      KeyID,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The ID of security KEY.",
      tsn_offsetof(tsn_key_s,KeyID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      PeerAddr,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The short address of peer device.",
      tsn_offsetof(tsn_key_s,PeerAddr),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      KeyType,
      Unsigned8,
      0,
      4,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The flag of whether aggregation is supported or not."
      "<br/>  0: KJ, KEY-JOIN"
      "<br/>  1: KS, KEY-SHARED"
      "<br/>  2: KEK, KEY-Eencription-KEY"
      "<br/>  3: KEDU, Eencription key for data unicast"
      "<br/>  4: KEDB, Eencription key for data broadcast",
      tsn_offsetof(tsn_key_s,KeyType),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      KeyDataValue,
      KeyData,
      0,
      Unsigned128Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The value of key.",
      tsn_offsetof(tsn_key_s,KeyAttackCount),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      KeyActiveSlot,
      Unsigned48,
      0,
      Unsigned48Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      1,
      DMAP_mib_device_all,
      "The ASN (absolute timeslot number in ms) for activating KEY.",
      tsn_offsetof(tsn_key_s,KeyActiveSlot),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      KeyAttackCount,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The times of KEY-attacking.",
      tsn_offsetof(tsn_key_s,KeyAttackCount),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      AlarmFlag,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      2,
      DMAP_mib_device_all,
      "The flag of whether KEY is attacked or not."
      "<br/>  0: alarm of KEY-attaction"
      "<br/>  1: alarm of KEY-update timeout",
      tsn_offsetof(tsn_key_s,AlarmFlag),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_key_,
      KeyState,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The status of KEY."
      "<br/>  0: Backup"
      "<br/>  1: Using"
      "<br/>  2: Expired"
      "<br/>  3: Invalid",
      tsn_offsetof(tsn_key_s,KeyState),
      tsn_mib_get,
      tsn_mib_set
      ),
  };
  
/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 38 
 *                   VcrEP Struct 
 *
#define DMAP_mib_id_vcrEP_VcrID                                0
#define DMAP_mib_id_vcrEP_VcrType                              1 
  #define DMAP_mib_id_vcrEP_VcrType_CLIENT              0 
  #define DMAP_mib_id_vcrEP_VcrType_SERVER              1 
  #define DMAP_mib_id_vcrEP_VcrType_PUBLISHER           2 
  #define DMAP_mib_id_vcrEP_VcrType_SUBSCRIBER          3 
  #define DMAP_mib_id_vcrEP_VcrType_REPORT_SOURCE       4 
  #define DMAP_mib_id_vcrEP_VcrType_REPORT_SINK         5
#define DMAP_mib_id_vcrEP_UapID                                2 
#define DMAP_mib_id_vcrEP_PeerAddr                             3 
#define DMAP_mib_id_vcrEP_VcrActiveTime                        4 
#define DMAP_mib_id_vcrEP_DataUpdateRate                       5 
#define DMAP_mib_id_vcrEP_Deadline                             6 
#define DMAP_mib_id_vcrEP_WatchdogTime                         7 
 ************************************************************************************/
  TSN_DMAP_mib_attribute_s vcrEP_mibs[8]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      VcrID,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The unique ID inside one device. 0 is for default C/S VCR.",
      tsn_offsetof(tsn_vcrEP_s,VcrID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      VcrType,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The type of VCR endpoint."
      "<br/>  0: CLIENT"
      "<br/>  1: SERVER"
      "<br/>  2: PUBLISHER"
      "<br/>  3: SUBSCRIBER"
      "<br/>  4: REPORT SOURCE"
      "<br/>  5: REPORT SINK",
      tsn_offsetof(tsn_vcrEP_s,VcrType),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      UapID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The unique ID of UAP inside one device. 0 is for DMAP.",
      tsn_offsetof(tsn_vcrEP_s,UapID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      PeerAddr,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The short address of peer device or gateway.",
      tsn_offsetof(tsn_vcrEP_s,PeerAddr),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      VcrActiveTime,
      TimeData,
      0,
      TimeDataMax,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The absolute time to activate P/S VcrEP. The DataUpdateRate starts "
      "from this time. 0 represents that this VcrEP should be activated "
      "immediatly.",
      tsn_offsetof(tsn_vcrEP_s,VcrActiveTime),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      DataUpdateRate,
      Unsigned32,
      0,
      Unsigned32Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The absolute time to publish pioriodical process data from P/S VcrEP in ms.",
      tsn_offsetof(tsn_vcrEP_s,DataUpdateRate),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      Deadline,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The multiplier of DataUpdateRate. UAP should send ALARM of process-data-not-update "
      "if DataUpdateRatexDealine ms has elapsed after the last pioriodical process data "
      "response on P/S VcrEP.",
      tsn_offsetof(tsn_vcrEP_s,Deadline),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_vcrEP_,
      WatchdogTime,
      Unsigned32,
      0,
      Unsigned32Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      100,
      DMAP_mib_device_all,
      "The maximum allowed time (in ms) for waiting the response on C/S VCR.",
      tsn_offsetof(tsn_vcrEP_s,WatchdogTime),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 40 
 *                   Process Data Description Struct 
 *
#define DMAP_mib_id_uao_ParameterDataType                  0
  #define DMAP_mib_id_uao_ParameterDataType_Unsigned8      0x8000
  #define DMAP_mib_id_uao_ParameterDataType_Unsigned16     0x4000
  #define DMAP_mib_id_uao_ParameterDataType_Unsigned32     0x2000
  #define DMAP_mib_id_uao_ParameterDataType_SingleFloat    0x1000
  #define DMAP_mib_id_uao_ParameterDataType_DoubleFloat    0x0800
  #define DMAP_mib_id_uao_ParameterDataType_Bitstring      0x0400
  #define DMAP_mib_id_uao_ParameterDataType_Count_MASK     0x03ff
 ************************************************************************************/
  TSN_DMAP_mib_attribute_s uao_parameter_datatype_mibs[1]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_,
      ParameterDataType,
      BitField16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The data type of UAO or input/output. 0: supported, 1: unsupported."
      "<br/>  bit15:  Unsigned8"
      "<br/>  bit14:  Unsigned16"
      "<br/>  bit13:  Unsigned32"
      "<br/>  bit12:  SingleFloat"
      "<br/>  bit11:  DoubleFloat"
      "<br/>  bit10:  Bitstring"
      "<br/>  bit0-9: DataTypeCount",
      tsn_offsetof(tsn_uao_parameter_datatype_s,ParameterDataType),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 39 
 *                   UAO Class Description Struct 
 *
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
 ***********************************************************************************/
  TSN_DMAP_mib_attribute_s supported_uao_description_mibs[7]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_supported_uao_,
      ClassID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The unique UAO ID of one device instance.",
      tsn_offsetof(tsn_supported_uao_description_s,ClassID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_supported_uao_,
      UAOType,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The type of UAO."
      "<br/>  0: AI"
      "<br/>  1: AO"
      "<br/>  2: DI"
      "<br/>  3: DO",
      tsn_offsetof(tsn_supported_uao_description_s,UAOType),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_supported_uao_,
      MaxInputDatalen,
      Unsigned16,
      1,
      988,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The maximum input data length in BYTE. "
      "MaxInputDatalen <= 1000 - the length of ASL header (4 bytes).",
      tsn_offsetof(tsn_supported_uao_description_s,MaxInputDatalen),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_supported_uao_,
      MaxOutputDatalen,
      Unsigned16,
      1,
      988,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The maximum output data length in BYTE. "
      "MaxOutputDatalen <= 1000 - the length of ASL header (4 bytes).",
      tsn_offsetof(tsn_supported_uao_description_s,MaxOutputDatalen),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_supported_uao_,
      MinDataUpdateRate,
      Unsigned32,
      0,
      Unsigned32Max,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The minimum data update rate (in ms) for pieriodical process data.",
      tsn_offsetof(tsn_supported_uao_description_s,MinDataUpdateRate),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      uao_parameter_datatype_mibs,
      DMAP_mib_id_supported_uao_,
      SupperInputType,
      Struct,
      0,
      0,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The description of UAO input data type.",
      tsn_offsetof(tsn_supported_uao_description_s,SupperInputType),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      uao_parameter_datatype_mibs,
      DMAP_mib_id_supported_uao_,
      SupperOutputType,
      Struct,
      0,
      0,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The description of UAO output data type.",
      tsn_offsetof(tsn_supported_uao_description_s,SupperOutputType),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.1.2.2, Page 41 
 *                   UAO Instance Description Struct 
 *
#define DMAP_mib_id_uao_instance_UAOID                                  0
#define DMAP_mib_id_uao_instance_ClassID                                1 
#define DMAP_mib_id_uao_instance_UAPID                                  2 
#define DMAP_mib_id_uao_instance_AckFlag                                3 
  #define DMAP_mib_id_uao_instance_AckFlag_Nessisary_to_be_confirmed       0
  #define DMAP_mib_id_uao_instance_AckFlag_Unnessisary_to_be_confirmed     1
#define DMAP_mib_id_uao_instance_NumberOfInputData                      4 
#define DMAP_mib_id_uao_instance_NumberOfOutputData                     5 
#define DMAP_mib_id_uao_instance_ConfiguredInputDataList                6 
#define DMAP_mib_id_uao_instance_ConfiguredOutputDataList               7 
 ************************************************************************************/
  TSN_DMAP_mib_attribute_s uao_instance_description_mibs[8]={
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_instance_,
      UAOID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The unique ID of UAO. 0 is used for MIB.",
      tsn_offsetof(tsn_uao_instance_description_s,UAOID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_instance_,
      ClassID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The unique UAO class ID that indicates it is an instance "
      "of an UAO with this classID in SupperUAOList.",
      tsn_offsetof(tsn_uao_instance_description_s,ClassID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_instance_,
      UAPID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The unique UAP ID that belong to one specific UAO.",
      tsn_offsetof(tsn_uao_instance_description_s,UAPID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_instance_,
      AckFlag,
      Unsigned16,
      0,
      0,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The flag of event data type to indicate whether this event "
      "should be confirmed."
      "<br/>  0: Nessisary to be confirmed."
      "<br/>  1: Unnessisary to be confirmed.",
      tsn_offsetof(tsn_uao_instance_description_s,AckFlag),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_instance_,
      NumberOfInputData,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The number of input data.",
      tsn_offsetof(tsn_uao_instance_description_s,NumberOfInputData),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_uao_instance_,
      NumberOfOutputData,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The number of output data.",
      tsn_offsetof(tsn_uao_instance_description_s,NumberOfOutputData),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      uao_parameter_datatype_mibs,
      DMAP_mib_id_uao_instance_,
      ConfiguredInputDataList,
      List,
      0,
      0,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The data description list of UAO input data.",
      tsn_offsetof(tsn_uao_instance_description_s,ConfiguredInputDataList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      uao_parameter_datatype_mibs,
      DMAP_mib_id_uao_instance_,
      ConfiguredOutputDataList,
      List,
      0,
      0,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      0,
      DMAP_mib_device_all,
      "The data description list of UAO output data.",
      tsn_offsetof(tsn_uao_instance_description_s,ConfiguredOutputDataList),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

/***********************************************************************************
 * GB/T26790.2-2015, 6.7, Page 32 
 *                   Management Information and Services 
 *
#define DMAP_mib_id_static_MIN                                0
#define DMAP_mib_id_static_PriGwFailureTime                  (DMAP_mib_id_static_MIN+ 0)
#define DMAP_mib_id_static_AddressTypeFlag                   (DMAP_mib_id_static_MIN+ 1) 
#define DMAP_mib_id_static_MaxPayloadLength                  (DMAP_mib_id_static_MIN+ 2) 
#define DMAP_mib_id_static_NACKCount                         (DMAP_mib_id_static_MIN+ 3) 
#define DMAP_mib_id_static_NetworkID                         (DMAP_mib_id_static_MIN+ 4) 
#define DMAP_mib_id_static_BitMap                            (DMAP_mib_id_static_MIN+ 5) 
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
  #define DMAP_mib_id_static_SecurityLevel_None                                   0
  #define DMAP_mib_id_static_SecurityLevel_Authentication                         1
  #define DMAP_mib_id_static_SecurityLevel_Authentication_MIC32                   2
  #define DMAP_mib_id_static_SecurityLevel_Authentication_MIC64                   3
  #define DMAP_mib_id_static_SecurityLevel_Authentication_MIC128                  4
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription              5
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription_MIC32        6
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription_MIC64        7
  #define DMAP_mib_id_static_SecurityLevel_Authentication_Encription_MIC128       8
#define DMAP_mib_id_static_AttackStatisticsDuration          (DMAP_mib_id_static_MIN+18) 
#define DMAP_mib_id_static_MaxKeyAttackedNumber              (DMAP_mib_id_static_MIN+19) 
#define DMAP_mib_id_static_AlarmReportDuration               (DMAP_mib_id_static_MIN+20) 
#define DMAP_mib_id_static_ChannelNumber                     (DMAP_mib_id_static_MIN+21) 
#define DMAP_mib_id_static_AggregationEnableFlag             (DMAP_mib_id_static_MIN+22) 
#define DMAP_mib_id_static_MAX                               (DMAP_mib_id_static_MIN+23) 

#define DMAP_mib_id_list_MIN                                  128 
#define DMAP_mib_id_list_SuperframeList                      (DMAP_mib_id_list_MIN+0) 
#define DMAP_mib_id_list_DllLinkList                         (DMAP_mib_id_list_MIN+1) 
#define DMAP_mib_id_list_ChannelConditionList                (DMAP_mib_id_list_MIN+2) 
#define DMAP_mib_id_list_DeviceList                          (DMAP_mib_id_list_MIN+3) 
#define DMAP_mib_id_list_KeyList                             (DMAP_mib_id_list_MIN+4) 
#define DMAP_mib_id_list_VCRList                             (DMAP_mib_id_list_MIN+5) 
#define DMAP_mib_id_list_SuperframeUAOList                   (DMAP_mib_id_list_MIN+6) 
#define DMAP_mib_id_list_ConfiguredUAOList                   (DMAP_mib_id_list_MIN+7) 
#define DMAP_mib_id_list_MAX                                 (DMAP_mib_id_list_MIN+8) 
 ************************************************************************************/
  TSN_DMAP_mib_attribute_s dmap_mibs_table[]={
    /***********************************************************************************
     * GB/T26790.2-2015, 6.7.1.2.2, Page 32 
     *           Non-struct Attributes 
     *
     ************************************************************************************/
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      PriGwFailureTime,
      TimeData,
      0,
      TimeDataMax,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      10,
      DMAP_mib_device_gw,
      "The maximum expired time (in us) whithout receiving any "
      "heartbeat messages.",
      tsn_offsetof(tsn_static_config_s,PriGwFailureTime),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      AddressTypeFlag,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "0: 8-bits short address; 1: 16-bits short address.",
      tsn_offsetof(tsn_static_config_s,AddressTypeFlag),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      MaxPayloadLength,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1000,
      DMAP_mib_device_all,
      "The maximum DLL payload length compatible with the IEEE "
      "802.11 Physical Layer Specification.",
      tsn_offsetof(tsn_static_config_s,MaxPayloadLength),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      NACKCount,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_all,
      "The number of NACK broadcasts.",
      tsn_offsetof(tsn_static_config_s,NACKCount),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      NetworkID,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "The network id used to indicate each one when mutiple network co-existing.",
      tsn_offsetof(tsn_static_config_s,NetworkID),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      BitMap,
      Unsigned24,
      0,
      0,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "Channel BitMap, used to describe the physical channel modulation and status."
      "<br/>0: unused, 1: used."
      "<br/>  Bit0-3 : Modulation method."
      "<br/>  Bit4-17: Channel status.",
      tsn_offsetof(tsn_static_config_s,BitMap),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      DeviceStatusReportCycle,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      10,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The time interval of reporting device status from gateway or field devices.",
      tsn_offsetof(tsn_static_config_s,DeviceStatusReportCycle),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      ChannelStatusReportCycle,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      10,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The time interval of reporting channel status from gateway or field devices.",
      tsn_offsetof(tsn_static_config_s,ChannelStatusReportCycle),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      LossConnectDuration,
      Unsigned24,
      0,
      Unsigned24Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      30,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The timeout interval of neighbour devices. If one device did not receive any "
      "frames from neighbour device for such an interval, it is regarded that the "
      "neighbour deivice has leaven the WIA-FA network.",
      tsn_offsetof(tsn_static_config_s,LossConnectDuration),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      KeyUpdateDuration,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      24,
      DMAP_mib_device_all,
      "The time interval of periodical key updating in hours.",
      tsn_offsetof(tsn_static_config_s,KeyUpdateDuration),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      TimeSlotDuration,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      200,
      DMAP_mib_device_all,
      "The value of timeslot in us.",
      tsn_offsetof(tsn_static_config_s,TimeSlotDuration),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      TwoWayTimeSynchronization,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_all,
      "Whether to use two-way time synchronization or not."
      "<br/>  0: Yes."
      "<br/>  1: NO.",
      tsn_offsetof(tsn_static_config_s,TwoWayTimeSynchronization),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      TwoWayTimeSynchronizationTimeout,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_all,
      "The timeout value of two-way time synchronization.",
      tsn_offsetof(tsn_static_config_s,TwoWayTimeSynchronizationTimeout),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      AccessDeviceTeamNumber,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_gw,
      "The number of access device teams.",
      tsn_offsetof(tsn_static_config_s,AccessDeviceTeamNumber),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      ExpectedLossRate,
      SingleFloat,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_gw,
      "The expected frame loss rate of WIA-FA network.",
      tsn_offsetof(tsn_static_config_s,ExpectedLossRate),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      LossRate,
      SingleFloat,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      0,
      DMAP_mib_device_gw,
      "The current frame loss rate in an industrial scenario.",
      tsn_offsetof(tsn_static_config_s,LossRate),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      MaxRetry,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      3,
      DMAP_mib_device_all,
      "The maximum retransmition times.",
      tsn_offsetof(tsn_static_config_s,MaxRetry),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      SecurityLevel,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      DMAP_mib_id_static_SecurityLevel_Authentication,
      DMAP_mib_device_all,
      "The security level of packet from DLL."
      "<br/>0: None"
      "<br/>1: Authentication"
      "<br/>2: Authentication + MIC-32"
      "<br/>3: Authentication + MIC-64"
      "<br/>4: Authentication + MIC-128"
      "<br/>5: Authentication + Encription"
      "<br/>6: Authentication + Encription + MIC-32"
      "<br/>7: Authentication + Encription + MIC-64"
      "<br/>8: Authentication + Encription + MIC-128",
      tsn_offsetof(tsn_static_config_s,SecurityLevel),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      AttackStatisticsDuration,
      Unsigned16,
      0,
      Unsigned16Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      60,
      DMAP_mib_device_all,
      "The interval (in minutes) to report statistics of attacks to devices.",
      tsn_offsetof(tsn_static_config_s,AttackStatisticsDuration),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      MaxKeyAttackedNumber,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      5,
      DMAP_mib_device_all,
      "The maximum KEY attacking times permitted.",
      tsn_offsetof(tsn_static_config_s,MaxKeyAttackedNumber),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      AlarmReportDuration,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_all,
      "The interval of repeated periodical alarm reports.",
      tsn_offsetof(tsn_static_config_s,AlarmReportDuration),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      ChannelNumber,
      Unsigned8,
      0,
      Unsigned8Max,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_all,
      "The number of trusted channel on one link.",
      tsn_offsetof(tsn_static_config_s,ChannelNumber),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET(
      DMAP_mib_id_static_,
      AggregationEnableFlag,
      Unsigned8,
      TSN_FALSE,
      TSN_TRUE,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      TSN_FALSE,
      DMAP_mib_device_all,
      "The flag of whether aggregation is enabled or not."
      "<br/>  0: Disabled;"
      "<br/>  1: Eabled;",
      tsn_offsetof(tsn_static_config_s,AggregationEnableFlag),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    /***********************************************************************************
     * GB/T26790.2-2015, 6.7.1.2.2, Page 35 
     *           Struct Attributes 
     *
     ************************************************************************************/
    __DECL_MIB_ATTR_OFFSET_LIST(
      superframe_mibs,
      DMAP_mib_id_list_,
      SuperframeList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      1,
      DMAP_mib_device_all,
      "The description list of superframes.",
      tsn_offsetof(tsn_static_config_s,SuperframeList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      dll_link_mibs,
      DMAP_mib_id_list_,
      DllLinkList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      1,
      DMAP_mib_device_all,
      "The description list of DLLs.",
      tsn_offsetof(tsn_static_config_s,DllLinkList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      channel_condition_mibs,
      DMAP_mib_id_list_,
      ChannelConditionList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      1,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The statistics record list of channel status.",
      tsn_offsetof(tsn_static_config_s,ChannelConditionList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      device_mibs,
      DMAP_mib_id_list_,
      DeviceList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      1,
      DMAP_mib_device_all,
      "The description list of WIA-FA devices.",
      tsn_offsetof(tsn_static_config_s,DeviceList),
      tsn_device_mib_get,
      tsn_device_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      key_mibs,
      DMAP_mib_id_list_,
      KeyList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_dynamic,
      1,
      DMAP_mib_device_all,
      "The description list of KEYs.",
      tsn_offsetof(tsn_static_config_s,KeyList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      vcrEP_mibs,
      DMAP_mib_id_list_,
      VCRList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The description list of VCRs.",
      tsn_offsetof(tsn_static_config_s,VCRList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      supported_uao_description_mibs,
      DMAP_mib_id_list_,
      SupportedUAOList,
      List,
      0,
      1024,
      DMAP_mib_permission_read,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The list of UAOs supported by the field devices.",
      tsn_offsetof(tsn_static_config_s,SupportedUAOList),
      tsn_mib_get,
      tsn_mib_set
      ),
      
    __DECL_MIB_ATTR_OFFSET_LIST(
      uao_instance_description_mibs,
      DMAP_mib_id_list_,
      ConfiguredUAOList,
      List,
      0,
      1024,
      DMAP_mib_permission_read|DMAP_mib_permission_write,
      DMAP_mib_type_static,
      1,
      DMAP_mib_device_gw|DMAP_mib_device_fd,
      "The list of configured UAO instances in the field devices.",
      tsn_offsetof(tsn_static_config_s,ConfiguredUAOList),
      tsn_mib_get,
      tsn_mib_set
      ),
  };

static tsn_err_e TSN_DMAP_mib_get_request(void *s);
static tsn_err_e TSN_DMAP_mib_get_confirm(void *s);
static tsn_primative_s TSN_DMAP_mib_get = {
  .data       = dmap_mibs_table,
  .request    = TSN_DMAP_mib_get_request,
  .indication = NULL,
  .response   = NULL,
  .confirm    = TSN_DMAP_mib_get_confirm,
};

#if 0
static tsn_err_e TSN_DMAP_mib_set_request(void *s);
static tsn_err_e TSN_DMAP_mib_set_confirm(void *s);
static tsn_primative_s TSN_DMAP_mib_set = {
  .data       = dmap_mibs_table,
  .request    = TSN_DMAP_mib_set_request,
  .indication = NULL,
  .response   = NULL,
  .confirm    = TSN_DMAP_mib_set_confirm,
};
#endif

tsn_mibid_t
TSN_DMAP_mib_id2index(tsn_mibid_t mibID)
{
  if (mibID >= DMAP_mib_id_static_MIN && mibID < DMAP_mib_id_static_MAX)
    return mibID;
  if (mibID >= DMAP_mib_id_list_MIN && mibID < DMAP_mib_id_list_MAX)
    return mibID;
  return DMAP_mib_id_INVALID;
}

const char *dmap_mib_AttributeID2string(int attrID)
{
  if (attrID >= DMAP_mib_id_static_MIN && attrID < DMAP_mib_id_static_MAX)
    return dmap_mibs_table[attrID].Name;
  if (attrID >= DMAP_mib_id_list_MIN && attrID < DMAP_mib_id_list_MAX)
    return dmap_mibs_table[attrID-DMAP_mib_id_list_MIN+DMAP_mib_id_static_MAX].Name;
  return "<UNKNOWN>";
}

const char *dmap_mib_MemberID2string(int attrID, int memberID)
{
  if (attrID >= DMAP_mib_id_static_MIN && attrID < DMAP_mib_id_static_MAX)
    return "<INVALID>";
  if (attrID >= DMAP_mib_id_list_MIN && attrID < DMAP_mib_id_list_MAX)
  {
    TSN_DMAP_mib_attribute_s *mib = &dmap_mibs_table[attrID
      -DMAP_mib_id_list_MIN+DMAP_mib_id_static_MAX];

    if (memberID > 0 && memberID < mib->DefaultEntryCount)
    {
      return mib->DefaultEntryAddr[memberID].Name;
    }
    return "<INVALID>";
  }
  return "<UNKNOWN>";
}

static int
TSN_DMAP_mib_list_struct_get_check(
  const TSN_DMAP_mib_attribute_s *entry, 
  unsigned int networkID, 
  unsigned int firstIndex, 
  unsigned int count, 
  unsigned int memberID)
{
  void *data;
  
  if (memberID >= entry->DefaultEntryCount)
    return DMAP_mib_get_confirm_INVALID_RANGE;
  
  if (TSN_TRUE != entry->MibGet(networkID, memberID, firstIndex, count, &data))
    return DMAP_mib_get_confirm_INVALID_RANGE;

  return DMAP_mib_get_confirm_SUCCESS;
}

tsn_err_e
TSN_DMAP_mib_init(void)
{
  return TSN_err_none;
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.7.2.2, Page 42 
 *           Parameters of DMAP-MIB-GET.request 
 *
   Unsigned8  Handle           The handle for DMAP-MIB-GET.request.
   Unsigned16 ShortAddr        The 8/16-bits shord address of field device or the 
                                 ID of access device.
   Unsigned8  AttributeID      The ID of this attribute in MIB.
   Unsigned8  MemberID         The ID of one member of a STRUCT attribute. 
                                 255 is for all attributes.
   Unsigned16 FirstStoreIndex  The index of first member of a STRUCT attribute to read.
   Unsigned16 Count            The number of members of a STRUCT attribute to read.
 *
 ************************************************************************************/
static tsn_err_e
TSN_DMAP_mib_get_confirm(void *s)
{
  tsn_primative_s *prv = &TSN_DMAP_mib_get;
  TsnDmapMibGetRequestS *req = (TsnDmapMibGetRequestS *)s;
  TSN_DMAP_mib_attribute_s *srcMib, *entry;
  tsn_mibid_t mibID = TSN_DMAP_mib_id2index(req->AttributeID);
  
  srcMib = (TSN_DMAP_mib_attribute_s*)prv->data;
  entry = &srcMib[mibID];

  if (entry->DataType == DATA_TYPE_List
    || entry->DataType == DATA_TYPE_Struct)
  {
  }
  else
  {
    
  }
  
  return TSN_err_none;
}

static tsn_err_e
TSN_DMAP_mib_get_request(void *s)
{
  tsn_err_e r = TSN_err_none, rc;
  TSN_DMAP_mib_attribute_s *srcMib, *entry;

  tsn_primative_s *prv = &TSN_DMAP_mib_get;
  int status = DMAP_mib_get_confirm_SUCCESS;
  TsnDmapMibGetRequestS *req = (TsnDmapMibGetRequestS *)s;
  TsnDmapMibGetConfirmS cfm = {
      .Handle = req->Handle,
      .Status = DMAP_mib_get_confirm_SUCCESS,
      .Count  = 0,
    };
  tsn_mibid_t mibID = TSN_DMAP_mib_id2index(req->AttributeID);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DMAP_mib_get_request.\n");
    tsn_print("\tHandle: %u.\n", req->Handle);
    tsn_print("\tShortAddr: %u.\n", req->ShortAddr);
    tsn_print("\tAttributeID: %u.\n", req->AttributeID);
    tsn_print("\tMemberID: %d.\n", req->MemberID);
    tsn_print("\tFirstStoreIndex: %u.\n", req->FirstStoreIndex);
    tsn_print("\tCount: %u.\n", req->Count);
  }

  if (mibID == DMAP_mib_id_INVALID)
  {
    r = TSN_err_invalid;
    status = DMAP_mib_get_confirm_INVALID_ATTRIBUTE;
    goto confirm;
  }
  
  srcMib = (TSN_DMAP_mib_attribute_s*)prv->data;
  entry = &srcMib[mibID];
  if (!TSN_DMAP_mib_entry_isreadable(entry))
  {
    status = DMAP_mib_get_confirm_PERMISSION;
    r = TSN_err_permission;
    goto confirm;
  }

  if (entry->DataType == DATA_TYPE_List
    || entry->DataType == DATA_TYPE_Struct)
  {
    status = TSN_DMAP_mib_list_struct_get_check(
      entry, 
      req->NetworkID,
      req->FirstStoreIndex, 
      req->Count, 
      req->MemberID);
    if (status != DMAP_mib_get_confirm_SUCCESS)
    {
      r = TSN_err_permission;
      goto confirm;
    }
  }
  else
  {
    if (req->FirstStoreIndex || req->MemberID)
    {
      status = DMAP_mib_get_confirm_PERMISSION;
      r = TSN_err_malformed;
      goto confirm;
    }
  }

confirm:
  cfm.Status = status;
  rc = prv->confirm(&cfm);
  if (rc != TSN_err_none)
    r = rc;
  return r;
}

#define SERVER_PORT_MIB 8768
static tsn_connection_s tsn_udp_mib_server = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT_MIB,
  .sent       = 0,
};
