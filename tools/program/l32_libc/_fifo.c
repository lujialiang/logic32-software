#include "_fifo.h"

void fifo_init(fifo_t *pfifo) {
	pfifo->full = 0;
	pfifo->empty = 1;
	pfifo->head = pfifo->buf;
	pfifo->tail = pfifo->buf;
}
	
uint8_t fifo_put(fifo_t *pfifo, uint8_t dat) {
	if( pfifo->full == 0 ) {
#ifdef DBG_FIFO
		printf("put %02X\n",dat);
#endif
		*pfifo->head = dat;
		if( pfifo->head == pfifo->buf+FIFO_DEEP-1 ) {
			pfifo->head = pfifo->buf;
		} else {
			pfifo->head++;
		}
		if( pfifo->head == pfifo->buf+FIFO_DEEP-1 && pfifo->tail == pfifo->buf ) {
			pfifo->full = 1;
		} else if ( pfifo->head + 1 == pfifo->tail ) {
			pfifo->full = 1;
		}
		pfifo->empty = 0;
		return 0;
	}
#ifdef DBG_FIFO
	puts("error: fifo full");
#endif
	return -1;
}

uint8_t fifo_get(fifo_t *pfifo, uint8_t* dat) {
	if( pfifo->empty == 1 ) {
#ifdef DBG_FIFO
		puts("error: pfifo empty");
#endif
		return -1;
	} else {
		*dat = *pfifo->tail;
#ifdef DBG_FIFO
		printf("get %02X\n",*dat);
#endif
		if( pfifo->tail == pfifo->buf+FIFO_DEEP-1 ) {
			pfifo->tail = pfifo->buf;
		} else {
			pfifo->tail++;
		}
		pfifo->empty = pfifo->head == pfifo->tail ? 1 : 0;
		return 0;
	}
}
