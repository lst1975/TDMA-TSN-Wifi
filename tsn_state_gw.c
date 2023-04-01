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
__gw_dmap_state_machine(tsn_msg_s *msg, tsn_device_s *dmap, 
  tsn_dmap_trigger_e trigger, void *dlpdu);

void __ReleaseResources(uint16_t *_Addr)
{
  TSN_FreeShortAddr(_Addr);
}
/* DMAP ---> Device Management Application Process */
static inline tsn_boolean_e 
__DLME_join_response(tsn_msg_s *msg, Unsigned8 AdID, 
  Unsigned8 Status, Unsigned16 ShortAddr)
{
  if (TSN_err_none != make_TSN_DLME_JOIN_response(msg, AdID, Status, ShortAddr))
  {
    tsn_free_msg(msg);
    return TSN_FALSE;
  }
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
   tsn_device_s *dmap, dlme_join_indication_s *ind)
{
  return __gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T1_auth, ind);
}

tsn_boolean_e 
gw_dmap_T1_receive_allocate_address_response(tsn_msg_s *msg, 
   tsn_device_s *dmap, dlme_join_indication_s *ind)
{
  return __gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T2_addr, ind);
}

tsn_err_e 
gw_dmap_T1_receive_dlme_join_indication(tsn_msg_s *msg, 
  tsn_device_s *dmap __TSN_UNUSED, dlme_join_indication_s *ind)
{
  tsn_err_e r;
  int NetworkID;
  tsn_device_s *dev;

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

  r = TSN_device_find_ByLongAddr(&dev, NetworkID, ind->PhyAddr);
  if (r != TSN_err_none)
    return r;
  return __gw_dmap_state_machine(msg, dev, DMAP_TRIGGER_T1_join, ind) 
            == TSN_TRUE ? TSN_err_none : -TSN_err_system;
}

tsn_boolean_e 
gw_dmap_T5_receive_information_set_confirm(tsn_msg_s *msg, 
   tsn_device_s *dmap, dlme_information_set_confirm_s *cfm)
{
  return __gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T5_infoSetConfirm, cfm);
}

