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
tsn_network_find(tsn_network_s **net, 
  unsigned int network)
{
  tsn_network_s *n;
  if (network >= TSN_NetworkID_MAX)
    return -TSN_err_exceeded;

  n = &sysCfg.network[network];
  if (n->Active != TSN_TRUE)
    return -TSN_err_existed;

  *net = n;
  return TSN_err_none;
}

tsn_err_e
tsn_network_init(tsn_network_s *net, int index)
{
  int i;

  net->Index = index;
  net->AcceptedPhyAddr = NULL;
  net->AdID            = 0;
  net->Active          = TSN_FALSE;
  net->FDNumber        = 0;
  net->DevCount        = 0;
  tsn_memset(net->Devices, 0, sizeof(net->Devices));

  INIT_LIST_HEAD(&net->Ads);
  INIT_LIST_HEAD(&net->Fds);
  INIT_LIST_HEAD(&net->All);
  __list_poison_entry(&net->link);
  
  for (i=0;i<TSN_ADID_MAX;i++)
  {
    tsn_sockaddr_s *a = &net->ads[i];
    tsn_memset(a, 0, sizeof(*a));
    a->sa = (struct sockaddr *)a;
  }
  
  {
    tsn_sockaddr_s *a;
    struct sockaddr_in *a4;

    a = &net->ads[0];
    a->sa   = (struct sockaddr *)a;
    a->slen = sizeof(a->u.addr4);

    a4 = &a->u.addr4;
    a4->sin_family      = AF_INET;
    a4->sin_port        = TSN_htons(8848);
    a4->sin_addr.s_addr = inet_addr("10.0.2.15");
  }
  
  return TSN_err_none;
}

tsn_err_e
tsn_network_actiate(unsigned int network)
{
  tsn_network_s *n;

  if (tsn_network_find(&n, network) != TSN_err_none)
    return -TSN_err_existed;

  n->Active = TSN_TRUE;
  list_add_tail(&n->link, &sysCfg.Nets);
  
  return TSN_err_none;
}

tsn_err_e
tsn_network_release(unsigned int network)
{
  tsn_network_s *n;

  if (tsn_network_find(&n, network) != TSN_err_none)
    return -TSN_err_existed;

  n->Active = TSN_FALSE;
  list_del(&n->link);
  
  return TSN_err_none;
}

