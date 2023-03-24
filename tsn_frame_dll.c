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

static inline tsn_err_e
tsn_dlpdu_dllhdr_convert(tsn_buffer_s *b, 
  tsn_dlpdu_dllhdr_s *n)
{
  tsn_dlpdu_dllhdr_raw_s *hdr;
  if (b->len < tsn_dlpdu_dllhdr_min_len)
    return -TSN_err_tooshort;

  hdr = (tsn_dlpdu_dllhdr_raw_s *)b->ptr;
  TSN_BUFFER_GO(b,2);

  n->type = hdr->type;
  n->is_segment = hdr->is_segment;
  n->is_preemption = hdr->is_preemption;
  n->is_shortaddr = hdr->is_shortaddr;
  n->segment_count = 0;
  n->segment_seq = 0;
  n->length = 0;
  
  if (hdr->is_shortaddr)
  {
    if (sysCfg.useShortAddr)
    { 
      n->addr.AddrType = DMAP_mib_id_static_AddressTypeFlag_u8;
      tsn_buffer_get8(b, &n->addr.AddrU8);
    }
    else
    {
      if (b->len < 2)
        return -TSN_err_tooshort;
      n->addr.AddrType = DMAP_mib_id_static_AddressTypeFlag_u16;
      tsn_buffer_get16(b, &n->addr.AddrU16);
    }
  }
  else
  {
    if (b->len < 4)
      return -TSN_err_tooshort;
    n->addr.AddrType = DMAP_mib_id_static_AddressTypeFlag_u64;
    tsn_buffer_get64(b, &n->addr.AddrU64);
  }

  if (b->len < 2)
    return -TSN_err_tooshort;
  tsn_buffer_get16(b, &n->seq);
  
  if (hdr->is_segment)
  {
    if (b->len < 2)
      return -TSN_err_tooshort;
    tsn_buffer_get8(b, &n->segment_count);
    tsn_buffer_get8(b, &n->segment_seq);
  }
  
  if (b->len < 2)
    return -TSN_err_tooshort;
  tsn_buffer_get16(b, &n->length);

  if (b->len < sysCfg.fcsLength + n->length)
    return -TSN_err_tooshort;
  
  return TSN_err_none;
}

static inline void
tsn_dlpdu_dllhdr_build(tsn_dlpdu_dllhdr_s *n, tsn_buffer_s *b)
{
  tsn_buffer_put8(b, *(Unsigned8 *)n);
  if (n->is_shortaddr)
  {
    if (sysCfg.useShortAddr)
    { 
      tsn_buffer_put8(b, n->addr.AddrU8);
    }
    else
    {
      tsn_buffer_put16(b, n->addr.AddrU16);
    }
  }
  else
  {
    tsn_buffer_put64(b, n->addr.AddrU64);
  }
  tsn_buffer_put16(b, n->seq);
  if (n->is_segment)
  {
    tsn_buffer_put8(b, n->segment_count);
    tsn_buffer_put8(b, n->segment_seq);
  }
  tsn_buffer_put16(b, n->length);
}

