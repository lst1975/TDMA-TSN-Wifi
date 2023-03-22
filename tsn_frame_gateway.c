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

enum{
  AD_JOIN_SUCCESS = 0,
  AD_JOIN_UNMATCHED_NetworkID,
  AD_JOIN_AUTHENTICATION_Failed,
  AD_JOIN_EXCEEDED,
};

struct GACKInfo{
  tsn_addr_u Addr;
  uint16_t Seq;
};
typedef struct GACKInfo gack_info_s;

static tsn_err_e 
send_TSN_gw_response(tsn_msg_s *msg)
{
  // TODO
  return TSN_err_none;
}

static tsn_err_e 
send_TSN_gw_data(tsn_msg_s *msg)
{
  // TODO
  return TSN_err_none;
}

static tsn_err_e 
___make_TSN_Buffer(tsn_buffer_s *b, int len)
{
  if (b->size < len)
  {
    Unsigned8 *uptr;
    
    if (b->data != NULL)
    {
      free(b->data);
      b->data = NULL;
    }

    uptr = (Unsigned8 *)malloc(len);
    if (uptr == NULL)
      return -TSN_err_nomem;
    tsn_buffer_init(b, uptr, len);
  }
  else
  {
    tsn_buffer_reinit(b);
  }
  return TSN_err_none;
}

static tsn_err_e 
make_TSN_AD_JOIN_response(tsn_msg_s *msg, 
  Unsigned8 Status, Unsigned16 ADAddr, Unsigned8 AdID)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;

  r = ___make_TSN_Buffer(b, 15);
  if (TSN_err_none != r)
    return r;
  
  tsn_buffer_put8(b, TSN_AD_JOIN_ack);
  tsn_buffer_put64(b, ADAddr);
  if (Status == AD_JOIN_SUCCESS)
  {
    tsn_buffer_put16(b, 4);
    tsn_buffer_put8(b, Status);
    tsn_buffer_put8(b, AdID);
    tsn_buffer_put16(b, ADAddr);
  }
  else
  {
    tsn_buffer_put16(b, 1);
    tsn_buffer_put8(b, Status);
  }
  
  return TSN_err_none;
}

static tsn_err_e 
make_TSN_GACK_indication(tsn_msg_s *msg, 
  Unsigned8 count, Unsigned8 AdID, Unsigned8 addType, 
  gack_info_s *gack)
{
  int len;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;

  len = tsn_get_addr_length(addType);
  if (len != 1 && len != 2 && len != 8)
    return -TSN_err_invalid;
  len += 2;
  len *= count;
  
  r = ___make_TSN_Buffer(b, len+4+1);
  if (TSN_err_none != r)
    return r;
  
  tsn_buffer_put8(b, TSN_GACK_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, len+1);
  tsn_buffer_put8(b, count);
  
  for (int i = 0; i < count; i++)
  {
    switch (addType)
    {
      case DMAP_mib_id_static_AddressTypeFlag_u8:
        tsn_buffer_put8(b, gack[i].Addr.AddrU8);
        break;
      case DMAP_mib_id_static_AddressTypeFlag_u16:
        tsn_buffer_put16(b, gack[i].Addr.AddrU16);
        break;
      case DMAP_mib_id_static_AddressTypeFlag_u64:
        tsn_buffer_put64(b, gack[i].Addr.AddrU64);
        break;
      default:
        return -TSN_err_invalid;
    }
    
    tsn_buffer_put16(b, gack[i].Seq);
  }
  return TSN_err_none;
}

static tsn_err_e 
make_TSN_NACK_indication(tsn_msg_s *msg, 
  Unsigned8 count, Unsigned8 AdID, Unsigned8 addType, 
  tsn_addr_u *nack)
{
  int len;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;

  len = tsn_get_addr_length(addType);
  if (len != 1 && len != 2)
    return -TSN_err_invalid;
  len *= count;
  
  r = ___make_TSN_Buffer(b, len+4+1);
  if (TSN_err_none != r)
    return r;
  
  tsn_buffer_put8(b, TSN_GACK_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, len+1);
  tsn_buffer_put8(b, count);
  for (int i = 0; i < count; i++)
  {
    switch (addType)
    {
      case DMAP_mib_id_static_AddressTypeFlag_u8:
        tsn_buffer_put8(b, nack[i].AddrU8);
        break;
      case DMAP_mib_id_static_AddressTypeFlag_u16:
        tsn_buffer_put16(b, nack[i].AddrU16);
        break;
      case DMAP_mib_id_static_AddressTypeFlag_u64:
      default:
        return -TSN_err_invalid;
    }
  }
  return TSN_err_none;
}

