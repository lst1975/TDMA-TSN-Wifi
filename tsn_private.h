#ifndef __TSN_PRIVATE_H__
#define __TSN_PRIVATE_H__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <endian.h>

#include "gw_base.h"
#include "gw_type.h"

#include "crc.h"
#include "fcs.h"

#include "gw_list.h"
#include "gw_buffer.h"
#include "gw_number.h"
#include "gw_mib.h"
#include "gw_addr.h"
#include "gw_log.h"
#include "gw_state.h"
#include "tsn_device.h"
#include "tsn_system.h"
#include "tsn_epoll.h"
#include "tsn_udp.h"

#endif
