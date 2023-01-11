/* DMAP ---> Device Management Application Process */

struct gw_dmap_state{
	int State;
	void *Handle;
};
typedef struct gw_dmap_state gw_dmap_state_s;

struct _tsn_gateway{
	Unsigned8 NetworkID;
	tsn_boolean_e Initialized;
	gw_dmap_state_s dmap_states_all[TSN_FieldDevice_MAX];
};
static struct _tsn_gateway tsn_gateway = {
	.NetworkID = 0,
	.Initialized = TSN_FALSE
};

static inline tsn_boolean_e 
__DLME_join_response(Unsigned8 Status, Unsigned16 ShortAddr)
{
	dlme_join_response_s rsp = { .Status = Status, .ShortAddr: ShortAddr};
	DLME_join_response(&rsp);
}

/***********************************************************************************
 * GB/T26790.2-2015, 6.2.5.1, Page 18, Table 12, Graph 13 
 *                   GW Device DMAP State Machine 
 *
 * No.     Current State         Event/Trigger ==> Action               Next State 
 * T1          Active      DLME-JOIN.indication(PhyAddr,SecMaterial)       Join
 *
 ***********************************************************************************/
static inline gw_dmap_state_s *
gw_dmap_get_state_by_phyddr(uint64_t PhyAddr)
{
	int deviceID = tsn_get_deviceID(PhyAddr);
	if (deviceID <=0 || deviceID > TSN_FieldDevice_MAX-1)
	{
		__DLME_join_response(TSN_AD_JOIN_ACK_INVALID_DeviceID, 0);
		return TSN_FALSE;
	}
	return &tsn_gateway.dmap_states_all[deviceID];
}

tsn_err_e 
gw_dmap_T1_receive_dlme_join_indication(void *dlpdu, 
  DlmeJoinIndication *ind)
{
	TSN_DMAP_mib_attribute_s *mib = TSN_DMAP_mib_device_find_ByLongAddr(ind->PhyAddr);
	return gw_dmap_state_machine(mib, DMAP_TRIGGER_T1, ind);
}

tsn_boolean_e 
gw_dmap_T1_receive_authentication_response(uint64_t PhyAddr,
	uint64_t SecMaterial, tsn_boolean_e AuthenResult)
{
	gw_dmap_state_s *dmap = gw_dmap_get_state_by_phyddr(PhyAddr);
	DlmeJoinIndication ind = {PhyAddr,SecMaterial,AuthenResult};
	return gw_dmap_state_machine(dmap, DMAP_TRIGGER_T1_auth, &ind);
}

tsn_boolean_e 
gw_dmap_T1_receive_allocate_address_response(uint64_t PhyAddr,
	uint64_t SecMaterial, short_addr_u ShortAddr)
{
	gw_dmap_state_s *dmap = gw_dmap_get_state_by_phyddr(PhyAddr);
	DlmeJoinIndication ind = {PhyAddr,SecMaterial,.ShortAddr = ShortAddr};
	return gw_dmap_state_machine(dmap, DMAP_TRIGGER_T1_addr, &ind);
}