static tsn_err_e 
make_TSN_DLDE_DATA_request(tsn_msg_s *msg, 
    Unsigned16  DstAddr,
    Unsigned16  VCR_ID,
    Unsigned8   DataType,
    Unsigned8   Priority,
    Unsigned16  PayloadLength,
    OctetString Payload)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  
  r = ___make_TSN_Buffer(b, PayloadLength+8);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLDE_DATA_request.\n");

  tsn_buffer_put16(b, DstAddr);
  tsn_buffer_put16(b, VCR_ID);
  tsn_buffer_put8(b, DataType);
  tsn_buffer_put8(b, Priority);
  tsn_buffer_put16(b, PayloadLength);
  tsn_buffer_putlen(b, Payload.data, PayloadLength);
  
  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("TSN_DLDE_DATA_request.\n");
    printf("\tDstAddr: %u\n", DstAddr);
    printf("\tVCR ID: %u\n",  VCR_ID);
    printf("\tDataType: %s\n", get_DLDE_DataType(DataType));
    printf("\tPriority: %s\n", get_DLDE_DataPriority(Priority));
    printf("\tPayload Length: %u\n", PayloadLength);
  }

  return TSN_err_none;
}

tsn_err_e 
make_TSN_information_get_request(tsn_msg_s *msg, 
  void *_req, Unsigned8 AdID, 
  tsn_buffer_s *data __TSN_UNUSED)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_set_request_s *req;

  req = (dlme_information_set_request_s *)_req;
  AttrLen = sizeof(*req);
  
  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_information_set_request.\n");

  tsn_buffer_put8(b, TSN_DLME_INFO_SET_request);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);
  tsn_buffer_putlen(b, (uint8_t *)req, sizeof(*req));

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("TSN_information_get_request.\n");
    printf("\tAdID: %u\n", AdID);
    printf("\tAttrLen: %u\n",  AttrLen);
    printf("\tDstAddr: %u\n", req->DstAddr);
    printf("\tAttribute Option: %s\n", dlme_info_op2string(req->AttributeOption));
    printf("\tAttributeID: %s\n", dmap_mib_AttributeID2string(req->AttributeID));
    printf("\tMemberID: %s\n", dmap_mib_MemberID2string(req->AttributeID, req->MemberID));
    printf("\tFirstStoreIndex: %u\n", req->FirstStoreIndex);
    printf("\tCount: %u\n", req->Count);
  }
  return TSN_err_none;
}

tsn_err_e 
make_TSN_information_set_request(tsn_msg_s *msg, 
  void *_req, Unsigned8 AdID, 
  tsn_buffer_s *data)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_set_request_s *req;

  req = (dlme_information_set_request_s *)_req;
  AttrLen = sizeof(*req)+TSN_BUFFER_LEN(data);
  
  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_information_set_request.\n");

  tsn_buffer_put8(b, TSN_DLME_INFO_SET_request);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);
  tsn_buffer_putlen(b, (uint8_t *)req, sizeof(*req));
  tsn_buffer_putlen(b, data->data, data->len);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("TSN_information_set_request.\n");
    printf("\tAdID: %u\n", AdID);
    printf("\tAttrLen: %u\n",  AttrLen);
    printf("\tDstAddr: %u\n", req->DstAddr);
    printf("\tAttribute Option: %s\n", dlme_info_op2string(req->AttributeOption));
    printf("\tAttributeID: %s\n", dmap_mib_AttributeID2string(req->AttributeID));
    printf("\tMemberID: %s\n", dmap_mib_MemberID2string(req->AttributeID, req->MemberID));
    printf("\tFirstStoreIndex: %u\n", req->FirstStoreIndex);
    printf("\tCount: %u\n", req->Count);
  }
  return TSN_err_none;
}

