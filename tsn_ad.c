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

tsn_device_s *
tsn_network_ad_find_by_PhyAddr(Unsigned8 NetworkID, Unsigned64 PhyAddr)
{
  tsn_err_e r;
  tsn_device_s *dev;
  tsn_network_s *net;

  r = tsn_system_get_network(&net, NetworkID);
  if (r != TSN_err_none)
    return NULL;

  list_for_each_entry(dev,tsn_device_s,&net->Ads,link)
  {
    if (dev->LongAddress == PhyAddr)
      return dev;
  }

  return NULL;
}

int
tsn_network_id_by_Sockaddr(tsn_sockaddr_s *s)
{
  int i;

  for (i=0;i<TSN_NetworkID_MAX;i++)
  {
    int j;
    tsn_network_s *n;

    n = &sysCfg.network[i];
    for (j=0;j<TSN_ADID_MAX;j++)
    {
      tsn_sockaddr_s *a = &n->ads[i];
      if (tsn_sockaddr_isequal(a, (struct sockaddr *)s))
        return i;
    }
  }
  return TSN_NetworkID_MAX;
}

tsn_sockaddr_s *
tsn_system_cfg_ad_find(unsigned int NetworkID, tsn_sockaddr_s *s)
{
  int i;
  tsn_network_s *net;

  if (TSN_err_none != tsn_system_get_network(&net, NetworkID))
    return NULL;

  for (i=0;i<TSN_ADID_MAX;i++)
  {
    tsn_sockaddr_s *a = &net->ads[i];
    if (tsn_sockaddr_isequal(a, (struct sockaddr *)s))
      return a;
  }
  
  return NULL;
}

tsn_boolean_e
tsn_system_cfg_ad_add(unsigned int NetworkID, tsn_sockaddr_s *s)
{
  int i;
  tsn_network_s *net;
  tsn_sockaddr_s *a;

  if (TSN_err_none != tsn_system_get_network(&net, NetworkID))
    return TSN_FALSE;

  a = tsn_system_cfg_ad_find(NetworkID, s);
  if (a != NULL)
    return TSN_TRUE;
  
  for (i=0;i<TSN_ADID_MAX;i++)
  {
    tsn_sockaddr_s *a = &net->ads[i];
    if (a->slen)
      continue;
    *a = *s;
    a->slen = tsn_sockaddr_salen(a);
    net->Active = TSN_TRUE;
    return TSN_TRUE;
  }
  
  return TSN_FALSE;
}

tsn_boolean_e
tsn_system_cfg_ad_del(unsigned int NetworkID, tsn_sockaddr_s *s)
{
  tsn_sockaddr_s *a = tsn_system_cfg_ad_find(NetworkID, s);
  if (a == NULL)
    return TSN_FALSE;
  
  memset(a, 0, sizeof(*a));
  return TSN_TRUE;
}
