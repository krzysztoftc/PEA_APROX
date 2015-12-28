/*
 * SalesMan.cpp
 *
 *  Created on: 22 paź 2015
 *      Author: krzysztof
 */

#include "SalesMan.h"
#include <algorithm>
#include <list>
#include <climits>
#include <queue>
#include <vector>
#include <exception>
#include "SolutionNode.h"
#include "Solutions.h"

using namespace std;

SalesMan::SalesMan() {
	// TODO Auto-generated constructor stub

}

SalesMan::~SalesMan() {
	// TODO Auto-generated destructor stub
}

void SalesMan::readFile(string filename) {
	matrix.readFile(filename);
}

string SalesMan::toString() {
	return matrix.toString();
}

void SalesMan::generate(int cities) {
	matrix.generate(cities);
}

Solution SalesMan::complete() {
	int cities = matrix.getSize();
	int *p = new int[cities];

	for (int i = 0; i < cities; i++) {
		p[i] = i;
	}

	list<int> currentTrace;
	list<int> minTrace;

	int currentCost = 0;
	int minCost = INT_MAX;

	do {
		currentCost = 0;
		currentTrace.clear();
		currentTrace.push_back(p[0]);
		for (int i = 1; i < cities; i++) {
			currentCost += matrix.getCost(
					pair<int, int>(currentTrace.back(), p[i]));
			currentTrace.push_back(p[i]);
		}
		currentCost += matrix.getCost(
				pair<int, int>(currentTrace.back(), p[0]));
		currentTrace.push_back(p[0]);

		if (currentCost < minCost) {
			minTrace = currentTrace;
			minCost = currentCost;
		}

	} while (next_permutation(p, p + cities));

	Solution s;

	s.trace = minTrace;
	s.cost = minCost;
	delete[] p;
	return s;
}

Solution SalesMan::branchAndBound() {
	Solution s;
	SolutionNode bestSolution;
	bestSolution.lowBound = INT_MAX;
	bool selected = false;

	priority_queue<SolutionNode> queue;

	SolutionNode root;
	MatrixCosts ms = matrix;
	root.lowBound = 0;
	root.matrix = matrix;
	root.mSize = root.matrix.size;
	int lb = root.matrix.reduction();
	root.lowBound = lb;

	for (int i = 0; i < root.matrix.size; i++) {
		root.colors.push_back(i);
	}

	queue.push(root);
	SolutionNode tmp;
	while (!queue.empty()) {
		SolutionNode node = queue.top();

		queue.pop();

		selected = true;
		while (selected) {
			if (node.lowBound >= bestSolution.lowBound) {
				selected = false;
				continue;
			}

			if (node.mSize < 3) {
				selected = false;
				node.lowBound += node.matrix.reduction();

				vector<pair<int, int> > lasts = node.matrix.findLasts();

				bool last_one_choosen = true;
				for (unsigned int i = 0; i < lasts.size(); i++) {
					int begin = lasts[i].first;
					int end = lasts[i].second;

					if (node.colors[begin] == node.colors[end]
							&& last_one_choosen) {
						continue;
					}

					node.edges.push_back(lasts[i]);

				}

				if (node.lowBound < bestSolution.lowBound
						&& node.edges.size() == node.matrix.size) {
					bestSolution = node;

				}
				selected = false;

			} else {
				SolutionNode left = node;

				SolutionNode right = node;

				pair<int, int> best = node.matrix.maxMin();

				if (best.first == 0 && best.second == 0) {
					selected = false;
					break;
				}

				pair<int, int> branching_edge =
						left.matrix.compute_edge_for_branching(best,
								node.colors);

				if (branching_edge.first == 0 && branching_edge.second == 0) {
					selected = false;
					break;
				}
				//tworzenie lewego syna - usuwanie krawedzi
				std::pair<int, int> edge = left.matrix.removeEdge(
						branching_edge, left.colors);

				if (edge.first == 0 && edge.second == 0) {
					selected = false;
					break;
				}

				left.edges.push_back(edge);
				left.mSize--;

				left.lowBound += left.matrix.reduction();

				//blokowanie krawedzi w prawym synu
				right.matrix.blockEdge(branching_edge);
				right.lowBound += right.matrix.reduction();

				node = left;
				selected =
						(node.lowBound < bestSolution.lowBound) ? true : false;

				if (right.lowBound < bestSolution.lowBound)
					queue.push(right);

			}
		}
	}
	list<pair<int, int> >::iterator it = bestSolution.edges.begin();

	vector<pair<int, int> > sorted_edges;
	copy(bestSolution.edges.begin(), bestSolution.edges.end(),
			back_inserter(sorted_edges));
	sort(sorted_edges.begin(), sorted_edges.end());

	s.cost = 0;

	int p = 0;
	s.trace.push_back(0);

	for (int i = 0; i < ms.size; i++) {
		int second = sorted_edges[p].second;
		s.trace.push_back(second);
		s.cost += ms.getCost(pair<int, int>(p, second));
		p = second;

	}

	return s;
}

