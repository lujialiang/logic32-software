/*-
 * Copyright (c) 2017 Lu JiaLiang
 * Email: 1056568029@qq.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>

#ifdef linux
#include <linux/if.h>
#include <linux/if_tun.h>
#define DEVTAP "/dev/net/tun"
#else  /* linux */
#define DEVTAP "/dev/tap0"
#endif /* linux */

/*
static int drop = 0;
*/
static int fd;

/*---------------------------------------------------------------------------*/
int tapdev_init(uint8_t *ip_addr,uint8_t *netmask,uint8_t *gateway)
{
  char buf[1024];
  
  fd = open(DEVTAP, O_RDWR);
  if(fd == -1) {
	fprintf(stderr, "open(): %s\n", strerror(errno));
    return -1;
  }

#ifdef linux
  {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP|IFF_NO_PI;
    if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
	  fprintf(stderr, "ioctl(): %s\n", strerror(errno));
      return -1;
    }
  }
#endif /* Linux */

  snprintf(buf, sizeof(buf), "ifconfig tap0 %d.%d.%d.%d netmask %d.%d.%d.%d",\
	   ip_addr[0], ip_addr[1], ip_addr[2], 1,\
	   netmask[0], netmask[1], netmask[2], netmask[3]);
  puts(buf);
  system(buf);
  snprintf(buf, sizeof(buf), "route add default gw %d.%d.%d.%d dev tap0",\
	   gateway[0], gateway[1], gateway[2], gateway[3]);
  puts(buf);
  system(buf);
  return 0;
}
/*---------------------------------------------------------------------------*/
unsigned char *tapdev_read(unsigned char *recv_buf,int *recv_len)
{
  fd_set fdset;
  struct timeval tv; //, now;
  int ret;
  
  tv.tv_sec = 0;
  tv.tv_usec = 1000;


  FD_ZERO(&fdset);
  FD_SET(fd, &fdset);

  ret = select(fd + 1, &fdset, NULL, NULL, &tv);
  if(ret == 0) {
    *recv_len = 0;
    return NULL;
  }
  ret = read(fd, recv_buf, 65536);
  if(ret == -1) {
	fprintf(stderr, "read(): %s\n", strerror(errno));
    *recv_len = 0;
	return NULL;
  }

#if 0
	if(ret) {
		printf("\n--- tap_dev: tapdev_read: read %d bytes\n", ret);
		int i;
		for(i = 0; i < ret; i++) {
		  printf("%02X", recv_buf[i]);
		  putchar(((i+1)%16)?32:10);
		}
		putchar(10);
	}
#endif
  /*  check_checksum(uip_buf, ret);*/
  *recv_len = ret;
  return recv_buf;
}
/*---------------------------------------------------------------------------*/
void tapdev_send(unsigned char *pkg_data,unsigned int len)
{
  int ret;
  /*  printf("tapdev_send: sending %d bytes\n", size);*/
  /*  check_checksum(uip_buf, size);*/

  /*  drop++;
  if(drop % 8 == 7) {
    printf("Dropped a packet!\n");
    return;
    }*/
  ret = write(fd, pkg_data, len);
  if(ret == -1) {
	fprintf(stderr, "write(): %s\n", strerror(errno));
    exit(1);
  }
#if 0
  puts("tapdev_send()");
#endif
}
/*---------------------------------------------------------------------------*/
