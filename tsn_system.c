#include "tsn_private.h"

tsn_sys_config_s sysCfg = {
  .run          = TSN_TRUE,
  .useShortAddr = TSN_TRUE,
  .dumpPacket   = TSN_TRUE,
  .logDebug     = TSN_TRUE,
  .logEvent     = TSN_TRUE,
  .logWarn      = TSN_TRUE,
  .logError     = TSN_TRUE,
  .fcsLength    = 2,
  .devType      = DMAP_mib_device_gw,
  .Connections  = 32,
  .Events       = 32,
  .config.PriGwFailureTime                 = 10, /* in us */
  .config.AddressTypeFlag                  = 0, 
  .config.MaxPayloadLength                 = 1000, /* in BYTES */
  .config.NACKCount                        = 1, 
  .config.NetworkID                        = 0, 
  .config.BitMap                           = { 0, 0, 0 }, 
  .config.DeviceStatusReportCycle          = 10 /* in ms */, 
  .config.LossConnectDuration              = 30 /* in ms */, 
  .config.KeyUpdateDuration                = 24    /* in hours */, 
  .config.TimeSlotDuration                 = 200    /* in us */, 
  .config.TwoWayTimeSynchronization        = TSN_FALSE, 
  .config.TwoWayTimeSynchronizationTimeout = TSN_FALSE, 
  .config.AccessDeviceTeamNumber           = 1, 
  .config.ExpectedLossRate                 = { .SN=0, .E=0, .F=0 }, 
  .config.LossRate                         = { .SN=0, .E=0, .F=0 }, 
  .config.MaxRetry                         = 3, 
  .config.SecurityLevel                    = DMAP_mib_id_static_SecurityLevel_Authentication, 
  .config.AttackStatisticsDuration         = 60 /* in minutes */,  
  .config.MaxKeyAttackedNumber             = 5, 
  .config.AlarmReportDuration              = 1 /* in ms */,   
  .config.ChannelNumber                    = 1, 
  .config.AggregationEnableFlag            = TSN_FALSE, 
};

TimeData tsn_system_time(void)
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
    s = &sysCfg.network[i];
    s->AcceptedPhyAddr = NULL;
    s->ShortAddr = 0;
    s->AdID = 0;
    s->FDNumber = 0;
    memset(s->Devices, 0, sizeof(s->Devices));
  }
  return TSN_TRUE;

error:
  return TSN_FALSE;
}

tsn_err_e
tsn_system_get_network(tsn_network_s **net, unsigned int network)
{
  if (network >= TSN_NetworkID_MAX)
    return -TSN_err_exceeded;
  *net = &sysCfg.network[network];
  return TSN_err_none;
}
