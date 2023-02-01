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

