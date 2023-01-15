/******************************************************************************
 *  Author:    980680431@qq.com
 *  License:  GPLv3
 *  Copyright: 980680431@qq.com 
 ******************************************************************************/
#include "tsn_private.h"

#define SERVER_PORT 8765

static tsn_connection_s tsn_udp_server = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT,
  .sent       = 0,
};

static tsn_err_e tsn_server_init(
  void (*tsn_read)(tsn_event_s *ev), 
  void (*tsn_send)(tsn_event_s *ev))
{
  return __tsn_server_init(&tsn_udp_server, tsn_read, tsn_send);
}

static void tsn_server_free(void)
{
  return __tsn_server_free(&tsn_udp_server);
}

static void __read_udp_msg(tsn_event_s *ev)
{
  ssize_t n;
  tsn_msg_s *m;
  tsn_connection_s *c;

  c = (tsn_connection_s *)ev->data;
  m = tsn_create_msg(c, TSN_BUFF_LEN_MAX);
  if (m == NULL)
  {
    TSN_error("Failed to tsn_create_msg.\n");
    return;
  }

  n = tsn_unix_recv(c, m);
  if (n < 0)
  {
    tsn_free_msg(m);
    return;
  }

  memcpy(&m->from, &c->client, sizeof(m->from));
  m->b.len = n;
}

static void __send_udp_msg(tsn_event_s *ev)
{
  tsn_connection_s *c;

  c = (tsn_connection_s *)ev->data;
  while (!list_empty(&ev->msgs))
  {
    tsn_msg_s *m;
    m = list_first_entry(&ev->msgs,tsn_msg_s,link);
    tsn_assert(m != NULL);
    if (tsn_unix_sendto(c, m) < 0)
    {
      TSN_debug("Failed to send udp message.\n");
      return;  
    }
    list_del(&m->link);
  }
}

int main(int argc, char **argv)
{
  int ret = EXIT_FAILURE;
  tsn_err_e r;

  r = wia_epoll_init();
  if (r != TSN_err_none)
  {
    TSN_error("wia_epoll_init failed.\n");
    goto clean0;
  }

  r = tsn_server_init(__read_udp_msg, __send_udp_msg);
  if (r != TSN_err_none)
  {
    TSN_error("tsn_server_init failed.\n");
    goto clean1;
  }

  while (sysCfg.run)
  {
    r = wia_epoll_process_events(TSN_TIMER_INFINITE, 0);
    if (r != TSN_err_none)
    {
      TSN_warn("Failed to wia_epoll_process_events().\n");
      TSN_error("(%d): %m.\n", errno, errno);
    }
  }
  
  ret = EXIT_SUCCESS;
  tsn_server_free();

clean1:
  wia_epoll_free();
clean0:
  return ret;
}
