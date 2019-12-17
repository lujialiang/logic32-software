#include <stdio.h>
#include <stdint.h>

#include "heap.h"
#include "malloc.h"

static uint8_t memory[1024];

void heap_init(void)
{
	OS_Heap_t *heap;
	heap = OS_HeapCreate("HEAP_USER", memory, sizeof(memory));
	OS_HeapRegister(HEAP_USER, heap);
}

typedef struct node_tag {
	int data;
	struct node_tag *next;
} node_t ;

char buffer[1024];

void free_nodes(node_t *p)
{
	node_t *t;
	while(p) {
		t = p->next;
		sprintf(buffer, "p:%p, d:%d\n", p, p->data);
		uart_puts(buffer);
		_free(p);
		p = t;
	}
}

int main(int argc, char *argv[])
{
	node_t *p1;
	node_t *p2;
	node_t *p3;
	heap_init();
	p1 = (node_t *)_calloc(1, sizeof(node_t));
	if( p1 == NULL ) {
		uart_puts("p1, malloc error\n");
	}
	else {
		p1->data = 11;
		//p1->next = NULL;
	}
	p2 = (node_t *)_calloc(1, sizeof(node_t));
	if( p1 == NULL ) {
		uart_puts("p2, malloc error\n");
	}
	else {
		p1->next = p2;
		p2->data = 22;
		//p2->next = NULL;
	}
#if 0
	p3 = (node_t *)_calloc(1, sizeof(node_t));
	if( p3 == NULL ) {
		uart_puts("p3, malloc error\n");
	}
	else {
		p2->next = p3;
		p3->data = 33;
		//p2->next = NULL;
	}
#endif
	free_nodes(p1);
	return 0;
}
