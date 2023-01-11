#include "tsn_private.h"

void tsn_send_udp_msg(tsn_msg_s *m)
{
  tsn_connection_s *c;
  c = (tsn_connection_s *)m->priv;
  list_add_tail(&m->link, &c->write.msgs);
}

static inline void 
tsn_event_init(tsn_event_s *ev, tsn_connection_s *c, 
  void (*handler)(tsn_event_s *ev))
{
  ev->instance    = 1;
  ev->active      = TSN_FALSE;
  ev->available   = TSN_FALSE;
  ev->ready       = TSN_FALSE;
  ev->pending_eof = TSN_FALSE;
  ev->accept      = TSN_FALSE;
  ev->eof         = TSN_FALSE;
  ev->error       = TSN_FALSE;
  ev->data        = c;
  ev->handler     = handler;
  INIT_LIST_HEAD(&ev->msgs);
}

tsn_err_e __tsn_server_init(
  tsn_connection_s *c,
  void (*tsn_read)(tsn_event_s *ev), 
  void (*tsn_send)(tsn_event_s *ev))
{
  int ret;
  tsn_err_e err;
  tsn_event_s *ev;
  
  struct sockaddr_in *server_addr = &c->addr.u.addr4;
  struct sockaddr_in *client_addr = &c->client.u.addr4;

  TSN_event("try to init udp server!\n");
  
  memset(server_addr, 0, sizeof(*server_addr));
  c->addr.sa   = (struct sockaddr *)server_addr;
  c->addr.slen = sizeof(*server_addr);
  server_addr->sin_family = c->family;
  server_addr->sin_addr.s_addr = TSN_htonl(INADDR_ANY); 
  server_addr->sin_port = TSN_htons(c->port);  

  memset(client_addr, 0, sizeof(*client_addr));
  c->client.sa   = (struct sockaddr *)client_addr;
  c->client.slen = sizeof(*client_addr);

  /* AF_INET:IPV4;SOCK_DGRAM:UDP */
  c->fd = socket(c->family, c->type, c->protocol); 
  if (c->fd < 0)
  {
    TSN_error("create socket fail!\n");
    err = -TSN_err_system;
    goto clean0;
  }

  ret = bind(c->fd, c->addr.sa, c->addr.slen);
  if (ret < 0)
  {
    TSN_error("socket bind fail!\n");
    err = -TSN_err_system;
    goto clean1;
  }

  ret = tsn_set_nonblocking(c->fd, 1);
  if (ret < 0)
  {
    TSN_error("socket bind fail!\n");
    err = -TSN_err_system;
    goto clean1;
  }

  tsn_print_sockaddr(c->addr.sa);

  tsn_event_init(&c->read, c, tsn_read);
  tsn_event_init(&c->write, c, tsn_send);

  err = wia_epoll_add_connection(c);
  if (err != TSN_err_none)
  {
    TSN_error("wia epoll add connection fail!\n");
    goto clean1;
  }
  
  return TSN_err_none;

clean1:
  close(c->fd);
  c->fd = -1;
clean0:
  TSN_error("(%d): %m.\n", errno, errno);
  return err;
}

void __tsn_server_free(tsn_connection_s *c)
{
  wia_epoll_del_connection(c, 0);
  if (c->fd != -1)
  {
    if (close(c->fd) == -1)
      TSN_warn("Failed to close fd:%d", c->fd);
    else
      c->fd = -1;
  }
}
