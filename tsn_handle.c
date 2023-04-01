
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

#define TSN_HANDLE_TIMEOUT 30000

struct _res_handle{
  tsn_handle_t Handle;
  list_head_s link;
};
typedef struct _res_handle TsnHandle;

struct _res_handle_s{
  tsn_handle_t Handle;
  list_head_s head;
};
typedef struct _res_handle_s tsn_handle_s;

static TsnHandle tsn_handle_array[TSN_HANDLE_MAX];

static tsn_handle_s tsn_handle_list = {
  .Handle = 0,
  .head = LIST_HEAD_INIT(tsn_handle_list.head),
};
 
static tsn_boolean_e 
__TSN_AllocateHandle(tsn_handle_t *Handle)
{
  tsn_handle_s *s = &tsn_handle_list;
  if (s->Handle >= TSN_HANDLE_MAX - 1)
  {
    TsnHandle *a;
    if (list_empty(&s->head))
      return TSN_FALSE;
    a = list_first_entry(&s->head, TsnHandle, link);
    list_del(&a->link);
    *Handle = a->Handle;
  }
  else
  {
    *Handle = s->Handle++;
  }
  return TSN_TRUE;
}

static void 
__TSN_FreeHandle(tsn_handle_t *_Handle)
{
  tsn_handle_t Handle = *_Handle;

  if (Handle == TSN_HANDLE_INVALID)
    return;
  
  tsn_handle_s *s = &tsn_handle_list;
  TsnHandle *a = &tsn_handle_array[Handle];
  list_add_tail(&a->link, &s->head);
  *_Handle = TSN_HANDLE_INVALID;
}

static tsn_msg_s* tsn_handle_msg_array[TSN_HANDLE_MAX]={0};
static list_head_s tsn_handle_msg_list = LIST_HEAD_INIT(tsn_handle_msg_list);
 
tsn_boolean_e 
TSN_AllocateHandle(tsn_msg_s *msg)
{
  if (TSN_FALSE == __TSN_AllocateHandle(&msg->handle))
    return TSN_FALSE;

  tsn_handle_msg_array[msg->handle] = msg;
  list_add_tail(&msg->wait, &tsn_handle_msg_list);
  msg->stamp = tsn_system_time();
  msg->isInQ = TSN_TRUE;
  return TSN_TRUE;
}

void 
TSN_FreeHandle(tsn_msg_s *msg)
{
  if (msg->handle == TSN_HANDLE_INVALID)
    return;
  if (msg->isInQ == TSN_FALSE)
    return;
  tsn_handle_msg_array[msg->handle] = NULL;
  list_del(&msg->wait);
  __TSN_FreeHandle(&msg->handle);
}

tsn_msg_s * 
TSN_GetMsgByHandle(tsn_handle_t Handle)
{
  TimeData stamp;
  tsn_msg_s *msg;

  if (msg->handle == TSN_HANDLE_INVALID)
    return NULL;
  msg = tsn_handle_msg_array[Handle];
  if (msg == NULL)
    return NULL;
  stamp = tsn_system_time();
  if (stamp - msg->stamp > TSN_HANDLE_TIMEOUT)
    return NULL;
  return msg;
}

void 
TSN_CheckHandle(void)
{
  int i;

  for (i == 0; i < TSN_HANDLE_MAX; i++)
  {
    tsn_msg_s *msg = TSN_GetMsgByHandle(i);
    if (msg == NULL)
    {
      msg = tsn_handle_msg_array[i];
      if (msg == NULL)
        continue;
      TSN_FreeHandle(msg);
      if (msg->cb != NULL)
        msg->cb(msg);
    }
  }
}

void
TSN_HandleListInit(void)
{
  tsn_memset(&tsn_handle_msg_array[0], 0, sizeof(tsn_handle_msg_array));
}
