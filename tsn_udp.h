#ifndef __TSN_UDP_H__
#define __TSN_UDP_H__

#define TSN_BUFF_LEN_MAX 1024

tsn_err_e __tsn_server_init(tsn_connection_s *c,
  void (*tsn_read)(tsn_event_s *ev), 
  void (*tsn_send)(tsn_event_s *ev));
void __tsn_server_free(tsn_connection_s *c);

void tsn_send_udp_msg(tsn_msg_s *m);

#endif
