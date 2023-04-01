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

tsn_err_e
TSN_device_find_ByLongAddr(tsn_device_s **_dev, 
  unsigned int NetworkID, Unsigned64 PhyAddr)
{
  tsn_err_e r;
  tsn_network_s *net;
  tsn_device_s *dev;
  
  r = tsn_network_find(&net, NetworkID);
  if (r != TSN_err_none)
    return r;

  list_for_each_entry(dev,tsn_device_s,&net->All,all)
  {
    if (dev->LongAddress == PhyAddr)
    {
      *_dev = dev;
      return TSN_err_none;
    }
  }

  return -TSN_err_existed;
}

tsn_err_e
TSN_device_find_ByShortAddr(tsn_device_s **_dev, 
  unsigned int NetworkID, Unsigned16 ShortAddress)
{
  tsn_err_e r;
  tsn_network_s *net;
  
  r = tsn_network_find(&net, NetworkID);
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
  dev->Version                = 2022;
  dev->LongAddress            = LongAddress;
  dev->NumberOfSuperframeUAO  = 1;
  dev->NumberOfConfiguredUAO  = 1;
  dev->TransmitDelay          = 1200; /* in us */
  dev->TimeValue              = tsn_system_time(); /* in us */
  dev->DeviceShortAddress     = DeviceShortAddress;
  dev->AggregationSupportFlag = TSN_FALSE;
  dev->ProbeTime              = 2; /*in default superframe*/
  dev->RedundantDeviceFlag    = TSN_FALSE;
  dev->AccessDeviceID         = AdID;
  dev->DeviceState            = DeviceState;
  dev->PowerSupplyStatus      = DMAP_mib_id_device_PowerSupplyStatus_Fixed;
  dev->MachineState           = DMAP_STATE_init;
  dev->SequenceNumber         = 0;
  dev->Flags                  = 0;
  __list_poison_entry(&dev->link);
  __list_poison_entry(&dev->all);
}

tsn_err_e
TSN_device_create(tsn_device_s **_dev,
  unsigned int NetworkID, Unsigned64 LongAddress, 
  Unsigned8 AdID, Unsigned8 DeviceState, 
  Unsigned16 DeviceShortAddress)
{
  tsn_err_e r;
  tsn_device_s *dev;
  tsn_network_s *net;

  r = TSN_device_find_ByLongAddr(&dev, NetworkID, LongAddress);
  if (r == TSN_err_none)
  {
    if (dev->DeviceShortAddress == DeviceShortAddress)
    {
      *_dev = dev;
      return r;
    }
    return -TSN_err_existed;
  }

  r = tsn_network_find(&net, NetworkID);
  if (r != TSN_err_none)
    return r;
  if (net->Devices[DeviceShortAddress])
    return -TSN_err_existed;
    
  dev = (tsn_device_s *)tsn_malloc(sizeof(*dev));
  if (dev == NULL)
  {
    TSN_error("Failed to malloc tsn_device_s.\n");    
    return -TSN_err_nomem;
  }
  
  TSN_device_init(
          dev, 
          LongAddress, 
          AdID, 
          DeviceState, 
          DeviceShortAddress);
  if (AdID != TSN_ADID_INVALID)
  {
    list_add_tail(&dev->link, &net->Ads);
  }
  else
  {
    list_add_tail(&dev->link, &net->Fds);
  }
  
  list_add_tail(&dev->all, &net->All);
  
  dev->Network = NetworkID;
  net->Devices[DeviceShortAddress] = dev;
  *_dev = dev;
  return TSN_err_none;
}

tsn_err_e
TSN_device_destroy(tsn_device_s *dev)
{
  tsn_err_e r;
  tsn_network_s *net;
  
  r = tsn_network_find(&net, dev->Network);
  if (r != TSN_err_none)
    return r;

  tsn_assert(dev->DeviceShortAddress != TSN_ShorAddress_INVALID);
  
  list_del(&dev->link);
  list_del(&dev->all);

  net->Devices[dev->DeviceShortAddress] = NULL;
  TSN_FreeShortAddr(&dev->DeviceShortAddress);
  tsn_free(dev);
  
  return TSN_err_none;
}

