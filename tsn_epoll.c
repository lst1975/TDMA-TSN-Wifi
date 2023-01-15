/******************************************************************************
 *  Author:    980680431@qq.com
 *  License:  GPLV3
 *  Copyright: 980680431@qq.com 
 ******************************************************************************/
#include "tsn_private.h"

#if (TSN_HAVE_EPOLLRDHUP)
tsn_uint_t wia_use_epoll_rdhup;
static void wia_epoll_test_rdhup(tsn_sys_config_s *sys);
#endif

static tsn_int_t            wia_ep_handle   = -1;
static struct epoll_event  *wia_event_list  = NULL;
static tsn_uint_t           wia_nevents     = 0;
static tsn_uint_t           wia_event_flags = 0;

int
tsn_set_nonblocking(tsn_socket_t s, int nb)
{
  return ioctl(s, FIONBIO, &nb);
}

int
tsn_set_tcpnopush(tsn_socket_t s, int cork)
{
  return setsockopt(s, IPPROTO_TCP, TCP_CORK,
                    (const void *) &cork, sizeof(int));
}

static tsn_err_e __wia_epoll_init(tsn_sys_config_s *sys)
{
  if (wia_ep_handle == -1) 
  {
    wia_ep_handle = epoll_create(sys->Connections >> 1);

    if (wia_ep_handle == -1) 
    {
      TSN_error("epoll_create() failed.\n");
      TSN_error("(%d): %m.\n", errno, errno);
      return -TSN_err_system;
    }

#if (TSN_HAVE_EPOLLRDHUP)
    wia_epoll_test_rdhup(sys);
#endif
  }

  if (wia_nevents < sys->Events) 
  {
    if (wia_event_list) 
    {
      free(wia_event_list);
    }

    wia_event_list = malloc(sizeof(struct epoll_event) * sys->Events);
    if (wia_event_list == NULL) 
    {
      return -TSN_err_nomem;
    }
  }

  wia_nevents = sys->Events;

#if (TSN_HAVE_CLEAR_EVENT)
  wia_event_flags = TSN_USE_CLEAR_EVENT
#else
  wia_event_flags = TSN_USE_LEVEL_EVENT
#endif
                        |TSN_USE_GREEDY_EVENT
                        |TSN_USE_EPOLL_EVENT;
  
  return TSN_err_none;
}

static void
wia_epoll_done(tsn_sys_config_s *sys __TSN_UNUSED)
{
  if (wia_ep_handle != -1 && close(wia_ep_handle) == -1) 
  {
    TSN_warn("epoll close() failed.\n");
    TSN_error("(%d): %m.\n", errno, errno);
  }

  wia_ep_handle = -1;
  if (wia_event_list != NULL)
  {
    free(wia_event_list);
    wia_event_list = NULL;
  }
  wia_nevents = 0;
}

static void __wia_epoll_free(tsn_sys_config_s *sys)
{
  wia_epoll_done(sys);
}

#if (TSN_HAVE_EPOLLRDHUP)

static void
wia_epoll_test_rdhup(tsn_sys_config_s *sys)
{
  int s[2], events;
  struct epoll_event ee;

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, s) == -1) 
  {
    TSN_error("socketpair() failed.\n");
    TSN_error("(%d): %m.\n", errno, errno);
    return;
  }

  ee.events = EPOLLET|EPOLLIN|EPOLLRDHUP;

  if (epoll_ctl(wia_ep_handle, EPOLL_CTL_ADD, s[0], &ee) == -1) 
  {
    TSN_error("epoll_ctl() failed.\n");
    TSN_error("(%d): %m.\n", errno, errno);
    goto failed;
  }

  if (close(s[1]) == -1) 
  {
    TSN_error("close() failed.\n");
    s[1] = -1;
    goto failed;
  }

  s[1] = -1;

  events = epoll_wait(wia_ep_handle, &ee, 1, 5000);

  if (events == -1) 
  {
    TSN_error("epoll_wait() failed.\n");
    TSN_error("(%d): %m.\n", errno, errno);
    goto failed;
  }

  if (events) 
  {
    wia_use_epoll_rdhup = ee.events & EPOLLRDHUP;

  } 
  else 
  {
    TSN_error("epoll_wait() timed out.\n");
  }

  TSN_debug("testing the EPOLLRDHUP flag: %s.\n",
           wia_use_epoll_rdhup ? "success" : "fail");

