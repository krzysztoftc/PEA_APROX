/*
 * SalesMan.h
 *
 *  Created on: 22 paź 2015
 *      Author: krzysztof
 */

#ifndef SALESMAN_H_
#define SALESMAN_H_

#include "MatrixCosts.h"
#include "Solution.h"
#include <vector>

#include "Solutions.h"

class SalesMan {
	MatrixCosts matrix;

public:
	SalesMan();
	void readFile(std::string filename);
	std::string toString();
	void generate(int cities);
	Solution branchAndBound();
	Solution complete();

	//just for dynamic algo
	std::vector<int> rtsp();
	std::pair<int, int> min_of_pair(std::vector<std::pair<int, int> >);
	std::pair<int, int> check_solve(int c, const std::set<int> &ts,
			const std::vector<Solutions> &solves);
	std::pair<int, int> tsp_divide(int c, std::set<int> &ts,
			std::vector<Solutions> &solves);
	Solution dynamic();

	virtual ~SalesMan();
};
#endif /* SALESMAN_H_ */
