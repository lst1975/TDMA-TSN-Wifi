#ifndef _TSN_FRAME_AD_H__
#define _TSN_FRAME_AD_H__

tsn_err_e
make_TSN_AD_JOIN_request(tsn_msg_s *msg,
  Unsigned64 ADAddr, Unsigned8 NetworkID,
  Unsigned64 PhyAddr);

tsn_err_e
make_TSN_DLDE_DATA_indication(tsn_msg_s *msg,
  void *_ind, Unsigned8 AdID,
  tsn_buffer_s *data);

tsn_err_e
make_TSN_DLME_JOIN_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned64 PhyAddr,
  Unsigned64 SecMaterial);

tsn_err_e
make_TSN_DLME_DEV_STATUS_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 ShortAddr,
  Unsigned8 PowerSupplyStatus);

tsn_err_e
make_TSN_DLME_CHAN_COND_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 SrcAddr, Unsigned8 Count,
  void *_ChannelConditionInfo);

tsn_err_e
make_TSN_DLME_TIME_SYNC_indication(tsn_msg_s *msg,
  Unsigned8 AdID, Unsigned16 SrcAddr,
  Unsigned64 FieldDeviceTimeValue);

tsn_err_e
make_TSN_DLME_INFO_GET_confirm(tsn_msg_s *msg,
  void *_cfm, Unsigned8 AdID,
  tsn_buffer_s *data);

tsn_err_e
make_TSN_DLME_INFO_SET_confirm(tsn_msg_s *msg,
  void *_cfm, Unsigned8 AdID);

tsn_err_e fd_tsn_dlpdu_process_adgw(tsn_msg_s *msg);

#endif
