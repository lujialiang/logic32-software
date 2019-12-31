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
	
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <termios.h>

//#define _DARWIN_

#define CHAR_CTL_C  -1L
#define CHAR_NONE   -2L

static struct termios cfg, old;

static int ctlCSeen = 0;

//setup the terminal
void kbd_init(void)
{
	int ret;
	ret = tcgetattr(0, &old);
	cfg = old;
	if(ret) {
		perror("cannot get term attrs");
	}
	#ifndef _DARWIN_
	//cfg.c_iflag &=~ (INLCR | INPCK | ISTRIP | IUCLC | IXANY | IXOFF | IXON);
	cfg.c_iflag &=~ (INLCR | INPCK | ISTRIP | IUCLC | IXANY | IXOFF | IXON | BRKINT);

	//cfg.c_oflag &=~ (OPOST | OLCUC | ONLCR | OCRNL | ONOCR | ONLRET);

	cfg.c_lflag &=~ (ECHO | ECHOE | ECHONL | ICANON | IEXTEN | XCASE);
	#else
	cfmakeraw(&cfg);
	#endif
	ret = tcsetattr(0, TCSANOW, &cfg);
	if(ret) {
		perror("cannot set term attrs");
	}
}

int kbd_readchar(void)
{
	struct timeval tv;
	fd_set set;
	char c;
	int i, ret = CHAR_NONE;
	if(ctlCSeen) {
		ctlCSeen = 0;
		return 0x03;
	}
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&set);
	FD_SET(0, &set);
	i = select(1, &set, NULL, NULL, &tv);
	if(i == 1 && 1 == read(0, &c, 1)) {
		ret = c;
	}
	return ret;
}

void kbd_writechar(int chr)
{
	if(!(chr & 0xFF00)) {
		putchar(chr);
		if(chr == '\n') {
			putchar('\r');
		}
	}
	else{
		printf("<<~~ EC_0x%x ~~>>", chr);
	}
	fflush(stdout);	
}

void kbd_deinit(void)
{
	tcsetattr(0, TCSANOW, &old);
}
	
//handle SIGTERM      
void ctl_cHandler(int v)
{
	//exit(-1);
	ctlCSeen = 1;
}
