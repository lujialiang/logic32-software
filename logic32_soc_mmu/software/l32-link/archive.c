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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "a_out.h"
#include "asm.h"
#include "hash.h"
#include "link.h"
#include "misc.h"
#include "_ar.h"

int get_file_list_from_archive(char *archive, struct obj_file_info *file_list_head)
{
	int fd;
	int nr;
	char buf[100];
	struct ar_hdr ahdr;
	off_t obj_file_size;
	char obj_file_name[MAXNAMLEN+1];
	size_t lname;
	off_t offset;
	struct obj_file_info *file_list_tail = file_list_head;
	// open archive, read only
	fd = open(archive, O_RDONLY);
	if( fd < 0 ) {
		fprintf(stderr, "%s: %s\n", archive, strerror(errno));
		return -1;
	}
	// check ar magic
	if( (nr = read(fd, buf, SARMAG)) != SARMAG ) {
		fprintf(stderr, "%s: %s\n", archive, strerror(errno));
		close(fd);
		return -1;
	}
	if( strncmp((const char*)buf, ARMAG, SARMAG) ) {
		fprintf(stderr, "%s: %s\n", archive, "bad magic");
		close(fd);
		return -1;
	}
	// initial file list
	memset(file_list_head, 0, sizeof(struct obj_file_info));
	file_list_tail = file_list_head;

	while(1) {
		// read ar header
		nr = read(fd, &ahdr, ARHDRSZ);
		if( nr == 0 ) {
			// end of file
			break;
		}
		else if( nr != ARHDRSZ ) {
			fprintf(stderr, "%s: %s\n", archive, "bad format");
			close(fd);
			return -1;
		}

		// check magic
		if( strncmp(ahdr.ar_fmag, ARFMAG, sizeof(ARFMAG) - 1)) {
			fprintf(stderr, "%s: %s\n", archive, "bad magic");
			close(fd);
			return -1;
		}

		if( ahdr.ar_name[0] == ' ' ) {
			fprintf(stderr, "%s: %s\n", archive, "bad format");
			close(fd);
			return -1;
		}

		if( !bcmp(ahdr.ar_name, AR_EFMT1, sizeof(AR_EFMT1) - 1)) {
			// long file name
			lname = atoi(ahdr.ar_name + sizeof(AR_EFMT1) - 1);
			if( lname <= 0 || lname > MAXNAMLEN ) {
				fprintf(stderr, "%s: %s\n", archive, "bad format");
				close(fd);
				return -1;
			}
			nr = read(fd, obj_file_name, (size_t)lname);
			if( nr != lname ) {
				if( nr < 0 ) {
					fprintf(stderr, "%s: %s\n", archive, strerror(errno));
				}
				else {
					fprintf(stderr, "%s: %s\n", archive, "bad format");
				}
				close(fd);
				return -1;
			}
			obj_file_name[lname] = '\0';
		}
		else {
			char *p;
			lname = 0;
			bcopy(ahdr.ar_name, obj_file_name, sizeof(ahdr.ar_name));
			for(p = obj_file_name + sizeof(ahdr.ar_name) - 1; *p == ' '; --p);
			*++p = '\0';
		}

		if( (offset = lseek(fd, 0, SEEK_CUR)) == (off_t)-1 ) {
			fprintf(stderr, "%s: %s\n", archive, strerror(errno));
			close(fd);
			return -1;
		}

		bcopy(ahdr.ar_size, buf, sizeof(ahdr.ar_size));
		buf[sizeof(ahdr.ar_size)] = '\0';
		obj_file_size = strtol(buf, (char **)NULL, 10);
	
		if( lseek(fd, obj_file_size, SEEK_CUR) == (off_t)-1 ) {
			fprintf(stderr, "%s: %s\n", archive, strerror(errno));
			close(fd);
			return -1;
		}

		struct obj_file_info *obj_info = (struct obj_file_info *)malloc(sizeof(struct obj_file_info));
		if( obj_info == NULL ) {
			fprintf(stderr, "%s: %s\n", "malloc()", strerror(errno));
			close(fd);
			return -1;
		}
		strcpy((char *)(obj_info->filename), (const char *)obj_file_name);
		obj_info->fd = fd;
		obj_info->size = obj_file_size - lname;
		obj_info->offset = offset;
		obj_info->link = NULL;
		file_list_tail->link = obj_info;
		file_list_tail = obj_info;
	}
	return fd;
}

int extract_obj_files(char *archive, struct obj_file_info *file_list_head)
{
	int cnt;
	int fd;
	struct obj_file_info *p, *t;
	p = file_list_head->link;
	cnt = 0;
	while(p) {
		t = p->link;
		fd = p->fd;
		if( lseek(fd, p->offset, SEEK_SET) != p->offset) {
			fprintf(stderr, "%s: %s\n", archive, strerror(errno));
			close(fd);
			return -1;
		}
		struct exec exec_head;
		int nr = read(fd, &exec_head, sizeof(struct exec));
		if( nr != sizeof(struct exec) ) {
			fprintf(stderr, "%s: %s\n", archive, strerror(errno));
			close(fd);
			return -1;
		}
		if( exec_head.a_magic != A_MAGIC1 ) {
			fprintf(stderr, "%s %s\n", p->filename, "bad magic");
			close(fd);
			return -1;
		}
		p = t;
		cnt++;
	}
	close(fd);
	printf("total extract %d files in %s\n", cnt, archive);
	return 0;
}

#ifdef AR_MAIN

void usage(char *argv[])
{
	printf("%s libfile\n", argv[0]);
}

int main(int argc, char *argv[])
{
	int archive_fd;
	struct obj_file_info list_head;
	if( argc != 2 ) {
		usage(argv);
		exit(1);
	}
	archive_fd = get_file_list_from_archive(argv[1], &list_head);
#if 0
	struct obj_file_info *p, *t;
	if( archive_fd >= 0 ) {
		int cnt = 0;
		p = list_head.link;
		while(p) {
			t = p->link;
			printf("file: '%s', size: %ld, offset: %ld\n", p->filename, p->size, p->offset);
			p = t;
			cnt++;
		}
		printf("total %d object files\n", cnt);
	}
#else
	if( archive_fd >= 0 ) {
		extract_obj_files(argv[1], &list_head);
	}
#endif
	return 0;
}
#endif