failed:

  if (s[1] != -1 && close(s[1]) == -1) 
  {
    TSN_error("close() failed.\n");
  }

  if (close(s[0]) == -1) 
  {
    TSN_error("close() failed.\n");
  }
}
#endif

tsn_err_e
wia_epoll_add_event(tsn_event_s *ev, tsn_int_t event, 
  tsn_uint_t flags)
{
  int                  op;
  uint32_t             events, prev;
  tsn_event_s         *e;
  tsn_connection_s    *c;
  struct epoll_event   ee;

  c = ev->data;

  events = (uint32_t) event;

  if (event == TSN_READ_EVENT) 
  {
    e = &c->write;
    prev = EPOLLOUT;
#if (TSN_READ_EVENT != EPOLLIN|EPOLLRDHUP)
    events = EPOLLIN|EPOLLRDHUP;
#endif
  } 
  else if (event == TSN_WRITE_EVENT)
  {
    e = &c->read;
    prev = EPOLLIN|EPOLLRDHUP;
#if (TSN_WRITE_EVENT != EPOLLOUT)
    events = EPOLLOUT;
#endif
  }
  else
  {
    TSN_error("add_event failed, unknown event %d.\n", event);
    return -TSN_err_system;
  }

  if (e->active) 
  {
    op = EPOLL_CTL_MOD;
    events |= prev;
  } 
  else 
  {
    op = EPOLL_CTL_ADD;
  }

#if (TSN_HAVE_EPOLLEXCLUSIVE && TSN_HAVE_EPOLLRDHUP)
  if (flags & EPOLLEXCLUSIVE) 
  {
    events &= ~EPOLLRDHUP;
  }
#endif

  ee.events = events | (uint32_t) flags;
  ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);

  TSN_debug("epoll add event: fd:%d op:%d ev:%08XD.\n",
           c->fd, op, ee.events);

  if (epoll_ctl(wia_ep_handle, op, c->fd, &ee) == -1) 
  {
    TSN_error("epoll_ctl(%d, %d) failed.\n", op, c->fd);
    TSN_error("(%d): %m.\n", errno, errno);
    return -TSN_err_system;
  }

  ev->active = TSN_TRUE;
  return TSN_err_none;
}

tsn_err_e
wia_epoll_del_event(tsn_event_s *ev, tsn_int_t event, 
  tsn_uint_t flags)
{
  int            op;
  uint32_t       prev;
  tsn_event_s         *e;
  tsn_connection_s    *c;
  struct epoll_event   ee;

  /*
   * when the file descriptor is closed, the epoll automatically deletes
   * it from its queue, so we do not need to delete explicitly the event
   * before the closing the file descriptor
   */

  if (flags & TSN_FLAG_CLOSE_EVENT) 
  {
    ev->active = 0;
    return TSN_err_none;
  }

  c = ev->data;

  if (event == TSN_READ_EVENT) 
  {
    e = &c->write;
    prev = EPOLLOUT;

  } 
  else 
  {
    e = &c->read;
    prev = EPOLLIN|EPOLLRDHUP;
  }

  if (e->active) 
  {
    op = EPOLL_CTL_MOD;
    ee.events = prev | (uint32_t) flags;
    ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);

  } 
  else 
  {
    op = EPOLL_CTL_DEL;
    ee.events = 0;
    ee.data.ptr = NULL;
  }

  TSN_debug("epoll del event: fd:%d op:%d ev:%08XD.\n",
           c->fd, op, ee.events);

  if (epoll_ctl(wia_ep_handle, op, c->fd, &ee) == -1) 
  {
    TSN_error("epoll_ctl(%d, %d) failed.\n", op, c->fd);
    return -TSN_err_system;
  }

  ev->active = TSN_FALSE;

  return TSN_err_none;
}

tsn_err_e
wia_epoll_add_connection(tsn_connection_s *c)
{
  struct epoll_event  ee;

  ee.events = EPOLLIN|EPOLLOUT|EPOLLET|EPOLLRDHUP;
  ee.data.ptr = (void *) ((uintptr_t) c | c->read.instance);

  TSN_debug("epoll add connection: fd:%d ev:%08XD.\n", c->fd, ee.events);

  if (epoll_ctl(wia_ep_handle, EPOLL_CTL_ADD, c->fd, &ee) == -1) 
  {
    TSN_error("epoll_ctl(EPOLL_CTL_ADD, %d) failed.\n", c->fd);
    return -TSN_err_system;
  }

  c->read.active  = TSN_TRUE;
  c->write.active = TSN_TRUE;

  return TSN_err_none;
}

