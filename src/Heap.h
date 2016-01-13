/*
 * Heap.h
 *
 *  Created on: 1 maj 2015
 *      Author: krzysztof
 */

#ifndef HEAP_H_
#define HEAP_H_

#include "Edge.h"

class Heap {
private:
	Edge *tab;
	int size;
	int elements;
	void swap(Edge &a, Edge &b);
	void rebuildDown();
	void rebuildUp();
public:
	Heap();
	void add(Edge value);
	void print()const;
	Edge remove();
	Edge getMin();
	void clear();

	virtual ~Heap();
};

#endif /* HEAP_H_ */