tsn_err_e 
make_TSN_DLME_JOIN_response(tsn_msg_s *msg, 
  Unsigned8 AdID, Unsigned8 Status, Unsigned16 ShortAddr)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;

  r = ___make_TSN_Buffer(b, 7);
  if (TSN_err_none != r)
    return r;
  
  tsn_buffer_put8(b, TSN_DLME_JOIN_response);
  tsn_buffer_put8(b, AdID);
  if (Status == DLME_JOIN_SUCCESS)
  {
    tsn_buffer_put16(b, 3);
    tsn_buffer_put8(b, Status);
    tsn_buffer_put16(b, ShortAddr);
  }
  else
  {
    tsn_buffer_put16(b, 1);
    tsn_buffer_put8(b, Status);
  }
  
  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_LEAVE_request(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 ShortAddr)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  int AttrLen = sizeof(ShortAddr);

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_leave_request.\n");

  tsn_buffer_put8(b, TSN_DLME_JOIN_response);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);
  tsn_buffer_put16(b, ShortAddr);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("TSN_leave_request.\n");
    printf("\tAdID: %u\n", AdID);
    printf("\tAttrLen: %u\n",  AttrLen);
    printf("\tShortAddr: %u\n", ShortAddr);
  }

  return TSN_err_none;
}

static tsn_err_e 
do_TSN_AD_JOIN_request(tsn_msg_s *msg)
{
  int status;
  tsn_err_e r;
  tsn_buffer_s *b;
  tsn_network_s *net;
  tsn_sockaddr_s *s;
  tsn_device_s *dev;
  tsn_ad_join_request_s req;
  tsn_gw_dlpdu_normal_s *n;
  Unsigned16 ShortAddr;

  b = &msg->b;
  if (TSN_BUFFER_LEN(b) != 9)
    return -TSN_err_malformed;

  n = (tsn_gw_dlpdu_normal_s *)msg->dlpdu;
  if (n->AttrLen != 9)
    return -TSN_err_malformed;
  
  TSN_event("Received TSN_AD_JOIN_request.\n");

  tsn_buffer_get8(b,  &req.NetworkID);
  tsn_buffer_get64(b, &req.PhyAddr);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tNetworkID: %u.\n", req.NetworkID);
    tsn_print_longaddr(req.PhyAddr);
  }

  if (!tsn_is_Access(req.PhyAddr))
    return -TSN_err_unsupport;

  if (req.PhyAddr != n->ADAddr)
    return -TSN_err_malformed;

  s = tsn_system_cfg_ad_find(req.NetworkID, &msg->from);
  if (s == NULL)
  {
    TSN_event("Received TSN_AD_JOIN_request from unauthorized client: ");
    tsn_sockaddr_print(&msg->from, "", "\n");
    return -TSN_err_invalid;
  }
  
  dev = tsn_network_ad_find_by_PhyAddr(req.NetworkID, req.PhyAddr);
  if (dev != NULL)
    goto good;

  if (TSN_FALSE == tsn_network_check_ad(req.NetworkID, 
    req.PhyAddr))
  {
    status = AD_JOIN_AUTHENTICATION_Failed;
    goto failed;
  }
    
  if (req.NetworkID>=TSN_NetworkID_MAX)
  {
    status = AD_JOIN_UNMATCHED_NetworkID;
    goto failed;
  }
  
  net = &sysCfg.network[req.NetworkID];
  if (net->AdID + 1 > TSN_ADID_MAX)
  {
    status = AD_JOIN_EXCEEDED;
    goto failed;
  }
  
  if (TSN_AllocateShortAddr(&ShortAddr) != TSN_TRUE)
  {
    status = AD_JOIN_EXCEEDED;
    goto failed;
  }

  r = TSN_device_create(&dev,
          req.NetworkID, 
          req.PhyAddr, 
          net->AdID, 
          DMAP_mib_id_device_DeviceState_Joining, 
          ShortAddr);
  if (r != TSN_err_none)
  {
    status = AD_JOIN_EXCEEDED;
    goto failed;
  }

  dev->SockAddr = s;
  net->AdID++;

good:  
  r = make_TSN_AD_JOIN_response(msg, 
    AD_JOIN_SUCCESS, 
    dev->AccessDeviceID, 
    dev->DeviceShortAddress);
  if (r != TSN_err_none)
    return r;
  return send_TSN_gw_response(msg);
  
failed:
  r = make_TSN_AD_JOIN_response(msg, status, 0, 0);
  if (r != TSN_err_none)
    return r;
  return send_TSN_gw_response(msg);
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.4.5, Page 56
 *
 * GB/T26790.2-2015, 8.1.7.2, Page 56
 ***********************************************************************************/