static tsn_err_e 
do_FRAME_TYPE_Beacon(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_Data(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_Aggregation(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_GACK(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_NACK(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_JoinRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_JoinResponse(tsn_msg_s *msg)
{
  if (sysCfg.devType == DMAP_mib_device_gw)
    return -TSN_err_unsupport;
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_LeaveRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_LeaveResponse(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_DeviceStatusReport(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_ChannelConditionReport(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_TwoWayTimeSynchronizationRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_TwoWayTimeSynchronizationResponse(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_RemoteAttributeGetRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_RemoteAttributeGetResponse(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_RemoteAttributeSetRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_RemoteAttributeSetResponse(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_KeyEstablishRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_KeyEstablishResponse(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_KeyUpdateRequest(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_KeyUpdateResponse(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}
static tsn_err_e 
do_FRAME_TYPE_SecurityAlarm(tsn_msg_s *msg)
{
  return -TSN_err_unsupport;
}

#define __DECL_F(n) [FRAME_TYPE_ ## n] = { \
  .number=FRAME_TYPE_ ## n, \
  .name=#n, \
  .func=do_FRAME_TYPE_ ## n, \
}
  
static name_number_s frameTableDll[FRAME_TYPE_MAX]={
  __DECL_F(Beacon),
  __DECL_F(Data),
  __DECL_F(Aggregation),
  __DECL_F(GACK),
  __DECL_F(NACK),
  __DECL_F(JoinRequest),
  __DECL_F(JoinResponse),
  __DECL_F(LeaveRequest),
  __DECL_F(LeaveResponse),
  __DECL_F(DeviceStatusReport),
  __DECL_F(ChannelConditionReport),
  __DECL_F(TwoWayTimeSynchronizationRequest),
  __DECL_F(TwoWayTimeSynchronizationResponse),
  __DECL_F(RemoteAttributeGetRequest),
  __DECL_F(RemoteAttributeGetResponse),
  __DECL_F(RemoteAttributeSetRequest),
  __DECL_F(RemoteAttributeSetResponse),
  __DECL_F(KeyEstablishRequest),
  __DECL_F(KeyEstablishResponse),
  __DECL_F(KeyUpdateRequest),
  __DECL_F(KeyUpdateResponse),
  __DECL_F(SecurityAlarm),
};

static void
tsn_dlpdu_dllhdr_print_flags(tsn_dlpdu_dllhdr_s *n)
{
  int pre = 0;
  tsn_print("\tFlags: ");
  if (n->is_segment)
  {
    if (pre)
      tsn_print("|");
    tsn_print("SEGMENT");
  }
  if (n->is_preemption)
  {
    if (pre)
      tsn_print("|");
    tsn_print("PREEMPTION");
  }
  if (n->is_shortaddr)
  {
    if (pre)
      tsn_print("|");
    tsn_print("SHORTADDR");
  }
  tsn_print("\n");
}

static const char *__frameType2String(unsigned int type)
{
  if (type >= FRAME_TYPE_MAX)
    return "<UNKONWN>";
  return frameTableDll[type].name;
}

void
tsn_dlpdu_dllhdr_print(tsn_dlpdu_dllhdr_s *n)
{
  if (sysCfg.dumpPacket)
  {
    tsn_print("Packet Type: %s\n", __frameType2String(n->type));
    tsn_dlpdu_dllhdr_print_flags(n);
    tsn_print_addr(&n->addr);
    tsn_print("\tNetworkID: %u\n", n->networkID);
    tsn_print("\tSequence: %u\n", TSN_ntohs(n->seq));
    if (n->is_segment)
    {
      tsn_print("\tSegment Count: %u\n", n->segment_count);
      tsn_print("\tSegment Sequence: %u\n", n->segment_seq);
    }
    tsn_print("\tLength: %u\n", TSN_ntohs(n->length));
  }
}

tsn_err_e
tsn_dlpdu_process_dll(tsn_msg_s *msg)
{
  tsn_err_e r;
  tsn_dlpdu_dllhdr_s n;
  tsn_buffer_s *b = &msg->b;
  Unsigned8 *base = b->ptr;

  TSN_event("Received dlpdu packet.\n");

  r = tsn_dlpdu_dllhdr_convert(b, &n);
  if (TSN_err_none != r)
    return r;

  tsn_dlpdu_dllhdr_print(&n);
  
  if (n.type >= FRAME_TYPE_MAX)
    return -TSN_err_unsupport;

  TSN_debug("Processing packet: %s.\n", 
    __frameType2String(n.type));

  if (TSN_TRUE != tsn_check_fcs(base, 
          b->ptr-base+n.length, 
          tsn_get_fcs(b->ptr+n.length)))
  {
    TSN_error("Bad FCS checksum, discarded.\n");
    return -TSN_err_malformed;
  }
  
  return (*frameTableDll[n.type].func)(msg);
}
