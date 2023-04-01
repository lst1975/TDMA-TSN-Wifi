#include "tsn_private.h"

#define __DECL_F(n) [TSN_ ## n] = { \
  .number=TSN_ ## n, \
  .name=#n, \
  .func=do_TSN_ ## n, \
}

tsn_err_e
make_TSN_AD_JOIN_request(tsn_msg_s *msg,
  Unsigned64 ADAddr, Unsigned8 NetworkID,
  Unsigned64 PhyAddr)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  AttrLen = 9;

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_AD_JOIN_request.\n");

  tsn_buffer_put8(b, TSN_AD_JOIN_request);
  tsn_buffer_put64(b, ADAddr);
  tsn_buffer_put16(b, AttrLen);

  tsn_buffer_put8(b, NetworkID);
  tsn_buffer_put64(b, PhyAddr);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_AD_JOIN_request.\n");
    tsn_print_longaddr(ADAddr);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tNetworkID: %u\n", NetworkID);
    tsn_print_longaddr(PhyAddr);
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLDE_DATA_indication(tsn_msg_s *msg,
  void *_ind, Unsigned8 AdID,
  tsn_buffer_s *data)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_dlde_data_transmit_indication_s *ind;

  ind = (tsn_dlde_data_transmit_indication_s *)_ind;
  AttrLen = 5+TSN_BUFFER_LEN(data);

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLDE_DATA_indication.\n");

  tsn_buffer_put8(b, TSN_DLDE_DATA_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);

  tsn_buffer_put16(b, ind->srcAddr);
  tsn_buffer_put8(b, ind->DataType);
  tsn_buffer_put16(b, ind->PayloadLength);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLDE_DATA_indication.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tsrcAddr: %#x\n", ind->srcAddr);
    tsn_print("\tDataType: %s\n", get_DLDE_DataType(ind->DataType));
    tsn_print("\tPayloadLength: %u\n", ind->PayloadLength);
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_JOIN_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned64 PhyAddr,
  Unsigned64 SecMaterial)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  AttrLen = 16;

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLME_JOIN_indication.\n");

  tsn_buffer_put8(b, TSN_DLME_JOIN_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);

  tsn_buffer_put64(b, PhyAddr);
  tsn_buffer_put64(b, SecMaterial);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_JOIN_indication.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print_longaddr(PhyAddr);
    tsn_print("\tSecMaterial: %"PRIu64"\n", SecMaterial);
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_DEV_STATUS_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 ShortAddr,
  Unsigned8 PowerSupplyStatus)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  AttrLen = 3;

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLME_DEV_STATUS_indication.\n");

  tsn_buffer_put8(b, TSN_DLME_DEV_STATUS_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);

  tsn_buffer_put16(b, ShortAddr);
  tsn_buffer_put8(b, PowerSupplyStatus);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_DEV_STATUS_indication.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tShortAddr: %#x\n", ShortAddr);
    tsn_print("\tPowerSupplyStatus: %u\n", PowerSupplyStatus);
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_CHAN_COND_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 SrcAddr, Unsigned8 Count,
  void *_ChannelConditionInfo)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  channel_condition_infomation_s *ChannelConditionInfo = (channel_condition_infomation_s *)_ChannelConditionInfo;
  AttrLen = 3+Count * 7;

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLME_CHAN_COND_indication.\n");

  tsn_buffer_put8(b, TSN_DLME_CHAN_COND_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);
  tsn_buffer_put16(b, SrcAddr);
  tsn_buffer_put8(b, Count);

  for(int i = 0;i < Count;i++)
  {
    tsn_buffer_put8(b, ChannelConditionInfo[i].ChannelID);
    tsn_buffer_put8(b, ChannelConditionInfo[i].LinkQuality);
    tsn_buffer_put32(b, *(Unsigned32 *)&ChannelConditionInfo[i].PacketLossRate);
    tsn_buffer_put8(b, ChannelConditionInfo[i].RetryNumber);
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_CHAN_COND_indication.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tSrcAddr: %#x\n", SrcAddr);
    tsn_print("\tCount: %u\n", Count);

    for(int i = 0;i < Count;i++)
    {
      tsn_print("\tChannelConditionInfo[%d]:\n", i);
      tsn_print("\tChannelID: %u\n", ChannelConditionInfo[i].ChannelID);
      tsn_print("\tLinkQuality: %u\n", ChannelConditionInfo[i].LinkQuality);
      tsn_print("\tPacketLossRate: %f\n", *(float *)&ChannelConditionInfo[i].PacketLossRate);
      tsn_print("\tRetryNumber: %u\n", ChannelConditionInfo[i].RetryNumber);
    }
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_TIME_SYNC_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 SrcAddr,
  Unsigned64 FieldDeviceTimeValue)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  AttrLen = 10;

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLME_TIME_SYNC_indication.\n");

  tsn_buffer_put8(b, TSN_DLME_TIME_SYNC_indication);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);

  tsn_buffer_put16(b, SrcAddr);
  tsn_buffer_put64(b, FieldDeviceTimeValue);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_TIME_SYNC_indication.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tSrcAddr: %#x\n", SrcAddr);
    tsn_print("\tFieldDeviceTimeValue: %"PRIu64"\n", FieldDeviceTimeValue);
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_INFO_GET_confirm(tsn_msg_s *msg,
  void *_cfm, Unsigned8 AdID,
  tsn_buffer_s *data)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_get_confirm_s *cfm;

  cfm = (dlme_information_get_confirm_s *)_cfm;
  AttrLen = 10+TSN_BUFFER_LEN(data);

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLME_INFO_GET_confirm.\n");

  tsn_buffer_put8(b, TSN_DLME_INFO_GET_confirm);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);

  tsn_buffer_put8(b, cfm->Handle);
  tsn_buffer_put16(b, cfm->SrcAddr);
  tsn_buffer_put8(b, cfm->Status);
  tsn_buffer_put8(b, cfm->AttributeID);
  tsn_buffer_put8(b, cfm->MemberID);
  tsn_buffer_put16(b, cfm->FirstStoreIndex);
  tsn_buffer_put16(b, cfm->Count);

  tsn_buffer_putlen(b, data->data, data->len);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_INFO_GET_confirm.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tHandle: %u\n", cfm->Handle);
    tsn_print("\tSrcAddr: %#x\n", cfm->SrcAddr);
    tsn_print("\tStatus: %s\n", dlmeInformationGetConfirmStatus2String(cfm->Status));
    tsn_print("\tAttributeID: %u\n", cfm->AttributeID);
    tsn_print("\tMemberID: %u\n", cfm->MemberID);
    tsn_print("\tFirstStoreIndex: %u\n", cfm->FirstStoreIndex);
    tsn_print("\tCount: %u\n", cfm->Count);
  }

  return TSN_err_none;
}