static tsn_err_e 
do_TSN_AD_JOIN_ack(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_GACK_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_NACK_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLDE_DATA_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLDE_DATA_indication(tsn_msg_s *msg)
{
  tsn_buffer_s *b = &msg->b;
  tsn_dlde_data_transmit_indication_s s;

  if (TSN_BUFFER_LEN(b) < 5)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_DLDE_DATA_indication.\n");

  tsn_buffer_get16(b, &s.srcAddr);
  tsn_buffer_get8(b, &s.DataType);
  tsn_buffer_get16(b, &s.PayloadLength);
  
  if (TSN_BUFFER_LEN(b) != s.PayloadLength)
    return -TSN_err_malformed;

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tTSN_DLDE_DATA_indication.\n");
    printf("\tDataType: %s.\n", get_DLDE_DataType(s.DataType));
    printf("\tSrcAddr: %u.\n", s.srcAddr);
    printf("\tPayloadLength: %u.\n", s.PayloadLength);
  }
  
  if (s.DataType != TSN_DLDE_DATA_TRANSMIT_INDICATION_DataType_DATA)
    return -TSN_err_unsupport;
    
  return send_TSN_gw_data(msg);
}

static tsn_err_e 
do_TSN_DLME_JOIN_indication(tsn_msg_s *msg)
{
  tsn_buffer_s *b = &msg->b;
  dlme_join_indication_s ind;
  
  if (TSN_BUFFER_LEN(b) < 8)
    return -TSN_err_tooshort;

  tsn_buffer_get64(b, &ind.PhyAddr);
  if (sysCfg.config.SecurityLevel != DMAP_mib_id_static_SecurityLevel_None)
  {
    if (TSN_BUFFER_LEN(b) < 8)
      return -TSN_err_tooshort;
    tsn_buffer_get64(b, &ind.SecMaterial);
  }
  else
  {
    ind.SecMaterial = 0;
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tTSN_DLME_JOIN_indication.\n");
    printf("\tPhyAddr: %"PRIu64".\n", ind.PhyAddr);
    printf("\tSecMaterial: %"PRIu64".\n", ind.SecMaterial);
  }

  return gw_dmap_T1_receive_dlme_join_indication(msg, NULL, &ind);
}