pair<int, int> SalesMan::min_of_pair(vector<pair<int, int> > table_of_pairs) {
	int min_value = INT_MAX;
	pair<int, int> min_pair(0, 0);

	vector<pair<int, int> >::iterator i = table_of_pairs.begin();

	while (i != table_of_pairs.end()) {
		if (i->first < min_value) {
			min_value = i->first;
			min_pair = *i;
		}
		++i;
	}

	return min_pair;
}

pair<int, int> SalesMan::check_solve(int c, const set<int> &ts,
		const vector<Solutions> &solves) {

	vector<Solutions>::const_iterator it;
	cout << "Solves begin size: " << solves.size();
	for (it = solves.begin(); it != solves.end(); it++) {
		Solutions s = *it;
		cout << "\ns.last city: " << s.last_city;

		if (c == s.last_city && ts == s.set) {
			cout << "\nret nie nil\n";
			return s.last;
		}
	}

	cout << "\nret nil\n";
	return pair<int, int>(-1, -1);
}

pair<int, int> SalesMan::tsp_divide(int c, set<int> &ts,
		vector<Solutions> &solves) {
	cout << "tsp_divide\n";

	if (ts.find(c) != ts.end()) {
		cout << "\nCos jest nie tak\n";
		exit(150);
	}

	if (!ts.empty()) {
		vector<pair<int, int> > sub_problem;

		int lc;
		for (lc = 0; lc < ts.size(); lc++) {
			int distance = matrix.getCost(lc, c);
			set<int> ts2;
			ts2.insert(ts.begin(), ts.end());

			cout << "\nlc: " << lc << " TS:\n";
			for (set<int>::iterator it = ts.begin(); it != ts.end(); it++) {
				cout << *it << " ";
			}

			cout << "\nTS2:\n";
			for (set<int>::iterator it = ts2.begin(); it != ts2.end(); it++) {
				cout << *it << " ";
			}

			ts2.erase(lc);

			cout << "\nTS2 after erase:\n";
			for (set<int>::iterator it = ts2.begin(); it != ts2.end(); it++) {
				cout << *it << " ";
			}

			pair<int, int> solve = check_solve(c, ts, solves);

			if (!(solve.first == -1 && solve.second == -1)) {
				return solve;
			}

			sub_problem.push_back(
					pair<int, int>(
							(distance + tsp_divide(lc, ts2, solves).first),
							lc));
		}

		pair<int, int> min = min_of_pair(sub_problem);

		Solutions s;
		s.last_city = c;
		s.last = min;
		s.set = ts;

		solves.push_back(s);
		static int b;
		cout << "\nbreak b " << ++b;
		return min;
	}

	return pair<int, int>(matrix.getCost(0, c), 0);
}

vector<int> SalesMan::rtsp() {

	vector<int> best_solution;
	vector<Solutions> solves;

	int c = 0;

	set<int> cs;

	for (int i = 1; i < matrix.size; i++) {
		cs.insert(i);
	}
	cout << "Break 1\n";
	int n = 1;
	while (1) {
		cout << "Break " << (++n) << endl;
		int lc = tsp_divide(c, cs, solves).second;
		cout << "wyszlo z tsp_divide\n";
		if (lc == 0)
			break;
		best_solution.push_back(lc);
		c = lc;
		cs.erase(lc);
	}

	return best_solution;
}

Solution SalesMan::dynamic() {
	vector<int> trace = rtsp();
	cout << "done";
	reverse(trace.begin(), trace.end());
	Solution toRet;
	toRet.trace = list<int>(trace.begin(), trace.end());
	toRet.cost = 0;

	for (vector<int>::iterator it = trace.begin() + 1; it != trace.end();
			it++) {
		toRet.cost += matrix.getCost(*(it - 1), *it);
	}
	toRet.cost += matrix.getCost(*(trace.end() - 1), *trace.begin());

	return toRet;
}