tsn_err_e
make_TSN_DLME_INFO_SET_confirm(tsn_msg_s *msg,
  void *_cfm, Unsigned8 AdID)
{
  int AttrLen;
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_set_confirm_s *cfm;

  cfm = (dlme_information_set_confirm_s *)_cfm;
  AttrLen = 3;

  r = ___make_TSN_Buffer(b, 4+AttrLen);
  if (TSN_err_none != r)
    return r;

  TSN_event("Make TSN_DLME_INFO_SET_confirm.\n");

  tsn_buffer_put8(b, TSN_DLME_INFO_SET_confirm);
  tsn_buffer_put8(b, AdID);
  tsn_buffer_put16(b, AttrLen);
  tsn_buffer_put8(b, cfm->Handle);
  tsn_buffer_put8(b, cfm->Status);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_INFO_SET_confirm.\n");
    tsn_print("\tAdID: %u\n", AdID);
    tsn_print("\tAttrLen: %u\n", AttrLen);
    tsn_print("\tHandle: %u\n", cfm->Handle);
    tsn_print("\tStatus: %s\n", dlme_info_set_cfm_status2string(cfm->Status));
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_AD_JOIN_request(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_AD_JOIN_ack(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_ad_join_response_s cfm;

  if(TSN_BUFFER_LEN(b) < 1)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_AD_JOIN_response.\n");

  tsn_buffer_get8(b, &cfm.Status);

  if (cfm.Status == TSN_AD_JOIN_ACK_SUCCESS)
  {
    tsn_buffer_get8(b, &cfm.AdID);
    tsn_buffer_get16(b, &cfm.ADAddr);
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_AD_JOIN_response.\n");
    tsn_print("\tStatus: %s.\n", AdJoinResponseStatus2String(cfm.Status));
    if (cfm.Status == TSN_AD_JOIN_ACK_SUCCESS)
    {
      tsn_print("\tAdID: %u.\n", cfm.AdID);
      tsn_print("\tADAddr: %#x.\n", cfm.ADAddr);
    }
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_GACK_indication(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_gack_indication_s ind;

  if(TSN_BUFFER_LEN(b) < 1)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_GACK_indication.\n");

  tsn_buffer_get8(b, &ind.DeviceCount);

  if (ind.DeviceCount <= 0)
    return -TSN_err_invalid;

  // get8 temporarily
  gack_info_s GAckInfo[ind.DeviceCount];
  for(int i = 0;i < ind.DeviceCount;i++)
  {
      tsn_buffer_get8(b, &GAckInfo[i].Addr.AddrU8);
      tsn_buffer_get16(b, &GAckInfo[i].Seq);
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_GACK_indication.\n");
    tsn_print("\tCount: %u.\n", ind.DeviceCount);
    for (int i = 0;i < ind.DeviceCount;i++)
    {
        tsn_print("\tAddr: %#x.\n", GAckInfo[i].Addr.AddrU8);
        tsn_print("\tSeq: %u.\n", GAckInfo[i].Seq);
    }
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_NACK_indication(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_nack_indication_s ind;

  if(TSN_BUFFER_LEN(b) < 1)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_NACK_indication.\n");

  tsn_buffer_get8(b, &ind.RetryDeviceCount);

  if (ind.RetryDeviceCount <= 0)
    return -TSN_err_invalid;

  // get8 temporarily
  tsn_addr_s DstAddressList[ind.RetryDeviceCount];
  for(int i = 0;i < ind.RetryDeviceCount;i++)
  {
      tsn_buffer_get8(b, &ind.DstAddressList[i].AddrU8);
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_GACK_indication.\n");
    tsn_print("\tCount: %u.\n", ind.RetryDeviceCount);
    for (int i = 0;i < ind.RetryDeviceCount;i++)
    {
        tsn_print("\tDstAddressList: %#x.\n", ind.DstAddressList[i].AddrU8);
    }
  }


  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_DLDE_DATA_request(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_dlde_data_transmit_request_s req;

  if(TSN_BUFFER_LEN(b) < 8)
    return -TSN_err_tooshort;

  TSN_event("Received TSN_DLDE_DATA_request.\n");

  tsn_buffer_get16(b, &req.DstAddr);
  tsn_buffer_get16(b, &req.VCR_ID);
  tsn_buffer_get8(b, &req.DataType);
  tsn_buffer_get8(b, &req.Priority);
  tsn_buffer_get16(b, &req.PayloadLength);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLDE_DATA_request.\n");
    tsn_print("\tDstAddr: %#x.\n", req.DstAddr);
    tsn_print("\tVCR_ID: %u.\n", req.VCR_ID);
    tsn_print("\tDataType: %s.\n", get_DLDE_DataType(req.DataType));
    tsn_print("\tPriority: %s.\n", get_DLDE_DataPriority(req.Priority));
    tsn_print("\tPayloadLength: %u.\n", req.PayloadLength);
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_DLDE_DATA_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_DLME_JOIN_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_DLME_JOIN_response(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_join_response_s res;

  TSN_event("Received TSN_DLME_JOIN_response.\n");

  if (TSN_BUFFER_LEN(b) < 1)
    return -TSN_err_tooshort;

  tsn_buffer_get8(b, &res.Status);

  if (res.Status == DLME_JOIN_SUCCESS)
  {
      tsn_buffer_get16(b, &res.ShortAddr);
  }

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_JOIN_response.\n");
    tsn_print("\tStatus: %s.\n", dlmeJoinStatus2String(res.Status));
    if (res.Status == DLME_JOIN_SUCCESS)
    {
        tsn_print("\tShortAddr: %#x.\n", res.ShortAddr);
    }
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_DLME_DEV_STATUS_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_DLME_CHAN_COND_indication(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
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
    tsn_print("\tTSN_DLME_Time_Sync_indication.\n");
    tsn_print("\tSrcAddr: %#x.\n", ind.SrcAddr);
    tsn_print("\tFieldDeviceTimeValue: %"PRIu64".\n",
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
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_get_request_s req;

  TSN_event("Received TSN_DLME_INFO_GET_request.\n");

  if (TSN_BUFFER_LEN(b) < sizeof(req))
    return -TSN_err_tooshort;

  tsn_buffer_getlen(b, (uint8_t *)&req, sizeof(req));

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_INFO_GET_request.\n");
    tsn_print("\tHandle: %u.\n", req.Handle);
    tsn_print("\tDstAddr: %#x.\n", req.DstAddr);
    tsn_print("\tAttributeID: %u.\n", req.AttributeID);
    tsn_print("\tMemberID: %u.\n", req.MemberID);
    tsn_print("\tFirstStoreIndex: %u.\n", req.FirstStoreIndex);
    tsn_print("\tCount: %u.\n", req.Count);
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_DLME_INFO_GET_confirm(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_DLME_INFO_SET_request(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_information_set_request_s req;

  TSN_event("Received TSN_DLME_INFO_SET_request.\n");

  if (TSN_BUFFER_LEN(b) < sizeof(req))
    return -TSN_err_tooshort;

  tsn_buffer_getlen(b, (uint8_t *)&req, sizeof(req));

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_INFO_SET_request.\n");
    tsn_print("\tHandle: %u.\n", req.Handle);
    tsn_print("\tDstAddr: %#x.\n", req.DstAddr);
    tsn_print("\tAttributeOption: %s.\n", dlme_info_op2string(req.AttributeOption));
    tsn_print("\tAttributeID: %u.\n", req.AttributeID);
    tsn_print("\tMemberID: %u.\n", req.MemberID);
    tsn_print("\tFirstStoreIndex: %u.\n", req.FirstStoreIndex);
    tsn_print("\tCount: %u.\n", req.Count);
  }

  return TSN_err_none;
}

static tsn_err_e
do_TSN_DLME_INFO_SET_confirm(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

static tsn_err_e
do_TSN_DLME_LEAVE_request(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  dlme_leave_request_s req;

  TSN_event("Received TSN_DLME_LEAVE_request.\n");

  if (TSN_BUFFER_LEN(b) < 2)
    return -TSN_err_tooshort;

  tsn_buffer_get16(b, &req.ShortAddr16);

  if (sysCfg.dumpPacket || sysCfg.logDebug)
  {
    tsn_print("\tTSN_DLME_LEAVE_request.\n");
    tsn_print("\tShortAddr16: %#x.\n", req.ShortAddr16);
  }

  return TSN_err_none;
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
    tsn_print("Packet Type: %s\n", __dlpduType2String(n->ServID));
    if (n->ServID > TSN_AD_JOIN_ack)
    {
      tsn_print("\tAccess Device ID: %u\n", n->AdID);
    }
    else
    {
      tsn_print_longaddr(n->ADAddr);
    }
    tsn_print("\tAttibutes Length: %u\n", n->AttrLen);
  }
}

tsn_err_e
tsn_dlpdu_process_adgw(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_buffer_s *b = &msg->b;
  tsn_gw_dlpdu_normal_s *n = (tsn_gw_dlpdu_normal_s *)msg->dlpdu;

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