static tsn_err_e 
do_TSN_DLME_JOIN_response(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLME_DEV_STATUS_indication(tsn_msg_s *msg)
{
  int NetworkID;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_device_s *dev;
  dlme_device_status_indication_s ind;

  if (TSN_BUFFER_LEN(b) < 3)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_DLME_DEV_STATUS_indication.\n");

  tsn_buffer_get16(b, &ind.ShortAddr);
  tsn_buffer_get8(b, &ind.PowerSupplyStatus);

  if (ind.PowerSupplyStatus > DMAP_mib_id_device_PowerSupplyStatus_MAX)
    return -TSN_err_invalid;
    
  NetworkID = tsn_network_id_by_Sockaddr(&msg->from);
  if (NetworkID == TSN_NetworkID_MAX)
  {
    TSN_error("Received packet from unknown AD.\n");
    if (sysCfg.logError)
    {
      tsn_sockaddr_print(&msg->from, "AD IP Address is ", "\n");
    }
    return -TSN_err_existed;
  }
  r = TSN_device_find_ByShortAddr(&dev, NetworkID, ind.ShortAddr);
  if (r != TSN_err_none)
  {
    TSN_error("Not find FD device with short address %u.\n", ind.ShortAddr);
    return -TSN_err_existed;
  }

  dev->PowerSupplyStatus = ind.PowerSupplyStatus;
  return TSN_err_none;
}

static tsn_err_e 
do_TSN_DLME_CHAN_COND_indication(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_device_s *dev;
  dlme_channel_condition_indication_s ind;

  if (TSN_BUFFER_LEN(b) < 2)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_DLME_CHAN_COND_indication.\n");

  tsn_buffer_get16(b, &ind.SrcAddr);
  tsn_buffer_get8(b, &ind.Count);
  for(int i = 0;i < ind.Count;i++)
  {
    tsn_buffer_get8(b, &ind.ChannelConditionInfo[i].ChannelID);
    tsn_buffer_get8(b, &ind.ChannelConditionInfo[i].LinkQuality);
    tsn_buffer_get32(b, (Unsigned32 *)&ind.ChannelConditionInfo[i].PacketLossRate);
    tsn_buffer_get8(b, &ind.ChannelConditionInfo[i].RetryNumber);
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tSrcAddr = 0x%x.\n", ind.SrcAddr);
    printf("\tCount = %u.\n", ind.Count);
    for(int i = 0;i < ind.Count;i++)
    {
  	  printf("ChannelConditionInfo[%d]:\n", i);
  	  printf("ChannelID = %u.\n", ind.ChannelConditionInfo[i].ChannelID);
  	  printf("LinkQuality = %u.\n", ind.ChannelConditionInfo[i].LinkQuality);
  	  /* print PacketLossRate */
  	  printf("RetryNumber = %u.\n", ind.ChannelConditionInfo[i].RetryNumber);
    }
  }

  return TSN_err_none;
}

static tsn_err_e 
do_TSN_DLME_TIME_SYNC_indication(tsn_msg_s *msg)
{
  tsn_buffer_s *b = &msg->b;
  dlme_time_sync_indication_s ind;
  
  if (TSN_BUFFER_LEN(b) < sizeof(ind))
    return -TSN_err_tooshort;

  TSN_event("Received TSN_DLME_Time_Sync_indication.\n");

  tsn_buffer_get16(b, &ind.SrcAddr);
  tsn_buffer_get64(b, &ind.FieldDeviceTimeValue);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tTSN_DLME_Time_Sync_indication.\n");
    printf("\tSrcAddr: %u.\n", ind.SrcAddr);
    printf("\tFieldDeviceTimeValue: %"PRIu64".\n", 
      ind.FieldDeviceTimeValue);
  }
  
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLME_TIME_SYNC_response(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLME_INFO_GET_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLME_INFO_GET_confirm(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_get_confirm_s cfm;

  if(TSN_BUFFER_LEN(b) < 12)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_DLME_INFO_GET_confirm.\n");

  tsn_buffer_get8(b, &cfm.Handle);
  tsn_buffer_get16(b, &cfm.SrcAddr);
  tsn_buffer_get8(b, &cfm.Status);
  tsn_buffer_get8(b, &cfm.AttributeID);
  tsn_buffer_get8(b, &cfm.MemberID);
  tsn_buffer_get16(b, &cfm.FirstStoreIndex);
  tsn_buffer_get16(b, &cfm.Count);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tTSN_DLME_INFO_GET_confirm.\n");
    printf("\tHandle: %u.\n", cfm.Handle);
    printf("\tSrcAddr: 0x%x.\n", cfm.SrcAddr);
    printf("\tStatus: %s.\n", dlmeInformationGetConfirmStatus2String(cfm.Status));
    printf("\tAttributeID: %u.\n", cfm.AttributeID);
    printf("\tMemberID: %d.\n", cfm.MemberID);
    printf("\tFirstStoreIndex: %u.\n", cfm.FirstStoreIndex);
    printf("\tCount: %u.\n", cfm.Count);
  }

  return TSN_err_none;
}

static tsn_err_e 
do_TSN_DLME_INFO_SET_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_DLME_INFO_SET_confirm(tsn_msg_s *msg)
{
  int NetworkID;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_device_s *dev;
  dlme_information_set_confirm_s cfm;

  TSN_event("Received TSN_DLME_INFO_SET_confirm.\n");

  if (TSN_BUFFER_LEN(b) < sizeof(cfm))
    return -TSN_err_tooshort;
  
  tsn_buffer_get16(b, &cfm.Handle);
  tsn_buffer_get8(b, &cfm.Status);

  if (cfm.Handle >= TSN_ShorAddress_INVALID)
    return -TSN_err_invalid;
    
  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("\tHandle: %u.\n", cfm.Handle);
    printf("\tStatus: %s.\n", dlme_info_set_cfm_status2string(cfm.Status));
  }

  if (cfm.Status != DLME_information_set_confirm_SUCCESS)
    return -TSN_err_system;

  NetworkID = tsn_network_id_by_Sockaddr(&msg->from);
  if (NetworkID == TSN_NetworkID_MAX)
  {
    TSN_error("Received packet from unknown AD.\n");
    if (sysCfg.logError)
    {
      tsn_sockaddr_print(&msg->from, "AD IP Address is ", "\n");
    }
    return -TSN_err_existed;
  }
  r = TSN_device_find_ByShortAddr(&dev, NetworkID, cfm.Handle);
  if (r != TSN_err_none)
  {
    TSN_error("Not find FD device with short address %u.\n", cfm.Handle);
    return -TSN_err_existed;
  }

  if (TSN_TRUE != gw_dmap_T5_receive_information_set_confirm(msg, dev, &cfm))
    return -TSN_err_system;
  else
    return TSN_err_none;
}

