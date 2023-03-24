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
#ifndef __TSN_EPOLL_H__
#define __TSN_EPOLL_H__

#define TSN_HAVE_EPOLLRDHUP     1
#define TSN_HAVE_EPOLLEXCLUSIVE 1
#define TSN_HAVE_CLEAR_EVENT    1
#define TSN_HAVE_WIN32          0

#define TSN_READ_EVENT  (EPOLLIN|EPOLLRDHUP)
#define TSN_WRITE_EVENT EPOLLOUT

static inline tsn_boolean_e __tsn_is_sockreset(int err)
{
#if (TSN_HAVE_WIN32)
  return (err == ECONNRESET || err == ECONNABORTED);
#else
  return (err == ECONNRESET);
#endif
}   

#if (TSN_HAVE_EPOLLEXCLUSIVE)
#define TSN_FLAG_EXCLUSIVE_EVENT  EPOLLEXCLUSIVE
#else
#define TSN_FLAG_EXCLUSIVE_EVENT  0
#endif

/*
 * The event filter notifies only the changes and an initial level:
 * kqueue, epoll.
 */
#define TSN_USE_CLEAR_EVENT      0x00000004

/*
 * The event filter is epoll.
 */
#define TSN_USE_EPOLL_EVENT      0x00000040

/*
 * The event filter requires to read/write the whole data:
 * select, poll, /dev/poll, kqueue, epoll.
 */
#define TSN_USE_LEVEL_EVENT      0x00000001

/*
 * The event filter requires to do i/o operation until EAGAIN: epoll.
 */
#define TSN_USE_GREEDY_EVENT     0x00000020

/*
 * The event filter is deleted just before the closing file.
 * Has no meaning for select and poll.
 * kqueue, epoll, eventport:         allows to avoid explicit delete,
 *                                   because filter automatically is deleted
 *                                   on file close,
 *
 * /dev/poll:                        we need to flush POLLREMOVE event
 *                                   before closing file.
 */
#define TSN_FLAG_CLOSE_EVENT     0x01
#define TSN_FLAG_POST_EVENTS     0x02

typedef struct tsn_event tsn_event_s;
typedef struct tsn_connection tsn_connection_s;

struct tsn_event {
  uint64_t instance:1;
  uint64_t active:1;
  uint64_t available:1;
  uint64_t ready:1;
  uint64_t pending_eof:1;
  uint64_t accept:1;
  uint64_t eof:1;
  uint64_t error:1;
  void  *data;
  void (*handler)(tsn_event_s *ev);
  list_head_s msgs;
};

struct tsn_connection{
  tsn_event_s read;
  tsn_event_s write;
  tsn_sockaddr_s addr;
  tsn_sockaddr_s client;
  uint64_t sent;
  int type;
  int protocol;
  tsn_socket_t fd;
  uint16_t port;
  uint16_t family;
  tsn_err_e (*process)(tsn_msg_s *m);
  void (*tsn_read)(tsn_event_s *ev); 
  void (*tsn_send)(tsn_event_s *ev);
};

#define TSN_TIMER_INFINITE (time_t)(-1)

tsn_err_e wia_epoll_init(void);
void wia_epoll_free(void);

tsn_err_e wia_epoll_add_event(tsn_event_s *ev, tsn_int_t event, tsn_uint_t flags);
tsn_err_e wia_epoll_del_event(tsn_event_s *ev, tsn_int_t event, tsn_uint_t flags);
tsn_err_e wia_epoll_add_connection(tsn_connection_s *c);
tsn_err_e wia_epoll_del_connection(tsn_connection_s *c, tsn_uint_t flags);
tsn_err_e wia_epoll_process_events(tsn_msec_t timer, tsn_uint_t flags);

int tsn_set_nonblocking(tsn_socket_t s, int nb);
int tsn_set_tcpnopush(tsn_socket_t s, int cork);
ssize_t tsn_unix_recv(tsn_connection_s *c, tsn_msg_s *m);
ssize_t tsn_unix_sendto(tsn_connection_s *c, tsn_msg_s *m);
ssize_t tsn_unix_send(tsn_connection_s *c, tsn_msg_s *m);

#endif
