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
 
#ifndef __TSN_UTILS_H__
#define __TSN_UTILS_H__

struct tsn_sockaddr{
  union {
    struct sockaddr_in  addr4;
    struct sockaddr_in6 addr6;
  }u;
  struct sockaddr  *sa;
  socklen_t slen;
};

typedef struct tsn_sockaddr tsn_sockaddr_s;

void tsn_sockaddr_print(tsn_sockaddr_s *s, 
      const char *head, const char *tail);
int tsn_sockaddr_salen(tsn_sockaddr_s *s);
tsn_boolean_e tsn_sockaddr_isequal(tsn_sockaddr_s *u, struct sockaddr *sa);

#define TSN_error(fmt, ...) do { if (sysCfg.logError) ___TSN_string("Error", fmt, ##__VA_ARGS__); } while(0)
#define TSN_event(fmt, ...) do { if (sysCfg.logEvent) ___TSN_string("Event", fmt, ##__VA_ARGS__); } while(0)
#define TSN_warn(fmt, ...)  do { if (sysCfg.logWarn)  ___TSN_string("Warn",  fmt, ##__VA_ARGS__); } while(0)
#define TSN_debug(fmt, ...) do { if (sysCfg.logDebug) ___TSN_string("Debug", fmt, ##__VA_ARGS__); } while(0)

#endif

