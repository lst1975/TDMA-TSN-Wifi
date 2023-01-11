
tsn_boolean_e 
TSN_AllocateShortAddr(Unsigned64 LongAddr, short_addr_u *Addr)
{
	TSN_DMAP_mib_attribute_s *mib, *attr;
	
	mib = TSN_DMAP_mib_device_list_by_LongAddr(LongAddr);
	if (mib == NULL)
		return TSN_FAILURE;
	
	attr = &mib[DMAP_mib_id_device_DeviceShortAddress];
	Addr->AddrType = TSN_DMAP_mib_shortAddr_type();
	if (Addr->AddrType == DMAP_mib_id_static_AddressTypeFlag_u16) 
		Addr->AddrU16 = attr.Value.value_Unsigned16;
	else if (Addr->AddrType == DMAP_mib_id_static_AddressTypeFlag_u8) 
		Addr->AddrU8 = attr.Value.value_Unsigned8;
	else 
		return TSN_FAILURE;
	
	return TSN_SUCCESS;
}