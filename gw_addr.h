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
#ifndef __GW_ADDR_H_
#define __GW_ADDR_H_

#define ___TSN_htonll(x) x
#define TSN_LongAddr_VendorID_Mask         ___TSN_htonll(0xffffff0000000000)
#define TSN_LongAddr_DeviceType_Mask       ___TSN_htonll(0x000000ffff000000)
  #define TSN_LongAddr_DeviceType_GateWay  ___TSN_htonll(0x0000000000000000)
  #define TSN_LongAddr_DeviceType_Access   ___TSN_htonll(0x0000000001000000)
  #define TSN_LongAddr_DeviceType_Field    ___TSN_htonll(0x0000000002000000)
  #define TSN_LongAddr_DeviceType_HandHeld ___TSN_htonll(0x0000000003000000)
#define TSN_LongAddr_DeviceID_Mask         ___TSN_htonll(0x0000000000ffffff)

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
  return (longAddr & TSN_LongAddr_DeviceType_Mask) == TSN_LongAddr_DeviceType_HandHeld;
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
tsn_print_longaddr(Unsigned64 __longAddr)
{
  Unsigned64 longAddr = TSN_htonll(__longAddr);
  Unsigned8 *a = (Unsigned8 *)&longAddr;
  printf("\t64 bits long addr: 0x%X%X%X%X%X%X%X%X\n", 
    (tsn_uint_t)a[0],(tsn_uint_t)a[1],(tsn_uint_t)a[2],
    (tsn_uint_t)a[3],(tsn_uint_t)a[4],(tsn_uint_t)a[5],
    (tsn_uint_t)a[6],(tsn_uint_t)a[7]);
  printf("\t\tVendorID: %X%X%X\n", 
    (tsn_uint_t)a[0],(tsn_uint_t)a[1],(tsn_uint_t)a[2]);
  printf("\t\tDeviceType: %s\n", 
    tsn_is_gateway(__longAddr) ? "Gateway" : 
    tsn_is_Access(__longAddr) ? "Access" :
    tsn_is_Field(__longAddr) ? "Field" :
    tsn_is_HandHeld(__longAddr) ? "Held" : "Unknown");
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
