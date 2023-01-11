#include "tsn_private.h"

#define SERVER_PORT 8766

static tsn_connection_s tsn_udp_server = {
  .fd         = -1,
  .type       = SOCK_DGRAM,
  .family     = AF_INET,
  .protocol   = IPPROTO_UDP,
  .port       = SERVER_PORT,
  .sent       = 0,
};

tsn_err_e tsn_server_init(void)
{
  return __tsn_server_init(&tsn_udp_server);
}

void tsn_server_free(void)
{
  return __tsn_server_free(&tsn_udp_server);
}

