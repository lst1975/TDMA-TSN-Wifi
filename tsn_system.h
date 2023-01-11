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
  uint64_t devType:3;
  uint64_t Connections:6;
  uint64_t Events:6;
  list_head_s posted_accept_events;
  list_head_s posted_events;
  
  tsn_network_s network[TSN_NetworkID_MAX];
  tsn_static_config_s config;
};
typedef struct tsn_sys_config tsn_sys_config_s;

struct tsn_msg {
  list_head_s link;
  tsn_buffer_s b;
  tsn_sockaddr_s from;
  void *priv;
};
typedef struct tsn_msg tsn_msg_s;

static inline tsn_msg_s *
tsn_create_msg(void *priv, int len)
{
  tsn_msg_s *m = (tsn_msg_s *)malloc(sizeof(*m)+len);
  if (m == NULL)
    return NULL;
  m->priv = priv;
  tsn_buffer_init(&m->b, (Unsigned8 *)(m+1), len);
  return m;
}

static inline void
tsn_free_msg(tsn_msg_s *m)
{
  free(m);
}

extern tsn_sys_config_s sysCfg;

TimeData tsn_system_time(void);
tsn_err_e
tsn_system_get_network(tsn_network_s **net, unsigned int network);

#endif

