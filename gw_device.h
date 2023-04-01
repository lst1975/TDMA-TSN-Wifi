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
#ifndef __GW_DEVICE_H__
#define __GW_DEVICE_H__

struct tsn_device{
  Unsigned64 LongAddress;
  TimeData   TimeValue;
  Unsigned16 Version;
  Unsigned16 NumberOfSuperframeUAO;
  Unsigned16 NumberOfConfiguredUAO;
  Unsigned16 TransmitDelay;
  Unsigned16 DeviceShortAddress;
  Unsigned8  AggregationSupportFlag;
  Unsigned8  ProbeTime;
  Unsigned8  RedundantDeviceFlag;
  Unsigned8  AccessDeviceID;
  Unsigned8  DeviceState;
  Unsigned8  PowerSupplyStatus;
  union {
    void *AdDevice;
    tsn_sockaddr_s *SockAddr;
  };
  list_head_s link;
  list_head_s all;
  Unsigned64 SequenceNumber;
  Unsigned8  MachineState;
  Unsigned8  Network;
  Unsigned16 Flags;
};
typedef struct tsn_device tsn_device_s;

tsn_err_e
TSN_device_find_ByLongAddr(
  tsn_device_s **_dev, 
  unsigned int NetworkID, 
  Unsigned64 PhyAddr);

tsn_err_e
TSN_device_find_ByShortAddr(
  tsn_device_s **_dev, 
  unsigned int NetworkID, 
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
  unsigned int NetworkID, 
  Unsigned64 LongAddress, 
  Unsigned8 AdID, 
  Unsigned8 DeviceState, 
  Unsigned16 DeviceShortAddress);

tsn_err_e
TSN_device_destroy(tsn_device_s *dev);

#endif
