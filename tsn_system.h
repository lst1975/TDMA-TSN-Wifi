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
#ifndef __TSN_SYSTEM_H__
#define __TSN_SYSTEM_H__

struct tsn_sys_config {
  uint64_t run: 1;
  uint64_t useShortAddr: 1;
  uint64_t dumpPacket: 1;
  uint64_t logDebug: 1;
  uint64_t logEvent: 1;
  uint64_t logWarn: 1;
  uint64_t logError: 1;
  uint64_t fcsLength: 3;
  uint64_t devType: 3;
  uint64_t Connections: 6;
  uint64_t Events: 6;
  uint64_t Initialized: 1;
  uint64_t State: 3;
  list_head_s posted_accept_events;
  list_head_s posted_events;
  
  tsn_network_s network[TSN_NetworkID_MAX];
  tsn_static_config_s config;
};
typedef struct tsn_sys_config tsn_sys_config_s;

#define TSN_HANDLE_MAX 256
#define TSN_HANDLE_INVALID ((uint16_t)(-1))

struct tsn_gw_dlpdu_normal{
  Unsigned8   ServID;
  Unsigned8   AdID;
  Unsigned64  ADAddr;
  Unsigned16  AttrLen;
  Unsigned8  *Attrs;
};
typedef struct tsn_gw_dlpdu_normal tsn_gw_dlpdu_normal_s;

typedef void (*tsn_msg_f)(tsn_msg_s *m);

struct tsn_msg {
  list_head_s link;
  list_head_s wait;
  tsn_buffer_s b;
  tsn_sockaddr_s from;
  void *priv;
  tsn_gw_dlpdu_normal_s _dlpdu;
  void *dlpdu;
  tsn_msg_f cb;
  TimeData stamp;
  uint16_t handle;
  uint8_t isInQ;
};
typedef struct tsn_msg tsn_msg_s;

static inline tsn_msg_s *
tsn_create_msg(void *priv, int len)
{
  tsn_msg_s *m = (tsn_msg_s *)malloc(sizeof(*m)+len);
  if (m == NULL)
    return NULL;
  m->priv   = priv;
  m->handle = TSN_HANDLE_INVALID;
  m->isInQ  = TSN_FALSE;
  m->stamp  = 0;
  m->cb     = NULL;
  m->freed  = TSN_FALSE;
  m->dlpdu  = &m->_dlpdu;
  tsn_buffer_init(&m->b, (Unsigned8 *)(m+1), len);
  return m;
}

static inline void
tsn_free_msg(tsn_msg_s *m)
{
  free(m);
}

static inline tsn_msg_s *
tsn_duplicate_msg(tsn_msg_s *_msg, int len)
{
  tsn_msg_s *msg;

  msg = tsn_create_msg(_msg->priv, len);
  if (msg == NULL)
    return NULL;
  msg->from = _msg->from;
  return msg;
}

extern tsn_sys_config_s sysCfg;

__TSN_INTERFACE TimeData tsn_system_time(void);
tsn_err_e tsn_system_get_network(tsn_network_s **net, unsigned int network);
void tsn_sockaddr_print(tsn_sockaddr_s *s, const char *head, const char *tail);
int tsn_sockaddr_salen(tsn_sockaddr_s *s);

#endif