tsn_boolean_e 
gw_dmap_T0_DmapInitializationDone(DlmeJoinIndication *ind)
{
	gw_dmap_state_s *dmap = tsn_gateway.dmap_states_all[0];
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
tsn_boolean_e 
gw_dmap_state_machine(gw_dmap_state_s *dmap, void *dlpdu, 
   tsn_dmap_trigger_e trigger)
{
	struct DlmeInformationSetRequest req;

 	switch (dmap->State)
	{
		case DMAP_STATE_end:
			return TSN_FALSE;
		case DMAP_STATE_init:
			if (trigger != DMAP_TRIGGER_T1)
				return TSN_FALSE;
			dmap->State = DMAP_STATE_active;
		case DMAP_STATE_active:
			if (trigger != DMAP_TRIGGER_T1)
				return TSN_FALSE;
			dmap->State = DMAP_STATE_join;
		case DMAP_STATE_join:
			if (trigger != DMAP_TRIGGER_T1)
				return TSN_FALSE;
			{
			tsn_boolean_e AuthenResult;
			DlmeJoinIndication *ind = (DlmeJoinIndication *)dlpdu;
			AuthenResult = TSN_Authentication(ind->PhyAddr, ind->SecMaterial);
			if (AuthenResult != TSN_SUCCESS)
			{
				__DLME_join_response(TSN_AD_JOIN_ACK_SYSTEM_ERROR, 0);
				dmap->State = DMAP_STATE_end;
				return TSN_FALSE;
			}
			dmap->State = DMAP_STATE_join_authenticating;
			}
			break;
		case DMAP_STATE_join_authenticating:
			if (trigger != DMAP_TRIGGER_T1_auth)
				return TSN_FALSE;
			{
			DlmeJoinIndication *ind = (DlmeJoinIndication *)dlpdu;
			if (ind->AuthenResult != TSN_SUCCESS)
			{
				__DLME_join_response(TSN_AD_JOIN_ACK_AUTHENTICATION_Failed, 0);
				dmap->State = DMAP_STATE_end;
				return TSN_FALSE;
			}
			else
			{
				tsn_boolean_e AllocateShortAddrResult;
				AllocateShortAddrResult = TSN_AllocateShortAddr(ind->PhyAddr, ind->SecMaterial);
				if (AllocateShortAddrResult != TSN_SUCCESS)
				{
					DLME_join_response(TSN_AD_JOIN_ACK_SYSTEM_ERROR, 0);
					dmap->TSN_FALSE = DMAP_STATE_end;
					return TSN_FALSE;
				}
				dmap->State = DMAP_STATE_operation;
			}
			}
			break;
		case DMAP_STATE_join_allocatingShortAddress:
			if (trigger != DMAP_TRIGGER_T1_addr)
				return TSN_FALSE;
			{
				tsn_boolean_e AllocateShortAddrResult;
				short_addr_u Addr;
				AllocateShortAddrResult = TSN_AllocateShortAddr(&Addr);
				if (AllocateShortAddrResult != TSN_SUCCESS)
				{
					DLME_join_response(TSN_AD_JOIN_ACK_NETWORK_SCALE_ERROR, 0);
					dmap->TSN_FALSE = DMAP_STATE_end;
					return TSN_FALSE;
				}
			}
			__DLME_join_response(TSN_AD_JOIN_ACK_SUCCESS, tsn_get_addr(&Addr));
			dmap->State = DMAP_STATE_operation;
			break;
		case DMAP_STATE_operation:
			if (TRUE == IsHostComputerConfigureDone())
			{
				if (ResAllocAlgrithm(SuperframeList.LinkList) == SUCCESS)
				{
					req.Handle = dmap->Handle;
					req.DstAddr = ShortAddr;
					req.AttributeOption = DLME_information_set_option_UPDATE;
					req.AttributeID = 131;
					req.MemberID = 12;
					req.FirstStoreIndex = ;
					req.Count = 1;
					req.AttributeValue = ALLOCATION;
					DLME_information_set_request(&req);
					req.AttributeOption = DLME_information_set_option_ADD;
					req.AttributeID = 128;
					req.MemberID = ;
					req.FirstStoreIndex = ;
					req.Count = 1;
					req.AttributeValue = ;
					DLME_information_set_request(&req);
					dmap->State = DMAP_STATE_resource_allocation;
					break;
				}
			}
			return FALSE;
		case DMAP_STATE_resource_allocation:
			/* DLME-INFO-SET.confirm() */
			if (AttributeID == 128)
			{
				req.Handle = dmap->Handle;
				req.DstAddr = ShortAddr;
				req.AttributeOption = DLME_information_set_option_ADD;
				req.AttributeID = 129;
				req.MemberID = ;
				req.FirstStoreIndex = ;
				req.Count = 1;
				req.AttributeValue = ;
				DLME_information_set_request(&req);
			}
			else if (AttributeID == 129)
			{
				if (IsAllResAllocateDone() == TRUE)
				{
					req.Handle = dmap->Handle;
					req.DstAddr = ShortAddr;
					req.AttributeOption = 2;
					req.AttributeID = 131;
					req.MemberID = 12;
					req.FirstStoreIndex = ;
					req.Count = 1;
					req.AttributeValue = OPERATION;
					DLME_information_set_request(&req);
				}
				break;
			}
			else if (AttributeID == 131)
			{
				dmap->State = DMAP_STATE_operation;
				break;
			}
			else
			{
				return FALSE;
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

