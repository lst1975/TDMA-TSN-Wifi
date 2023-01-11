#ifndef __GW_ADDR_H_
#define __GW_ADDR_H_

#define TSN_LongAddr_VendorID_Mask   TSN_htonll(0xffffff0000000000)
#define TSN_LongAddr_DeviceType_Mask TSN_htonll(0x000000ffff000000)
	#define TSN_LongAddr_DeviceType_GateWay  TSN_htonll(0x0000000000000000)
	#define TSN_LongAddr_DeviceType_Access   TSN_htonll(0x0000000001000000)
	#define TSN_LongAddr_DeviceType_Field	   TSN_htonll(0x0000000002000000)
	#define TSN_LongAddr_DeviceType_HandHeld TSN_htonll(0x0000000003000000)
#define TSN_LongAddr_DeviceID_Mask   TSN_htonll(0x0000000000ffffff)

static inline tsn_boolean_e
tsn_is_gateway(Unsigned64 longAddr)
{
	return (longAddr & TSN_LongAddr_DeviceType_Mask) == TSN_LongAddr_DeviceType_GateWay;
}
static inline tsn_boolean_e
tsn_is_Access(Unsigned64 longAddr)
{
	return (longAddr & TSN_LongAddr_DeviceType_Mask) == TSN_LongAddr_DeviceType_Access;
}
static inline tsn_boolean_e
tsn_is_Field(Unsigned64 longAddr)
{
	return (longAddr & TSN_LongAddr_DeviceType_Mask) == TSN_LongAddr_DeviceType_Field;
}
static inline tsn_boolean_e
tsn_is_HandHeld(Unsigned64 longAddr)
{
	return (longAddr & TSN_LongAddr_DeviceType_Mask) | TSN_LongAddr_DeviceType_HandHeld;
}
static inline Unsigned64
tsn_get_deviceType(Unsigned64 longAddr)
{
	return (longAddr & TSN_LongAddr_DeviceType_Mask) >> 24;
}
static inline Unsigned64
tsn_set_deviceType(Unsigned64 longAddr, Unsigned64 type)
{
	return (longAddr & ~TSN_LongAddr_DeviceType_Mask) | type;
}
static inline Unsigned64
tsn_set_gateway(Unsigned64 longAddr)
{
	tsn_set_deviceType(longAddr, TSN_LongAddr_DeviceType_GateWay);
}
static inline Unsigned64
tsn_set_Access(Unsigned64 longAddr)
{
	tsn_set_deviceType(longAddr, TSN_LongAddr_DeviceType_Access);
}
static inline Unsigned64
tsn_set_Field(Unsigned64 longAddr)
{
	tsn_set_deviceType(longAddr, TSN_LongAddr_DeviceType_Field);
}
static inline Unsigned64
tsn_set_HandHeld(Unsigned64 longAddr)
{
	tsn_set_deviceType(longAddr, TSN_LongAddr_DeviceType_HandHeld);
}
static inline Unsigned64
tsn_get_deviceID(Unsigned64 longAddr)
{
	return (longAddr & TSN_LongAddr_DeviceID_Mask);
}
static inline Unsigned64
tsn_set_deviceID(Unsigned64 longAddr, Unsigned64 ID)
{
	return (longAddr & ~TSN_LongAddr_DeviceID_Mask) | ID;
}

static inline void
tsn_print_longaddr(Unsigned64 longAddr)
{
  Unsigned8 *a = (Unsigned8 *)longAddr;
  printf("\t64 bits long addr: 0x%X%X%X%X%X%X%X%X\n", 
    (tsn_uint_t)a[0],(tsn_uint_t)a[1],(tsn_uint_t)a[2],
    (tsn_uint_t)a[3],(tsn_uint_t)a[4],(tsn_uint_t)a[5],
    (tsn_uint_t)a[6],(tsn_uint_t)a[7]);
  printf("\t\tVendorID: %X%X%X\n", 
    (tsn_uint_t)a[0],(tsn_uint_t)a[1],(tsn_uint_t)a[2]);
  printf("\t\tDeviceType: %s\n", 
    tsn_is_gateway(longAddr) ? "Gateway" : 
    tsn_is_Access(longAddr) ? "Access" :
    tsn_is_Field(longAddr) ? "Field" :
    tsn_is_HandHeld(longAddr) ? "Held" : "Unknown");
  printf("\t\tDeviceID: %X%X%X\n", 
    (tsn_uint_t)a[5],(tsn_uint_t)a[6],(tsn_uint_t)a[7]);
}

union _tsnAddr{
	Unsigned8 AddrType;
	Unsigned8 AddrU8;
	Unsigned16 AddrU16;
	Unsigned64 AddrU64;
};
typedef union _tsnAddr tsn_addr_u;

static inline Unsigned16
tsn_get_short_addr(tsn_addr_u *Addr)
{
	if (Addr->AddrType == DMAP_mib_id_static_AddressTypeFlag_u16) 
		return Addr->AddrU16;
	else if (Addr->AddrType == DMAP_mib_id_static_AddressTypeFlag_u8) 
		return Addr->AddrU8;
	else 
		return 0;
}

static inline void
tsn_print_addr(tsn_addr_u *Addr)
{
  switch (Addr->AddrType)
  {
    case DMAP_mib_id_static_AddressTypeFlag_u8:
      printf("8 bits short addr: %u\n", Addr->AddrU8);
      break;
    case DMAP_mib_id_static_AddressTypeFlag_u16:
      printf("16 bits short addr: %u\n", Addr->AddrU16);
      break;
    case DMAP_mib_id_static_AddressTypeFlag_u64:
      tsn_print_longaddr(Addr->AddrU64);
      break;
  }
}

static inline int 
tsn_get_addr_length(int addType)
{
  switch (addType)
  {
    case DMAP_mib_id_static_AddressTypeFlag_u8:
      return 1;
    case DMAP_mib_id_static_AddressTypeFlag_u16:
      return 2;
    case DMAP_mib_id_static_AddressTypeFlag_u64:
      return 8;
    default:
      return 0;
  }
}

#endif
