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

void tsn_send_udp_msg(tsn_msg_s *m)
{
  tsn_connection_s *c;
  c = (tsn_connection_s *)m->priv;
  list_add_tail(&m->link, &c->write.msgs);
}

static void __read_udp_msg(tsn_event_s *ev)
{
  ssize_t n;
  tsn_err_e r;
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

  tsn_memcpy(&m->from, &c->client, sizeof(m->from));
  m->b.len = n;

  r = c->process(m);
  if (r != TSN_err_none)
  {
    tsn_free_msg(m);
    TSN_error("Failed to process message.\n");
  }
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
    list_del(&m->link);
    if (tsn_unix_sendto(c, m) < 0)
    {
      TSN_debug("Failed to send udp message.\n");
      tsn_free_msg(m);
      return;  
    }
  }
}

static tsn_err_e __tsn_server_init(tsn_connection_s *c)
{
  int ret;
  tsn_err_e err;
  tsn_event_s *ev;
  
  struct sockaddr_in *server_addr = &c->addr.u.addr4;
  struct sockaddr_in *client_addr = &c->client.u.addr4;

  TSN_event("try to init udp server!\n");
  
  tsn_memset(server_addr, 0, sizeof(*server_addr));
  c->addr.sa   = (struct sockaddr *)server_addr;
  c->addr.slen = sizeof(*server_addr);
  server_addr->sin_family = c->family;
  server_addr->sin_addr.s_addr = TSN_htonl(INADDR_ANY); 
  server_addr->sin_port = TSN_htons(c->port);  

  tsn_memset(client_addr, 0, sizeof(*client_addr));
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

  tsn_event_init(&c->read, c, c->tsn_read);
  tsn_event_init(&c->write, c, c->tsn_send);

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

static void __tsn_server_free(tsn_connection_s *c)
{
  tsn_event_s *ev = &c->write;
  while (!list_empty(&ev->msgs))
  {
    tsn_msg_s *m;
    m = list_first_entry(&ev->msgs,tsn_msg_s,link);
    tsn_assert(m != NULL);
    list_del(&m->link);
    tsn_free_msg(m);
  }
  
  wia_epoll_del_connection(c, 0);
  
  if (c->fd != -1)
  {
    if (close(c->fd) == -1)
      TSN_warn("Failed to close fd:%d", c->fd);
    else
      c->fd = -1;
  }
}

tsn_err_e tsn_server_init(tsn_connection_s *c)
{
  if (c->tsn_read == NULL)
    c->tsn_read = __read_udp_msg;
  if (c->tsn_send == NULL)
    c->tsn_send = __send_udp_msg;
  return __tsn_server_init(c);
}

void tsn_server_free(tsn_connection_s *c)
{
  return __tsn_server_free(c);
}

