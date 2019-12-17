#ifndef _MY_DBG_H_
#define _MY_DBG_H_

#include <stdio.h>

#if 1
	#define my_detail(format, args...) printf("\n\r DEBUG [%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
	#define my_info(format, args...)   printf("\n\r INFO [%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
	#define my_warn(format, args...)   printf("\n\r WARN [%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
	#define my_error(format, args...)  printf("\n\r ERROR[%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
	#define my_enter(format, args...)  printf("\n\r ENTER [%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
	#define my_level(format, args...)  printf("\n\r LEVEL [%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
	#define my_msg(format, args...)    printf("\n\r message [%s|%s|%d]\t"format"\n\r",__FILE__,__func__,__LINE__,##args)
#else
	#define my_detail(format, args...)
	#define my_info(format, args...)
	#define my_warn(format, args...)
	#define my_error(format, args...)
	#define my_enter(format, args...)
	#define my_level(format, args...)
	#define my_msg(format, args...)
#endif


#endif // _MY_DBG_H_
