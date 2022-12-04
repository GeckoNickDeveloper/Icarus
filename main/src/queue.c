#include "../include/queue.h"
#include <stdlib.h>
#include <esp_log.h>

node_t* dequeue(queue_t* queue) {
	node_t* element;

	element = queue->head;
	if (queue->head != NULL) {
		queue->head = queue->head->next;

		if (queue->head == NULL)
			queue->tail = NULL;
	}

	return element;
};

void enqueue(queue_t* queue, const vector3d_t value) {
	node_t* new;

	new = (node_t*) malloc(sizeof(node_t));
	new->value = value;
	new->next = NULL;

	if (queue->tail != NULL)
		queue->tail->next = new;	
	else
		queue->head = new;

	queue->tail = new;
};

/*
bool create_queue(queue_t* queue) {
	bool result;
	
	queue = (queue_t*) malloc();
	queue->head = NULL;
	queue->tail = NULL;
	
	return result;
};

bool destroy_queue(queue_t* queue) {
	bool result;



	return result;
};*/