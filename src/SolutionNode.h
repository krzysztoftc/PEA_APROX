/*
 * SolutionNode.h
 *
 *  Created on: 22 paź 2015
 *      Author: krzysztof
 */

#ifndef SOLUTIONNODE_H_
#define SOLUTIONNODE_H_

#include <list>
#include "MatrixCosts.h"

struct SolutionNode {
	MatrixCosts matrix;
	int lowBound;
	std::list<std::pair<int, int> > edges;
	std::vector<int> colors;
	int mSize;


	bool operator>(const SolutionNode& r) const
	{
		return this->lowBound < r.lowBound;
	}
	bool operator<(const SolutionNode& r) const
	{
		return this->lowBound > r.lowBound;
	}

};

#endif /* SOLUTIONNODE_H_ */
