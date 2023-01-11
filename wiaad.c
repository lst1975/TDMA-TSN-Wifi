/******************************************************************************
 *  Author:    980680431@qq.com
 *  Liscence:  GPLV3
 *  Copyright: 980680431@qq.com 
 ******************************************************************************/
#include "tsn_private.h"

int main(int argc, char **argv)
{
  int ret = EXIT_FAILURE;
  tsn_err_e r;

  r = wia_epoll_init();
  if (r != TSN_err_none)
  {
    TSN_error("wia_epoll_init failed.\n");
    goto clean1;
  }

  r = tsn_server_init();
  if (r != TSN_err_none)
  {
    TSN_error("tsn_server_init failed.\n");
    goto clean0;
  }

  while (sysCfg.run)
  {
    r = wia_epoll_process_events(200, 0);
    if (r != TSN_err_none)
    {
      TSN_warn("Failed to wia_epoll_process_events().\n");
    }
  }
  
  ret = EXIT_SUCCESS;

clean2:
  tsn_server_free();
clean1:
  wia_epoll_free();
clean0:
  return ret;
}
