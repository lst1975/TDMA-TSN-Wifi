PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE ?= debug

OBJS_GW = wiagw.o gw_log.o tsn_epoll.o tsn_device.o tsn_system.o \
			crc16.o crc32.o crc64.o crc.o fcs16.o tsn_udp.o \
			sha.o sha1.o sha224.o sha256.o sha384.o sha512.o \
			md5.o hmac.o tsn_frame_gateway.o tsn_encrypt.o \
			tsn_ad.o tsn_resource.o tsn_handle.o tsn_state_gw.o \
			tsn_auth.o
			
OBJS_AD = wiaad.o gw_log.o tsn_epoll.o tsn_device.o tsn_system.o \
			crc16.o crc32.o crc64.o crc.o fcs16.o tsn_udp.o \
			sha.o sha1.o sha224.o sha256.o sha384.o sha512.o \
			md5.o hmac.o tsn_frame_ad.o tsn_encrypt.o \
			tsn_ad.o tsn_resource.o tsn_handle.o   \
			tsn_auth.o tsn_frame_dll.o
			
OBJS_FD = wiafd.o tsn_epoll.o gw_log.o tsn_epoll.o tsn_device.o tsn_system.o \
			crc16.o crc32.o crc64.o crc.o fcs16.o tsn_udp.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
    $(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	wiagw wiaad

wiagw:	$(OBJS_GW)
	$(CC) $(LDFLAGS) -o $@ $^
	$(EXTRA_CMDS)

wiaad:	$(OBJS_AD)
	$(CC) $(LDFLAGS) -o $@ $^
	$(EXTRA_CMDS)

wiafd:	$(OBJS_FD)
	$(CC) $(LDFLAGS) -o $@ $^
	$(EXTRA_CMDS)

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr wiagw $(OBJS_GW) $(EXTRA_CLEAN)
	rm -fr wiaad $(OBJS_AD) $(EXTRA_CLEAN)
	