static tsn_err_e 
do_TSN_DLME_LEAVE_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_KEY_ESTABLISH_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_KEY_ESTABLISH_confirm(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_KEY_UPDATE_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_KEY_UPDATE_confirm(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e 
do_TSN_SECURITY_ALARM_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

#define __DECL_F(n) [TSN_ ## n] = { \
  .number=TSN_ ## n, \
  .name=#n, \
  .func=do_TSN_ ## n, \
}
  
static name_number_s frameTableGateway[TSN_dlpdu_type_max]={
  __DECL_F(AD_JOIN_request),
  __DECL_F(AD_JOIN_ack),
  __DECL_F(GACK_indication),
  __DECL_F(NACK_indication),
  __DECL_F(DLDE_DATA_request),
  __DECL_F(DLDE_DATA_indication),
  __DECL_F(DLME_JOIN_indication),
  __DECL_F(DLME_JOIN_response),
  __DECL_F(DLME_DEV_STATUS_indication),
  __DECL_F(DLME_CHAN_COND_indication),
  __DECL_F(DLME_TIME_SYNC_indication),
  __DECL_F(DLME_TIME_SYNC_response),
  __DECL_F(DLME_INFO_GET_request),
  __DECL_F(DLME_INFO_GET_confirm),
  __DECL_F(DLME_INFO_SET_request),
  __DECL_F(DLME_INFO_SET_confirm),
  __DECL_F(DLME_LEAVE_request),
  __DECL_F(KEY_ESTABLISH_request),
  __DECL_F(KEY_ESTABLISH_confirm),
  __DECL_F(KEY_UPDATE_request),
  __DECL_F(KEY_UPDATE_confirm),
  __DECL_F(SECURITY_ALARM_indication),
};
#undef __DECL_F

static const char *
__dlpduType2String(unsigned int type)
{
  if (type >= TSN_dlpdu_type_max)
    return "<UNKONWN>";
  return frameTableGateway[type].name;
}

static inline tsn_err_e
tsn_adgw_dll_dlpdu2normal(tsn_buffer_s *b, 
  tsn_gw_dlpdu_normal_s *n)
{
  n->AdID   = 0;
  n->ADAddr = 0;

  if (b->len < 3)
    return -TSN_err_tooshort;

  tsn_buffer_get8(b, &n->ServID);
  if (n->ServID > TSN_AD_JOIN_ack)
  {
    if (b->len < 1)
      return -TSN_err_tooshort;
    tsn_buffer_get8(b, &n->AdID);
  }
  else
  {
    if (b->len < 4)
      return -TSN_err_tooshort;
    tsn_buffer_get64(b, &n->ADAddr);
  }
  if (b->len < 2)
    return -TSN_err_tooshort;
  tsn_buffer_get16(b, &n->AttrLen);
  if (b->len < n->AttrLen)
    return -TSN_err_tooshort;
  
  n->Attrs = TSN_BUFFER_PTR(b);
  return TSN_err_none;
}

void
tsn_dlpdu_adgw_print(tsn_gw_dlpdu_normal_s *n)
{
  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    printf("Packet Type: %s\n", __dlpduType2String(n->ServID));
    if (n->ServID > TSN_AD_JOIN_ack)
    {
      printf("\tAccess Device ID: %u\n", n->AdID);
    }
    else
    {
      tsn_print_longaddr(n->ADAddr);
    }
    printf("\tAttibutes Length: %u\n", n->AttrLen);
  }
}

tsn_err_e
tsn_dlpdu_process_adgw(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_gw_dlpdu_normal_s *n = &msg->_dlpdu;

  TSN_event("Received dlpdu packet from AD.\n");

  r = tsn_adgw_dll_dlpdu2normal(b, n);
  if (TSN_err_none != r)
    return r;

  tsn_dlpdu_adgw_print(n);
  
  if (n->ServID >= TSN_dlpdu_type_max)
    return -TSN_err_unsupport;

  TSN_debug("Processing packet: %s.\n", 
    __dlpduType2String(n->ServID));

  return (*frameTableGateway[n->ServID].func)(msg);
}

