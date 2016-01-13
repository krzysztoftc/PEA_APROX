/*
 * MatrixCosts.h
 *
 *  Created on: 22 paź 2015
 *      Author: krzysztof
 */

#ifndef MATRIXCOSTS_H_
#define MATRIXCOSTS_H_

#include <iostream>
#include <vector>
#include <list>

class MatrixCosts {
	int **matrix;

	void create();

public:
	void remove();
	int size;
	MatrixCosts();
	void setCities(int cities);
	void readFile(std::string filename);
	void generate(int cities);
	int reduction();
	std::string toString();
	std::pair<int, int> maxMin();
	std::pair<int, int> removeEdge(std::pair<int, int> vert,
			std::vector<int> &color);
	std::pair<int, int> findLast();
	std::vector<std::pair<int, int> > findLasts();
	int blockEdge(std::pair<int, int> vert);
	MatrixCosts(const MatrixCosts &matrixCosts);
	int getSize();
	int getCost(std::pair<int, int> edge);
	int getCost(int r, int c);
	MatrixCosts& operator=(MatrixCosts &m);
	void remove(std::pair<int, int> toRem);
	virtual ~MatrixCosts();
	std::pair<int, int> compute_edge_for_branching(
			std::pair<int, int> max_min, std::vector<int> const &colors);
	void generate_euclidean(unsigned vertices);
	std::list <std::pair<int, int> >mst_kruskal() const;
};

#endif /* MATRIXCOSTS_H_ */
