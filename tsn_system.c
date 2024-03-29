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

tsn_sys_config_s sysCfg = {
  .run          = TSN_TRUE,
  .useShortAddr = TSN_TRUE,
  .dumpPacket   = TSN_TRUE,
  .logDebug     = TSN_TRUE,
  .logEvent     = TSN_TRUE,
  .logWarn      = TSN_TRUE,
  .logError     = TSN_TRUE,
  .Initialized  = TSN_FALSE,
  .State        = DMAP_STATE_init,
  .fcsLength    = 2,
  .devType      = DMAP_mib_device_gw,
  .Connections  = 32,
  .Events       = 32,
  .config.PriGwFailureTime                 = 10,   /* in us */
  .config.AddressTypeFlag                  = 0, 
  .config.MaxPayloadLength                 = 1000, /* in BYTES */
  .config.NACKCount                        = 1, 
  .config.NetworkID                        = 0, 
  .config.BitMap                           = { 0, 0, 0 }, 
  .config.DeviceStatusReportCycle          = 10    /* in ms */, 
  .config.LossConnectDuration              = 30    /* in ms */, 
  .config.KeyUpdateDuration                = 24    /* in hours */, 
  .config.TimeSlotDuration                 = 200   /* in us */, 
  .config.TwoWayTimeSynchronization        = TSN_FALSE, 
  .config.TwoWayTimeSynchronizationTimeout = TSN_FALSE, 
  .config.AccessDeviceTeamNumber           = 1, 
  .config.ExpectedLossRate                 = { .SN=0, .E=0, .F=0 }, 
  .config.LossRate                         = { .SN=0, .E=0, .F=0 }, 
  .config.MaxRetry                         = 3, 
  .config.SecurityLevel                    = DMAP_mib_id_static_SecurityLevel_Authentication, 
  .config.AttackStatisticsDuration         = 60    /* in minutes */,  
  .config.MaxKeyAttackedNumber             = 5, 
  .config.AlarmReportDuration              = 1     /* in ms */,   
  .config.ChannelNumber                    = 1, 
  .config.AggregationEnableFlag            = TSN_FALSE, 
  .Nets                                    = LIST_HEAD_INIT(sysCfg.Nets)
};

__TSN_INTERFACE TimeData tsn_system_time(void)
{
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0)
    return 0;

  return tv.tv_sec * 1000000 + tv.tv_usec;
}

tsn_boolean_e tsn_system_init(void)
{
  int i;
  tsn_network_s *s;

  if (tsn_crc_init() != TSN_TRUE)
    goto error;

  for (i=0;i<TSN_NetworkID_MAX;i++)
  {
    tsn_err_e r;
    r = tsn_network_init(&sysCfg.network[i], i);
    if (r != TSN_err_none)
      goto error;
  }

  if (tsn_network_actiate(0) != TSN_err_none)
    goto error;
  
  return TSN_TRUE;

error:
  return TSN_FALSE;
}
