#include <stdio.h>

#include "uart.h"

typedef struct node_s {
	int data;
	struct node_s *next;
} node_t;

node_t head;

node_t *apped(node_t *n)
{
	node_t *p;
	p = &head;
	while (p->next) {
		p = p->next;
	}
	n->next = NULL;
	p->next = n;
	return n;
}

int sum(node_t *p)
{
	int ret = 0;
	while (p) {
		ret += p->data;
		p = p->next;
	}
	return ret;
}

int add(node_t n1, node_t n2)
{
	return n1.data + n2.data;
}

int main(void)
{
	int i;
	char buffer[1024];
	int result;

	node_t node1, node2, node3;

	head.data = 0;
	head.next = NULL;
	node1.data = 10;
	node2.data = 20;
	node3.data = 30;
	apped(&node1);
	apped(&node2);
	apped(&node3);

	result = sum(&head);

	sprintf(buffer, "result=%d", result);
	uart_puts(buffer);

	result = add(node1, node2);
	sprintf(buffer, "result=%d", result);
	uart_puts(buffer);

	for (i=0; i<10; i++) {
		sprintf(buffer, "hello, i=%d", i);
		uart_puts(buffer);
	}
	return 0;
}

