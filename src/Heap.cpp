/*
 * Heap.cpp
 *      Author: krzysztof
 */

#include "Heap.h"
#include <iostream>

Heap::Heap() {
	// TODO Auto-generated constructor stub
	tab = new Edge[10];
	elements = 0;
	size = 10;
}

void Heap::rebuildUp() {
	for (int i = elements; i > 0; i = (i - 1) / 2) {
		if ((tab + i)->wage < (tab + ((i - 1) / 2))->wage) {
			swap(*(tab + ((i - 1) / 2)), *(tab + i));
		} else
			break;
	}
}

void Heap::rebuildDown() {
	int i = 0;
	while ((2 * i + 2) < elements) {
		if ((tab + i)->wage > (tab + (2 * i + 1))->wage
				|| (tab + i)->wage > (tab + (2 * i + 2))->wage) {
			if ((tab + (2 * i + 1))->wage < (tab + (2 * i + 2))->wage) {
				swap(*(tab+i), *(tab+(2 * i + 1)));
				i = (2 * i + 1);
			} else {
				swap(*(tab+i), *(tab+(2 * i + 2)));
				i = (2 * i + 2);
			}
		} else
			break;
	}
}

void Heap::swap(Edge &a, Edge &b) {
	Edge temp = a;
	a = b;
	b = temp;
}

void Heap::print() const{
	std::cout<<"\n-------\n";
	for (int i =0; i<elements;i++){
		std::cout<<tab[i].begin<<" "<<tab[i].wage<<" "<<tab[i].end<<std::endl;
	}
}

void Heap::add(Edge value) {
	if (elements + 1 == size) {
		Edge *temp = new Edge[size * 2];
		for (int i = 0; i < size; i++) {
			*(temp + i) = *(tab + i);
		}
		delete[] tab;
		tab = temp;
		size = size * 2;
	}
	*(tab + elements) = value;
	rebuildUp();
	elements++;
}

Edge Heap::remove() {
	if (elements) {
		Edge pop = *tab;
		swap(*tab, *(tab + (--elements)));
		rebuildDown();
		return pop;
	}
	return Edge();
}

Edge Heap::getMin() {
	return tab[0];
}

void Heap::clear() {
	delete[] tab;
	size = 10;
	elements = 0;
	tab = new Edge[size];
}

Heap::~Heap() {
// TODO Auto-generated destructor stub
	clear();
	delete[] tab;
	tab = 0;
}

