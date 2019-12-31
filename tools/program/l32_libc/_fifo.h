#ifndef FIFO_H_INCLUDE
#define FIFO_H_INCLUDE

#include <stdint.h>

#ifdef DBG_FIFO
#include <stdio.h>
#endif

#define FIFO_DEEP 32

typedef struct fifo_s {
	int32_t full;
	int32_t empty;
	uint8_t* head;
	uint8_t* tail;
	uint8_t buf[FIFO_DEEP];
} fifo_t;

void fifo_init(fifo_t *pfifo); 
uint8_t fifo_put(fifo_t *pfifo, uint8_t dat); 
uint8_t fifo_get(fifo_t *pfifo, uint8_t* dat); 

#endif //FIFO_H_INCLUDE
