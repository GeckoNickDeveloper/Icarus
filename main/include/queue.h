#ifndef QUEUE_H
#define QUEUE_H

#include "shared.h"

typedef struct node {
	vector3d_t value; 
	struct node* next;
} node_t;

typedef struct queue {
	node_t* head;
	node_t* tail;
} queue_t;

extern node_t* dequeue(queue_t* queue);
extern void enqueue(queue_t* queue, const vector3d_t value);

//extern bool create_queue(queue_t* queue);
//extern bool destroy_queue(queue_t* queue);

#endif //! QUEUE_H