tsn_boolean_e 
gw_dmap_T0_DmapInitializationDone(tsn_msg_s *msg, 
   tsn_device_s *dmap)
{
  return __gw_dmap_state_machine(msg, dmap, DMAP_TRIGGER_T0, NULL);
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
 *  T16 |                             +              + -------------->+  Resource  +     |     
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
#define DLME_STATE_information_set_handle_mask           0xF000
#define DLME_STATE_information_set_DeviceList       0x1000
#define DLME_STATE_information_set_SuperframeList   0x2000
#define DLME_STATE_information_set_DllLinkList      0x4000
static tsn_boolean_e 
gw_dmap_state_machine(tsn_msg_s *msg, tsn_device_s *dmap, 
   tsn_dmap_trigger_e trigger, void *dlpdu)
{
  switch (dmap->MachineState)
  {
    case DMAP_STATE_leave:
      if (trigger != DMAP_TRIGGER_T16)
      {
        goto ret_free_msg_false;
      }
      else
      {
        dlme_leave_request_s *req;
        req = (dlme_leave_request_s *)dlpdu;
        __ReleaseResources(&req->ShortAddr16);
        dmap->MachineState = DMAP_STATE_end;
        tsn_free_msg(msg);
      }
      break;
      
    case DMAP_STATE_end:
      goto ret_free_msg_false;

    case DMAP_STATE_init:
      if (trigger != DMAP_TRIGGER_T1_join)
      {
        goto ret_free_msg_false;
      }
      dmap->MachineState = DMAP_STATE_active;
      /* FALLTHROUGH */

    case DMAP_STATE_active:
      if (trigger != DMAP_TRIGGER_T1_join)
      {
        goto ret_free_msg_false;
      }
      dmap->MachineState = DMAP_STATE_join;
      /* FALLTHROUGH */

    case DMAP_STATE_join:
      if (trigger != DMAP_TRIGGER_T1_join)
      {
        goto ret_free_msg_false;
      }
      else
      {
        tsn_boolean_e AuthenResult;
        dlme_join_indication_s *ind = (dlme_join_indication_s *)dlpdu;
        AuthenResult = TSN_Authentication(msg, dmap, ind);
        if (AuthenResult != TSN_SUCCESS)
        {
          __DLME_join_response(
            msg,
            dmap->AccessDeviceID,
            TSN_AD_JOIN_ACK_SYSTEM_ERROR,
            dmap->DeviceShortAddress);
          dmap->MachineState = DMAP_STATE_end;
          return TSN_TRUE;
        }
        dmap->MachineState = DMAP_STATE_join_authenticating;
        return gw_dmap_T1_receive_authentication_response(msg, dmap, ind);
      }
      break;
      
    case DMAP_STATE_join_authenticating:
      if (trigger != DMAP_TRIGGER_T1_auth)
      {
        goto ret_free_msg_false;
      }
      else
      {
        dlme_join_indication_s *ind = (dlme_join_indication_s *)dlpdu;
        if (ind->AuthenResult != TSN_SUCCESS)
        {
          __DLME_join_response(
            msg,
            dmap->AccessDeviceID,
            TSN_AD_JOIN_ACK_AUTHENTICATION_Failed, 
            dmap->DeviceShortAddress);
          dmap->MachineState = DMAP_STATE_end;
          return TSN_TRUE;
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
            dmap->MachineState = DMAP_STATE_end;
            return TSN_TRUE;
          }
          dmap->DeviceShortAddress = Addr;
          dmap->MachineState = DMAP_STATE_join_allocatingShortAddress;
          ind->AddrResult = TSN_SUCCESS;
          return gw_dmap_T1_receive_allocate_address_response(msg,dmap,ind);
        }
      }
      break;
      
    case DMAP_STATE_join_allocatingShortAddress:
      if (trigger != DMAP_TRIGGER_T2_addr)
      {
        goto ret_free_msg_false;
      }
      else
      {
        dlme_join_indication_s *ind = (dlme_join_indication_s *)dlpdu;
        if (ind->AddrResult != TSN_SUCCESS)
        {
          __DLME_join_response(
            msg,
            dmap->AccessDeviceID,
            TSN_AD_JOIN_ACK_EXCEEDED, 
            dmap->DeviceShortAddress);
          dmap->MachineState = DMAP_STATE_end;
          return TSN_TRUE;
        }
      }
      __DLME_join_response(msg, dmap->AccessDeviceID, 
        TSN_AD_JOIN_ACK_SUCCESS, dmap->DeviceShortAddress);
      dmap->MachineState = DMAP_STATE_resource_allocation;
      
      trigger = DMAP_TRIGGER_T4_ResAlloc;
      /* FALLTHROUGH */
      
    case DMAP_STATE_resource_allocation:
      if (trigger == DMAP_TRIGGER_T4_ResAlloc)
      {
        tsn_buffer_s b;
        dlme_information_set_request_s req;
        
        {
          uint8_t DeviceState;
          
          req.DstAddr         = TSN_htons(dmap->DeviceShortAddress);
          req.AttributeOption = DLME_information_set_option_UPDATE;
          req.AttributeID     = DMAP_mib_id_list_DeviceList; /* 131 */
          req.MemberID        = DMAP_mib_id_device_DeviceState;  /* 12 */
          req.FirstStoreIndex = TSN_htons(dmap->DeviceShortAddress);
          req.Count           = TSN_htons(1);
          DeviceState  = DMAP_mib_id_device_DeviceState_AllocatingResources;
          dmap->Flags |= DLME_STATE_information_set_DeviceList;
          tsn_buffer_set(&b, &DeviceState, 1);
          DLME_information_set_request(msg, &req, dmap->AccessDeviceID, dmap->Network, &b);

          TSN_event("Try to send DeviceState : AllocatingResources.\n");
        }
        
        {
          tsn_superframe_s s, *c;
          
          c = &sysCfg.config.SuperframeList;
          req.DstAddr         = TSN_htons(dmap->DeviceShortAddress);
          req.AttributeOption = DLME_information_set_option_ADD;
          req.AttributeID     = DMAP_mib_id_list_SuperframeList; /* 128 */
          req.MemberID        = DMAP_mib_id_ALL;
          req.FirstStoreIndex = TSN_htons(dmap->DeviceShortAddress);
          req.Count           = TSN_htons(1);
          dmap->Flags |= DLME_STATE_information_set_SuperframeList;

          s.SuperframeID = c->SuperframeID;
          s.NumberSlots  = TSN_htons(c->NumberSlots);
          s.ActiveFlag   = c->ActiveFlag;
          tsn_memcpy(&s.ActiveSlot, &c->ActiveSlot, sizeof(c->ActiveSlot));
          tsn_buffer_set(&b, (Unsigned8 *)&s, sizeof(s));
          DLME_information_set_request(msg, &req, dmap->AccessDeviceID, dmap->Network, &b);

          TSN_event("Try to send SuperframeList.\n");
        }

        {
          tsn_dll_link_s s, *c;
          
          c = &sysCfg.config.DllLinkList;
          req.DstAddr         = TSN_htons(dmap->DeviceShortAddress);
          req.AttributeOption = DLME_information_set_option_ADD;
          req.AttributeID     = DMAP_mib_id_list_DllLinkList; /* 129 */
          req.MemberID        = DMAP_mib_id_ALL;
          req.FirstStoreIndex = TSN_htons(dmap->DeviceShortAddress);
          req.Count           = TSN_htons(1);
          dmap->Flags |= DLME_STATE_information_set_DllLinkList;

          s.LinkID               = TSN_htons(c->LinkID);
          s.LinkType             = c->LinkType;
          *(uint32_t*)&s.PacketLossRate = TSN_htonl(*(uint32_t*)&c->PacketLossRate);
          s.PeerAddr             = TSN_htons(c->PeerAddr);
          s.RelativeSlotNumber   = TSN_htons(c->RelativeSlotNumber);
          s.ChannelIndex         = c->ChannelIndex;
          s.SuperframeID         = c->SuperframeID;
          tsn_buffer_set(&b, (Unsigned8 *)&s, sizeof(s));
          DLME_information_set_request(msg, &req, dmap->AccessDeviceID, dmap->Network, &b);

          TSN_event("Try to send DllLinkList.\n");
        }

        tsn_free_msg(msg);
        return TSN_TRUE;
      }
      else if (trigger == DMAP_TRIGGER_T5_infoSetConfirm)
      {
        /* DLME-INFO-SET.confirm() */
        dlme_information_set_confirm_s *cfm;

        cfm = (dlme_information_set_confirm_s *)dlpdu;
        if (cfm->Status != DLME_information_set_response_SUCCESS)
        {
          dmap->MachineState = DMAP_STATE_end;
          goto ret_free_msg_false;
        }

        switch (cfm->Handle & DLME_STATE_information_set_handle_mask) 
        {
          case DLME_STATE_information_set_DeviceList:
            dmap->Flags &= ~DLME_STATE_information_set_DeviceList;
            break;
          case DLME_STATE_information_set_SuperframeList:
            dmap->Flags &= ~DLME_STATE_information_set_SuperframeList;
            break;
          case DLME_STATE_information_set_DllLinkList:
            dmap->Flags &= ~DLME_STATE_information_set_DllLinkList;
            break;
          default:
            dmap->MachineState = DMAP_STATE_end;
            goto ret_free_msg_false;
        }
        if (!dmap->Flags)
          dmap->MachineState = DMAP_STATE_operation;

        return TSN_TRUE;
      }
      else
      {
        goto ret_free_msg_false;
      }
      break;
      
    case DMAP_STATE_operation:
#if 0
      if (trigger == DMAP_TRIGGER_T7)
      {
        TsnDmapMibSetRequestS *req;
        req = (TsnDmapMibSetRequestS *)dlpdu;

        Unsigned8 Status = WriteToMIB(req);

        TsnDmapMibSetConfirmS cfm;
        cfm.Handle = req->Handle;
        cfm.Status = Status;
        DMAP_MIB_set_confirm(cfm);
      }
      else if (trigger == DMAP_TRIGGER_T8)
      {
        TsnDmapMibGetRequestS *req;
        req = (TsnDmapMibSetRequestS *)dlpdu;

        Unsigned8 Status = ReadFromMIB(req);

        TsnDmapMibGetConfirmS cfm;
        cfm.Handle = req->Handle;
        cfm.Status = Status;
        cfm.Count  = req->Count;
        tsn_memcpy(&cfm.AttributeValue, &req->IndexValue, sizeof(req->IndexValue));
        DMAP_mib_get_confirm(cfm);
      }
      else if (trigger == DMAP_TRIGGER_T9)
      {
        dlme_information_set_request_s *req;
        req = (dlme_information_set_request_s *)dlpdu;

        DLME_information_set_request(req);
      }
      else if (trigger == DMAP_TRIGGER_T10)
      {
        dlme_information_set_confirm_s *cfm;
        cfm = (dlme_information_set_confirm_s *)dlpdu;

        IndicateSetMIBResult(cfm->Handle, cfm->Status);
      }
      else if (trigger == DMAP_TRIGGER_T11)
      {
        dlme_information_get_request_s *req;
        req = (dlme_information_get_request_s *)dlpdu;

        DLME_information_get_request(req);
      }
      else if (trigger == DMAP_TRIGGER_T12)
      {
        dlme_information_get_confirm_s *cfm;
        cfm = (dlme_information_get_confirm_s *)dlpdu;

        IndicateGetMIBResult(cfm);
      }
      else if (trigger == DMAP_TRIGGER_T13)
      {
        dlme_channel_condition_indication_s *ind;
        ind = (dlme_channel_condition_indication_s *)dlpdu;

        HandleChannelStatusReport(ind);
      }
      else if (trigger == DMAP_TRIGGER_T14)
      {
        dlme_device_status_indication_s *ind;
        ind = (dlme_device_status_indication_s *)dlpdu;

        HandleDeviceStatusReport(ind);
      }
      else if (trigger == DMAP_TRIGGER_T15)
      {
        dlme_leave_request_s *req;
        req = (dlme_leave_request_s *)dlpdu;
        DLME_leave_request(req->ShortAddr16);
        IndicateHostComputerLeaveResult(req->ShortAddr16);
      }
      else
      {
        goto ret_free_msg_false;
      }
#endif
      break;
      
    default:
      goto ret_free_msg_false;
  }

  return TSN_TRUE;

ret_free_msg_false:
  tsn_free_msg(msg);
  return TSN_FALSE;
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
static tsn_boolean_e 
__gw_dmap_state_machine(tsn_msg_s *msg, tsn_device_s *dmap, 
   tsn_dmap_trigger_e trigger, void *dlpdu)
{
  switch (sysCfg.State)
  {
    case DMAP_STATE_init:
      if (trigger != DMAP_TRIGGER_T0)
        return TSN_FALSE;
      sysCfg.State = DMAP_STATE_active;
      /* FALLTHROUGH */
      
    case DMAP_STATE_active:
      if (trigger > DMAP_TRIGGER_T0 && trigger < DMAP_TRIGGER_MAX)
        return gw_dmap_state_machine(msg, dmap, trigger, dlpdu);
      return TSN_FALSE;
      
    default:
      return TSN_FALSE;
  }
}

tsn_boolean_e
TSN_IsDmapInitializationDone(void)
{
  return sysCfg.Initialized;
}

void
TSN_SetDmapInitializationDone(void)
{
  __gw_dmap_state_machine(NULL, NULL, DMAP_TRIGGER_T0, NULL);
  sysCfg.Initialized = TSN_TRUE;
}

