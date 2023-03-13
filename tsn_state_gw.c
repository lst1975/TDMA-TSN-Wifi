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

static tsn_boolean_e 
gw_dmap_state_machine(tsn_msg_s *msg, tsn_device_s *dev, 
   tsn_dmap_trigger_e trigger, void *dlpdu);

/* DMAP ---> Device Management Application Process */

static inline tsn_boolean_e 
__DLME_join_response(tsn_msg_s *msg, Unsigned8 AdID, 
  Unsigned8 Status, Unsigned16 ShortAddr)
{
  if (TSN_err_none != make_TSN_DLME_JOIN_response(msg, AdID, Status, ShortAddr))
    return TSN_FALSE;
  tsn_send_udp_msg(msg);
  return TSN_TRUE;
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.2.5.1, Page 18, Table 12, Graph 13 
 *                   GW Device DMAP State Machine 
 *
 * No.     Current State         Event/Trigger ==> Action               Next State 
 * T1          Active      DLME-JOIN.indication(PhyAddr,SecMaterial)       Join
 *
 ***********************************************************************************/
tsn_boolean_e 
gw_dmap_T1_receive_authentication_response(tsn_msg_s *msg, 
   tsn_device_s *dmap, DlmeJoinIndication *ind)
{
  return gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T1_auth, ind);
}

tsn_boolean_e 
gw_dmap_T1_receive_allocate_address_response(tsn_msg_s *msg, 
   tsn_device_s *dmap, DlmeJoinIndication *ind)
{
  return gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T1_addr, ind);
}

tsn_err_e 
gw_dmap_T1_receive_dlme_join_indication(tsn_msg_s *msg, 
  tsn_device_s *dmap __TSN_UNUSED, dlme_join_indication_s *ind)
{
  tsn_err_e r;
  tsn_device_s *dev;

  r = TSN_device_find_ByLongAddr(&dev, ind->, ind->PhyAddr);
  if (r != TSN_err_none)
    return r;
  return gw_dmap_state_machine(msg, dev, DMAP_TRIGGER_T1, ind);
}

tsn_boolean_e 
gw_dmap_T5_receive_information_set_confirm(tsn_msg_s *msg, 
   tsn_device_s *dmap, dlme_information_set_confirm_s *cfm)
{
  return gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T5_ResAlloc, cfm);
}

tsn_boolean_e 
gw_dmap_T0_DmapInitializationDone(tsn_msg_s *msg, 
   tsn_device_s *dmap)
{
  return gw_dmap_state_machine(dmap, DMAP_TRIGGER_T0, ind);
}

/***********************************************************************************
 * GB/T26790.2-2015, 8.5.1, Page 85 
 *                   Access Device DLL State Machine 
 *
 ************************************************************************************
 *                                           |
 *                                           |T1
 *                                           |
 *                                           V
 *                                        ++++++++
 *                                        +      +
 *                                        + INIT +
 *                                        +      +
 *                                        ++++++++
 *                                           |
 *                                           |
 *                                           | T2
 *            +++++++++                      |
 *            +       +                      V
 *      ----->+  END  +       T3      ++++++++++++++++                        
 *      |     +       + <-------------+              +                ++++++++++++++  T5
 *      |     +++++++++               +              +      T4        +            +<-----
 *   T16|                             +              + -------------->+  Resource  +     |     
 *      |     +++++++++               +   OPERATION  +                + Allocation +     |
 *      |     +       +               +              + <------------- +            +------
 *      ------+ LEAVE + <-------------+              +      T6        ++++++++++++++
 *            +       +      T15      +              +
 *            +++++++++               ++++++++++++++++    
 *                                         |    ^
 *                                         |    |
 *                                         |    |
 *                                         |    |
 *                                         |    |
 *                                         |-----
 *                                  T7||T8||...||T14
 *
 ***********************************************************************************/