tsn_err_e
wia_epoll_del_connection(tsn_connection_s *c, 
  tsn_uint_t flags)
{
  int                 op;
  struct epoll_event  ee;

  /*
   * when the file descriptor is closed the epoll automatically 
   * deletes it from its queue so we do not need to delete 
   * explicitly the event before the closing the file descriptor
   */

  if (flags & TSN_FLAG_CLOSE_EVENT) 
  {
    c->read.active  = TSN_FALSE;
    c->write.active = TSN_FALSE;
    return TSN_err_none;
  }

  TSN_debug("epoll del connection: fd:%d.\n", c->fd);

  op = EPOLL_CTL_DEL;
  ee.events = 0;
  ee.data.ptr = NULL;

  if (epoll_ctl(wia_ep_handle, op, c->fd, &ee) == -1) 
  {
    TSN_error("epoll_ctl(%d, %d) failed.\n", op, c->fd);
    return -TSN_err_system;
  }

  c->read.active  = TSN_FALSE;
  c->write.active = TSN_FALSE;

  return TSN_err_none;
}

static tsn_err_e
__wia_epoll_process_events(tsn_sys_config_s *sys, 
  tsn_msec_t timer, tsn_uint_t flags)
{
  int            events, err, instance, i;
  uint32_t       revents;
  tsn_uint_t     level;
  tsn_connection_s  *c;
  tsn_event_s       *rev, *wev;

  /* TSN_TIMER_INFINITE == INFTIM */
  events = epoll_wait(wia_ep_handle, wia_event_list, 
    (int) wia_nevents, timer);
  err = (events == -1) ? errno : 0;
  if (err) 
  {
    if (err == EINTR) 
    {
      TSN_event("epoll_wait() failed.\n");
    } 
    else 
    {
      TSN_warn("epoll_wait() failed.\n");
    }

    return -TSN_err_system;
  }

  if (events == 0) 
  {
    if (timer != TSN_TIMER_INFINITE) 
    {
      return TSN_err_none;
    }

    TSN_error("epoll_wait() returned no events without timeout.\n");
    return -TSN_err_system;
  }

  for (i = 0; i < events; i++) 
  {
    c = wia_event_list[i].data.ptr;

    instance = (uintptr_t) c & 1;
    c = (tsn_connection_s *) ((uintptr_t) c & (uintptr_t) ~1);

    rev = &c->read;

    if (c->fd == -1 || rev->instance != instance) 
    {
      /*
       * the stale event from a file descriptor
       * that was just closed in this iteration
       */

      TSN_debug("epoll: stale event %p.\n", c);
      continue;
    }

    revents = wia_event_list[i].events;

    TSN_debug("epoll: fd:%d ev:%04XD d:%p.\n",
             c->fd, revents, wia_event_list[i].data.ptr);

    if (revents & (EPOLLERR|EPOLLHUP)) 
    {
      TSN_debug("epoll_wait() error on fd:%d ev:%04XD.\n",
               c->fd, revents);

      /*
       * if the error events were returned, add EPOLLIN and EPOLLOUT
       * to handle the events at least in one active handler
       */

      revents |= EPOLLIN|EPOLLOUT;
    }

    if ((revents & EPOLLIN) && rev->active) 
    {
      rev->ready = TSN_TRUE;
      rev->available = TSN_FALSE;

#if (TSN_HAVE_EPOLLRDHUP)
      if (revents & EPOLLRDHUP) 
      {
        rev->pending_eof = TSN_TRUE;
      }
#endif
      rev->handler(rev);
    }

    wev = &c->write;

    if ((revents & EPOLLOUT) && wev->active) 
    {
      if (c->fd == -1 || wev->instance != instance) 
      {

        /*
         * the stale event from a file descriptor
         * that was just closed in this iteration
         */

        TSN_debug("epoll: stale event %p.\n", c);
        continue;
      }

      wev->ready = 1;
      wev->handler(wev);
    }
  }

  return TSN_err_none;
}

tsn_err_e
wia_epoll_process_events(tsn_msec_t timer, tsn_uint_t flags)
{
  return __wia_epoll_process_events(&sysCfg, timer, flags);
}

static inline int
tsn_connection_error(int err)
{
  /* Winsock may return NGX_ECONNABORTED instead of NGX_ECONNRESET */
  if (__tsn_is_sockreset(err))
  {
    return 0;
  }

  return err;
}

