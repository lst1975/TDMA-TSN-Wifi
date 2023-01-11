#include "tsn_private.h"

tsn_err_e
TSN_device_find_ByLongAddr(tsn_device_s **_dev, 
  unsigned int network, Unsigned64 PhyAddr)
{
  int i;
  tsn_err_e r;
  tsn_network_s *net;
  tsn_device_s *dev;
  
  r = tsn_system_get_network(&net, network);
  if (r != TSN_err_none)
    return r;

  for (i=0;i<net->ShortAddr;i++)
  {
    dev = net->Devices[i];
    if (dev != NULL && dev->LongAddress == PhyAddr)
    {
      *_dev = dev;
      return TSN_err_none;
    }
  }

  return -TSN_err_existed;
}

tsn_err_e
TSN_device_find_ByShortAddr(tsn_device_s **_dev, 
  unsigned int network, Unsigned16 ShortAddress)
{
  tsn_err_e r;
  tsn_network_s *net;
  
  r = tsn_system_get_network(&net, network);
  if (r != TSN_err_none)
    return r;

  if (ShortAddress >= TSN_ShorAddress_MAX)
    return -TSN_err_exceeded;

  *_dev = net->Devices[ShortAddress];
  return TSN_err_none;
}

void
TSN_device_init(tsn_device_s *dev,
  Unsigned64 LongAddress, 
  Unsigned8 AdID, 
  Unsigned8 DeviceState, 
  Unsigned16 DeviceShortAddress)
{
  dev->Version = 2022;
  dev->LongAddress = LongAddress;
  dev->NumberOfSuperframeUAO = 1;
  dev->NumberOfConfiguredUAO = 1;
  dev->TransmitDelay = 1200; /* in us */
  dev->TimeValue = tsn_system_time(); /* in us */
  dev->DeviceShortAddress = DeviceShortAddress;
  dev->AggregationSupportFlag = TSN_FALSE;
  dev->ProbeTime = 2; /*in default superframe*/
  dev->RedundantDeviceFlag = TSN_FALSE;
  dev->AccessDeviceID = AdID;
  dev->DeviceState = DeviceState;
  dev->PowerSupplyStatus = DMAP_mib_id_device_PowerSupplyStatus_Fixed;
  dev->MachineState = DMAP_STATE_init;
  dev->SequenceNumber = 0;
}

tsn_err_e
TSN_device_create(tsn_device_s **_dev,
  unsigned int network, Unsigned64 LongAddress, 
  Unsigned8 AdID, Unsigned8 DeviceState, 
  Unsigned16 DeviceShortAddress)
{
  tsn_err_e r;
  tsn_device_s *dev;
  tsn_network_s *net;

  r = TSN_device_find_ByLongAddr(&dev, network, LongAddress);
  if (r == TSN_err_none)
  {
    if (dev->DeviceShortAddress == DeviceShortAddress)
    {
      *_dev = dev;
      return r;
    }
    return -TSN_err_existed;
  }

  r = tsn_system_get_network(&net, network);
  if (r != TSN_err_none)
    return r;
  if (net->Devices[DeviceShortAddress])
    return -TSN_err_existed;
    
  dev = (tsn_device_s *)malloc(sizeof(*dev));
  if (dev == NULL)
  {
    TSN_error("Failed to malloc tsn_device_s.\n");    
    return -TSN_err_nomem;
  }
  TSN_device_init(dev, LongAddress, AdID, DeviceState, 
          DeviceShortAddress);
  net->Devices[DeviceShortAddress] = dev;
  *_dev = dev;
  return TSN_err_none;
}