static tsn_boolean_e 
gw_dmap_state_machine(tsn_msg_s *msg, tsn_device_s *dmap, 
   tsn_dmap_trigger_e trigger, void *dlpdu)
{
  DlmeInformationSetRequest req;

  switch (dmap->MachineState)
  {
    case DMAP_STATE_end:
      return TSN_FALSE;
    case DMAP_STATE_init:
      if (trigger != DMAP_TRIGGER_T1)
        return TSN_FALSE;
      dmap->MachineState = DMAP_STATE_active;
    case DMAP_STATE_active:
      if (trigger != DMAP_TRIGGER_T1)
        return TSN_FALSE;
      dmap->MachineState = DMAP_STATE_join;
    case DMAP_STATE_join:
      if (trigger != DMAP_TRIGGER_T1)
        return TSN_FALSE;
      {
        tsn_boolean_e AuthenResult;
        DlmeJoinIndication *ind = (DlmeJoinIndication *)dlpdu;
        AuthenResult = TSN_Authentication(msg, dmap, ind);
        if (AuthenResult != TSN_SUCCESS)
        {
          __DLME_join_response(
            msg,
            dmap->AccessDeviceID,
            TSN_AD_JOIN_ACK_SYSTEM_ERROR,
            dmap->DeviceShortAddress);
          dmap->MachineState = DMAP_STATE_end;
          return TSN_FALSE;
        }
        dmap->MachineState = DMAP_STATE_join_authenticating;
      }
      break;
      
    case DMAP_STATE_join_authenticating:
      if (trigger != DMAP_TRIGGER_T1_auth)
        return TSN_FALSE;
      {
        DlmeJoinIndication *ind = (DlmeJoinIndication *)dlpdu;
        if (ind->AuthenResult != TSN_SUCCESS)
        {
          __DLME_join_response(
            msg,
            dmap->AccessDeviceID,
            TSN_AD_JOIN_ACK_AUTHENTICATION_Failed, 
            dmap->DeviceShortAddress);
          dmap->MachineState = DMAP_STATE_end;
          return TSN_FALSE;
        }
        else
        {
          uint16_t Addr;
          if (TSN_AllocateShortAddr(&Addr) != TSN_TRUE)
          {
            __DLME_join_response(
              msg,
              dmap->AccessDeviceID,
              TSN_AD_JOIN_ACK_SYSTEM_ERROR, 
              dmap->DeviceShortAddress);
            dmap->TSN_FALSE = DMAP_STATE_end;
            return TSN_FALSE;
          }
          dmap->DeviceShortAddress = Addr;
          dmap->MachineState = DMAP_STATE_operation;
          ind->AddrResult = TSN_SUCCESS;
          return gw_dmap_T1_receive_allocate_address_response(msg,dmap,ind);
        }
      }
      break;
      
    case DMAP_STATE_join_allocatingShortAddress:
      if (trigger != DMAP_TRIGGER_T1_addr)
        return TSN_FALSE;
      {
        DlmeJoinIndication *ind = (DlmeJoinIndication *)dlpdu;
        if (ind->AddrResult != TSN_SUCCESS)
        {
          __DLME_join_response(
            msg,
            dmap->AccessDeviceID,
            TSN_AD_JOIN_ACK_EXCEEDED, 
            dmap->DeviceShortAddress);
          dmap->TSN_FALSE = DMAP_STATE_end;
          return TSN_FALSE;
        }
      }
      __DLME_join_response(msg,TSN_AD_JOIN_ACK_SUCCESS, dmap->DeviceShortAddress);
      dmap->MachineState = DMAP_STATE_operation;
      break;
      
    case DMAP_STATE_operation:
      {
        tsn_buffer b;
        
        {
          uint8_t DeviceState;
          req.Handle          = dmap->Handle;
          req.DstAddr         = TSN_htons(dmap->DeviceShortAddress);
          req.AttributeOption = DLME_information_set_option_UPDATE;
          req.AttributeID     = 131; /* DMAP_mib_id_list_DeviceList */
          req.MemberID        = 12;  /* DMAP_mib_id_device_DeviceState */
          req.FirstStoreIndex = TSN_htons(dmap->DeviceShortAddress);
          req.Count           = TSN_htons(1);
          DeviceState  = DMAP_mib_id_device_DeviceState_AllocatingResources;
          tsn_buffer_set(&b, &DeviceState, 1);
          DLME_information_set_request(msg, &req, dmap->AccessDeviceID, &b);

          TSN_event("Try to send DeviceState : AllocatingResources.\n");
        }
        
        {
          tsn_superframe_s s, *c;
          
          c = &sysCfg.config.SuperframeList;
          req.AttributeOption = DLME_information_set_option_ADD;
          req.AttributeID     = 128; /* DMAP_mib_id_list_SuperframeList */
          req.DstAddr         = TSN_htons(dmap->DeviceShortAddress);
          req.MemberID        = DMAP_mib_id_ALL;
          req.FirstStoreIndex = TSN_htons(dmap->DeviceShortAddress);
          req.Count           = TSN_htons(1);

          s.SuperframeID = c->SuperframeID;
          s.NumberSlots  = TSN_htons(c->NumberSlots);
          s.ActiveFlag   = c->ActiveFlag;
          tsn_memcpy(&s.ActiveSlot, &c.ActiveSlot, sizeof(c.ActiveSlot));
          tsn_buffer_set(&b, &s, sizeof(s));
          DLME_information_set_request(msg, &req, dmap->AccessDeviceID, &b);

          TSN_event("Try to send SuperframeList.\n");
        }

        {
          tsn_dll_link_s s, *c;
          
          c = &sysCfg.config.DllLinkList;
          req.AttributeOption = DLME_information_set_option_ADD;
          req.AttributeID     = 129; /* DMAP_mib_id_list_DllLinkList */
          req.DstAddr         = TSN_htons(dmap->DeviceShortAddress);
          req.MemberID        = DMAP_mib_id_ALL;
          req.FirstStoreIndex = TSN_htons(dmap->DeviceShortAddress);
          req.Count           = TSN_htons(1);

          s.LinkID               = TSN_htons(c->LinkID);
          s.LinkType             = c->LinkType;
          s.PacketLossRate       = TSN_htonl(c->PacketLossRate);
          s.PeerAddr             = TSN_htons(c->PacketLossRate);
          s.RelativeSlotNumber   = TSN_htons(c->RelativeSlotNumber);
          s.ChannelIndex         = c->ChannelIndex;
          s.SuperframeID         = c->SuperframeID;
          tsn_buffer_set(&b, &s, sizeof(s));
          DLME_information_set_request(msg, &req, dmap->AccessDeviceID, &b);

          TSN_event("Try to send DllLinkList.\n");
        }
      }
    
      dmap->MachineState = DMAP_STATE_resource_allocation;
      break;
      
    case DMAP_STATE_resource_allocation:
      /* DLME-INFO-SET.confirm() */
      if (AttributeID == 128)
      {
        req.Handle = dmap->Handle;
        req.DstAddr = TSN_htons(dmap->DeviceShortAddress);
        req.AttributeOption = DLME_information_set_option_ADD;
        req.AttributeID = 129;
        req.MemberID = ;
        req.FirstStoreIndex = TSN_htons();
        req.Count = TSN_htons(1);
        req.AttributeValue = ;
        DLME_information_set_request(msg, &req);
      }
      else if (AttributeID == 129)
      {
        if (IsAllResAllocateDone() == TRUE)
        {
          req.Handle = dmap->Handle;
          req.DstAddr = TSN_htons(dmap->DeviceShortAddress);
          req.AttributeOption = 2;
          req.AttributeID = 131;
          req.MemberID = 12;
          req.FirstStoreIndex = TSN_htons();
          req.Count = TSN_htons(1);
          req.AttributeValue = OPERATION;
          DLME_information_set_request(msg, &req);
        }
        break;
      }
      else if (AttributeID == 131)
      {
        dmap->MachineState = DMAP_STATE_operation;
        break;
      }
      else
      {
        return TSN_FALSE;
      }
      break;
      
    default:
      return FALSE;
  }

  return TRUE;
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.2.5.1, Page 17 
 *                   GW Device DMAP State Machine 
 *
 ************************************************************************************
 *                                           |
 *                                           |
 *                                           |
 *                                           V
 *                                        ++++++++
 *                                        +      +
 *                                        + INIT +
 *                                        +      +
 *                                        ++++++++
 *                                           |
 *                                           |
 *                                           |
 *                                           |
 *                                           V
 *                  T2||T3||...T16        ++++++++                     
 *                              --------> +      +      
 *                              |         + IDLE +                      
 *                              --------- +      + 
 *                                        ++++++++   
 *
 *
 ***********************************************************************************/
tsn_boolean_e 
__gw_dmap_state_machine(gw_dmap_state_s *dmap_gw, void *dlpdu, 
tsn_dmap_trigger_e trigger)
{
  switch (dmap_gw->State)
  {
    case DMAP_STATE_init:
      if (trigger == DMAP_TRIGGER_T0)
        dmap_gw->State = DMAP_STATE_active;
      return TRUE;
    case DMAP_STATE_active:
      if (trigger > DMAP_TRIGGER_T0
        || trigger < DMAP_TRIGGER_MAX)
        return gw_dmap_state_machine(NULL, dlpdu, trigger);
      return FALSE;
    default:
      return FALSE;
  }
}

tsn_boolean_e
TSN_IsDmapInitializationDone(void)
{
  return tsn_gateway.Initialized;
}

void
TSN_SetDmapInitializationDone(void)
{
  tsn_gateway.Initialized = TSN_TRUE;
}

