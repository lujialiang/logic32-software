#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node_tag {
	int data;
	struct node_tag *next;
} node_t ;

void free_nodes(node_t *p)
{
	node_t *t;
	while(p) {
		t = p->next;
		printf("p:%p, d:%d\n", p, p->data);
		free(p);
		p = t;
	}
}

int main(int argc, char *argv[])
{
	node_t *p1;
	node_t *p2;
	p1 = (node_t *)calloc(1, sizeof(node_t));
	if( p1 == NULL ) {
		printf("p1, malloc error\n");
	}
	else {
		p1->data = 11;
		p1->next = NULL;
	}
	p2 = (node_t *)calloc(1, sizeof(node_t));
	if( p1 == NULL ) {
		printf("p2, malloc error\n");
	}
	else {
		p1->next = p2;
		p2->data = 22;
		p2->next = NULL;
	}
	free_nodes(p1);
	return 0;
}
