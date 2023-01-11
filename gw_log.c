#include "tsn_private.h"

#define __DECL_F(n) [FRAME_TYPE_ ## n] = { \
  .number=FRAME_TYPE_ ## n, \
  .name=#n, \
  .func=do_FRAME_TYPE_ ## n, \
}
  
static const char *errTable[]={
  "TSN_err_none",
  "TSN_err_malformed",
  "TSN_err_tooshort",
  "TSN_err_unsupport",
};

const char *TSN_err2str(int err)
{
  if (-err > TSN_err_max)
    return "<unknown>";
  return errTable[-err];
}

void ___TSN_string(const char *type, const char *fmt, ...)
{
  char buffer[1024];

  va_list args;
  va_start (args, fmt);
  vsnprintf(buffer,sizeof(buffer),fmt, args);
  va_end (args);

  printf("%s: %s", type, buffer);
}

void
tsn_print_sockaddr(struct sockaddr *sa)
{

  if (sa->sa_family == AF_INET)
  {
    char buf[INET_ADDRSTRLEN];
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;
    printf("socket family: IPv4\n");
    printf("socket address: %s\n", inet_ntop(AF_INET, &sin->sin_addr, buf, sizeof(buf)));
    printf("socket port: %u\n", TSN_ntohs(sin->sin_port));
  }
  else
  {
    char buf[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
    printf("Family: IPv6\n");
    printf("IPv6 Address: %s\n", inet_ntop(AF_INET6, &sin6->sin6_addr, buf, sizeof(buf)));
    printf("Socket Port: %u\n", TSN_ntohs(sin6->sin6_port));
  }
}