ssize_t
tsn_unix_send(tsn_connection_s *c, tsn_msg_s *m)
{
  ssize_t       n;
  int           err;
  tsn_event_s  *wev;
  tsn_buffer_s *b;
  
  wev = &c->write;
  b = &m->b;

  for ( ;; ) 
  {
    n = send(c->fd, b->data, b->size, 0);

    TSN_debug("send: fd:%d %z of %uz.\n", c->fd, n, b->size);

    if (n > 0) 
    {
      if (n < (ssize_t)b->size) 
      {
        wev->ready = TSN_FALSE;
      }
      c->sent += n;
      return n;
    }

    err = errno;
    if (n == 0) 
    {
      TSN_warn("send() returned zero.\n");
      wev->ready = TSN_FALSE;
      return n;
    }

    if (err == EAGAIN || err == EINTR) 
    {
      wev->ready = TSN_FALSE;

      TSN_debug("send() not ready.\n");

      if (err == EAGAIN) 
      {
        return -EAGAIN;
      }
    } 
    else 
    {
      wev->error = TSN_TRUE;
      return -err;
    }
  }
}

ssize_t
tsn_unix_sendto(tsn_connection_s *c, tsn_msg_s *m)
{
  ssize_t       n;
  int           err;
  tsn_event_s  *wev;
  tsn_buffer_s *b;
  
  wev = &c->write;
  b = &m->b;
  for ( ;; ) 
  {
    n = sendto(c->fd, b->data, b->size, 0, m->from.sa, m->from.slen);

    TSN_debug("sendto: fd:%d %z of %uz.\n", c->fd, n, b->size);

    if (n >= 0) 
    {
      if ((size_t) n != b->size) 
      {
        wev->error = TSN_TRUE;
        return -errno;
      }

      c->sent += n;
      return n;
    }

    err = errno;
    if (err == EAGAIN) 
    {
      wev->ready = TSN_FALSE;
      TSN_debug("sendto() not ready.\n");
      return -EAGAIN;
    }

    if (err != EINTR) 
    {
      wev->error = TSN_TRUE;
      return -err;
    }
  }
}

ssize_t
tsn_unix_recv(tsn_connection_s *c, tsn_msg_s *m)
{
  ssize_t       n;
  int     err;
  tsn_event_s  *rev;
  tsn_buffer_s *b;

  rev = &c->read;
  b = &m->b;

#if (TSN_HAVE_EPOLLRDHUP)
  if ((wia_event_flags & TSN_USE_EPOLL_EVENT)
      && wia_use_epoll_rdhup)
  {
    TSN_debug("recv: eof:%d, avail:%d.\n",
             rev->pending_eof, rev->available);

    if (!rev->available && !rev->pending_eof) 
    {
      rev->ready = TSN_FALSE;
      return -EAGAIN;
    }
  }
#endif

  do 
  {
    if (c->addr.slen)
      n = recvfrom(c->fd, b->ptr, b->size, 0, c->addr.sa, &c->addr.slen);
    else
      n = recv(c->fd, b->ptr, b->size, 0);

    TSN_debug("recv: fd:%d %z of %uz.\n", c->fd, n, b->size);

    if (n == 0) 
    {
      rev->ready = TSN_FALSE;
      rev->eof = TSN_TRUE;
      return 0;
    }

    if (n > 0) 
    {
#if (TSN_HAVE_EPOLLRDHUP)
      if ((wia_event_flags & TSN_USE_EPOLL_EVENT)
          && wia_use_epoll_rdhup)
      {
        if ((size_t) n < b->size) 
        {
          if (!rev->pending_eof) 
          {
            rev->ready = TSN_FALSE;
          }

          rev->available = TSN_FALSE;
        }

        return n;
      }
#endif

      if ((size_t) n < b->size
          && !(wia_event_flags & TSN_USE_GREEDY_EVENT))
      {
        rev->ready = TSN_FALSE;
      }

      return n;
    }

    err = errno;
    if (err == EAGAIN || err == EINTR) 
    {
      TSN_debug("recv() not ready.\n");
      n = -TSN_err_eagain;
    } 
    else 
    {
      n = -tsn_connection_error(err);
      break;
    }
  } 
  while (err == EINTR);

  rev->ready = TSN_FALSE;

  if (n < 0) 
  {
    rev->error = TSN_TRUE;
  }

  return n;
}

tsn_err_e wia_epoll_init(void)
{
  return __wia_epoll_init(&sysCfg);
}

void wia_epoll_free(void)
{
  return __wia_epoll_free(&sysCfg);
}

