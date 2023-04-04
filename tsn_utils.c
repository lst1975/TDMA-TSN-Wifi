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

void
tsn_sockaddr_print(tsn_sockaddr_s *s, const char *head, const char *tail)
{
  char ipstr[INET6_ADDRSTRLEN];
  tsn_print("%s%s%s", head, inet_ntop(s->sa->sa_family, s, ipstr, 
    s->sa->sa_family == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN), tail);
}

int
tsn_sockaddr_salen(tsn_sockaddr_s *s)
{
  return s->sa->sa_family == AF_INET ? sizeof(s->u.addr4) : sizeof(s->u.addr6);
}

tsn_boolean_e
tsn_sockaddr_isequal(tsn_sockaddr_s *u, struct sockaddr *sa)
{
  if (sa->sa_family != u->sa->sa_family)
  {
    return TSN_FALSE;
  }
  else
  {
    tsn_sockaddr_s *s = (tsn_sockaddr_s *)sa;
    s->sa = sa;
    
    switch (sa->sa_family)
    {
      case AF_INET:
        if (u->u.addr4.sin_port != s->u.addr4.sin_port)
          return TSN_FALSE;
        if (u->u.addr4.sin_addr.s_addr != s->u.addr4.sin_addr.s_addr)
          return TSN_FALSE;
        break;
        
      case AF_INET6:
        if (u->u.addr6.sin6_port != s->u.addr6.sin6_port)
          return TSN_FALSE;
        if (tsn_memcmp(&u->u.addr6.sin6_addr, &s->u.addr6.sin6_addr, 16))
          return TSN_FALSE;
        break;
        
      default:
        return TSN_FALSE;
    }
  }
  return TSN_TRUE;
}
