/******************************************************************************
 *  Author:    980680431@qq.com
 *  Liscence:  GPLV3
 *  Copyright: 980680431@qq.com
 ******************************************************************************/

#include "tsn_private.h"

tsn_boolean_e tsn_crc_init(void)
{
  return TSN_TRUE;
}

uint64_t tsn_get_fcs(char *p)
{
  if (sysCfg.fcsLength==1)
    return p[0];
  else if (sysCfg.fcsLength==2)
    return TSN_ntohs(*(uint16_t*)p);
  else if (sysCfg.fcsLength==4)
    return TSN_ntohl(*(uint32_t*)p);
  else 
    return 0;
}

int tsn_set_fcs(char *p, uint64_t fcs)
{
  if (sysCfg.fcsLength==1)
  {
    p[0] = (uint8_t)fcs;
    return 1;
  }
  else if (sysCfg.fcsLength==2)
  {
    *(uint16_t*)p = TSN_htons((uint16_t)fcs);
    return 2;
  }
  else if (sysCfg.fcsLength==4)
  {
    *(uint32_t*)p = TSN_htonl((uint32_t)fcs);
    return 4;
  }
  else 
    return 0;
}

uint64_t
tsn_calc_fcs(char *p, int length)
{
  return crc32(p, length);
}

tsn_boolean_e
tsn_check_fcs(char *p, int length, uint64_t fcs)
{
  return crc32(p, length) == fcs;
}

