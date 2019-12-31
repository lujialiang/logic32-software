/*
 * 自定义调试信息
*/

#ifndef _MY_DBG_H_
#define _MY_DBG_H_

//#include "elibs.h"

#if 1
	#define my_detail(format, args...) printf("\n DEBUG [%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
	#define my_info(format, args...)   printf("\n INFO [%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
	#define my_wrn(format, args...)   printf("\n WARN [%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
	#define my_error(format, args...)  eprintf("\n ERROR[%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
	#define my_enter(format, args...) printf("\n ENTER [%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
	#define my_level(format, args...) printf("\n LEVEL [%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
	#define my_msg(format, args...)    printf("\n message [%s|%s|%d]\t"format"\n",__FILE__,__func__,__LINE__,##args)
#else
	#define my_detail(format, args...)
	#define my_info(format, args...)
	#define my_wrn(format, args...)
	#define my_error(format, args...)
	#define my_enter(format, args...)
	#define my_level(format, args...)
	#define my_msg(format, args...)
#endif


#endif // _MY_DBG_H_
