/*
 * Edge.h
 *
 *  Created on: 1 maj 2015
 *      Author: krzysztof
 */

#ifndef EDGE_H_
#define EDGE_H_

struct Edge {
public:
	int begin;
	int end;
	int wage;
	bool operator ==(const Edge &e);
	Edge(int begin, int wage, int end);
	Edge();
	~Edge();
};

#endif /* EDGE_H_ */
