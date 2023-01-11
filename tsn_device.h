#ifndef __TSN_DEVICE_H__
#define __TSN_DEVICE_H__

#define TSN_ShorAddress_MAX 1024
#define TSN_NetworkID_MAX 8

typedef struct tsn_network tsn_network_s;

struct tsn_network{
  uint64_t AdID:8;
  uint64_t ShortAddr:16;
  uint64_t FDNumber:24;
  uint64_t *AcceptedPhyAddr;
  tsn_device_s *Devices[TSN_ShorAddress_MAX];
};

tsn_err_e
TSN_device_find_ByLongAddr(
  tsn_device_s **_dev, 
  unsigned int network, 
  Unsigned64 PhyAddr);

tsn_err_e
TSN_device_find_ByShortAddr(
  tsn_device_s **_dev, 
  unsigned int network, 
  Unsigned16 ShortAddress);

void
TSN_device_init(
  tsn_device_s *dev,
  Unsigned64 LongAddress, 
  Unsigned8 AdID, 
  Unsigned8 DeviceState, 
  Unsigned16 DeviceShortAddress);

tsn_err_e
TSN_device_create(
  tsn_device_s **_dev,
  unsigned int network, 
  Unsigned64 LongAddress, 
  Unsigned8 AdID, 
  Unsigned8 DeviceState, 
  Unsigned16 DeviceShortAddress);

#endif
