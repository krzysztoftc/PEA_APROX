/*
 * MatrixCosts.cpp
 *
 *  Created on: 22 paź 2015
 *      Author: krzysztof
 */

#include "MatrixCosts.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <limits.h>
#include <cstring>

using namespace std;

MatrixCosts::MatrixCosts() {
	matrix = 0;
	size = 0;
}

MatrixCosts::~MatrixCosts() {
	remove();
}

void MatrixCosts::create() {
	if (size == 0)
		return;

	remove();

	matrix = new int*[size]();

	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size]();
	}

}

void MatrixCosts::remove() {
	if (matrix) {
		for (int i = 0; i < size; i++) {
			if (matrix[i])
				delete[] matrix[i], matrix[i] = 0;
		}

		delete[] matrix;
		matrix = 0;
	}
}

void MatrixCosts::readFile(string filename) {
	fstream input;
	input.open(filename.c_str(), ios::in);
	if (input.good()) {
		int in;

		remove();

		input >> in;
		size = in;

		create();

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				input >> in;
				matrix[i][j] = in;
			}
		}
	} else {
		cout << "\nBlad wczytania pliku!" << endl;

	}
	input.close();
}

string MatrixCosts::toString() {
	string toRet = "";
	toRet += "size: ";
	stringstream converter;
	converter << size;
	toRet += converter.str() + "\n\t";

	for (int j = 0; j < size; j++) {
		stringstream converter;
		converter << j;
		toRet += converter.str() + "\t";
	}

	for (int i = 0; i < size; i++) {

		toRet += "\n";
		stringstream converter;
		converter << i;
		toRet += converter.str() + "\t";

		for (int j = 0; j < size; j++) {
			stringstream converter;
			converter << matrix[i][j];
			toRet += converter.str() + "\t";
		}

	}
	toRet += "\n";
	return toRet;
}

void MatrixCosts::generate(int cities) {
	remove();

	size = cities;

	create();

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j)
				matrix[i][j] = -1;
			else {
				matrix[i][j] = 1 + rand() % 50;
//				matrix[j][i] = matrix[i][j];
			}
		}
	}
}

int MatrixCosts::reduction() {
	int bound = 0;
	int min;

	//redukcja wierszy
	for (int i = 0; i < size; i++) {
		min = INT_MAX;

		//szukanie min
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] == 0) {
				min = 0;
				break;
			}
			if (matrix[i][j] > 0 && matrix[i][j] < min)
				min = matrix[i][j];
		}

		//redukcja
		if (min != INT_MAX && min > 0)
			bound += min;
		if (min != INT_MAX && min > 0) {
			for (int j = 0; j < size; j++) {
				if (matrix[i][j] > 0) {
					matrix[i][j] -= min;
				}
			}
		}
	}

	//redukcja kolumn
	for (int i = 0; i < size; i++) {
		min = INT_MAX;
		//szukanie min
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] == 0) {
				min = 0;
				break;
			}

			if (matrix[j][i] > 0 && matrix[j][i] < min)
				min = matrix[j][i];
		}

//redukcja
		if (min != INT_MAX && min > 0)
			bound += min;
		if (min != INT_MAX && min > 0) {
			for (int j = 0; j < size; j++) {
				if (matrix[j][i] > 0) {
					matrix[j][i] -= min;

				}
			}
		}
	}

	return bound;
}

//wybiera najwieksza sposrod minimalnych wartosci w wierszach i kolumnach
//zwraca pare <wiersz, kolumna> (indeksy macierzy)
pair<int, int> MatrixCosts::maxMin() {
	pair<int, int> toRet;
	pair<int, int> current;

	int currentMin = INT_MAX;
	int best = -1;

	//przeglad po wierszach
	for (int i = 0; i < size; i++) {
		bool zero = false;
		currentMin = INT_MAX;
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] == 0) {
				//jesli sa przynajmniej dwa zera, to mozemy je wybrac
				if (zero == true) {
					current.first = i;
					current.second = j;
					currentMin = 0;
				}

				else {
					zero = true;
				}
			}
			//wybor najlepszego w danym wierszu
			if (matrix[i][j] > 0) {
				if (currentMin > matrix[i][j]) {
					currentMin = matrix[i][j];
					current.first = i;
					current.second = j;
				}
			}

		}

		//wybor najlepszego we wszystkich wierszach
		if (currentMin != INT_MAX && best < currentMin) {
			best = currentMin;
			toRet = current;
		}
	}

	//przeglad po kolumnach
	for (int i = 0; i < size; i++) {
		bool zero = false;
		currentMin = INT_MAX;
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] == 0) {
				//jesli sa przynajmniej dwa zera, to mozemy je wybrac
				if (zero == true) {
					current.first = j;
					current.second = i;
					currentMin = 0;
					//return toRet;
				}

				else {
					zero = true;
				}
			}
			//wybor najlepszego w danym wierszu
			if (matrix[j][i] > 0) {
				if (currentMin > matrix[j][i]) {
					currentMin = matrix[j][i];
					current.first = j;
					current.second = i;
				}
			}
		}
		//wybor najlepszego we wszystkich wierszach
		if (currentMin != INT_MAX && best < currentMin) {
			best = currentMin;
			toRet = current;
		}
	}

	return toRet;
}

//Funkcja usuwa luk "wokol" zadanego wierzcholka
pair<int, int> MatrixCosts::removeEdge(pair<int, int> vert,
		std::vector<int> &color) {

	int row = vert.first;
	int col = vert.second;

	int color_end = color[col];

	for (int i = 0; i < color.size(); i++) {
		if (color[i] == color_end)
			color[i] = color[row];
	}

	color[col] = color[row];
	matrix[col][row] = -1;

	//usuwanie - wpisywanie inf
	for (int i = 0; i < size; i++) {
		//w wybranym wierszu
		matrix[row][i] = -1;
		//w wybranej kolumnie
		matrix[i][col] = -1;
	}
	return pair<int, int>(row, col);
}

int MatrixCosts::blockEdge(pair<int, int> vert) {
	//wybor luku - wiersza, kolumna wg zadanego w wierzcholku
	int row = vert.first;
	int col = vert.second;

	//blokowanie luku
	matrix[row][col] = -1;

	return 0;
}

pair<int, int> MatrixCosts::compute_edge_for_branching(pair<int, int> max_min,
		vector<int> const &colors) {
	pair<int, int> to_ret(0, 0);

	int row = max_min.first;
	int col = max_min.second;

	if (!(matrix[row][col] == 0))
		for (row = 0;
				row < size
						&& (matrix[row][col] != 0 || colors[row] == colors[col]);
				row++)
			;

	if (row == size) {
		row = max_min.first;
		if (!(matrix[row][col] == 0))
			for (col = 0;
					col < size
							&& (matrix[row][col] != 0
									|| colors[row] == colors[col]); col++)
				;
	}

	if(row >= size || col >= size) return to_ret;
	to_ret.first = row;
	to_ret.second = col;

	return to_ret;
}

MatrixCosts::MatrixCosts(const MatrixCosts &matrixCosts) {

	this->size = matrixCosts.size;

	matrix = new int*[size];

	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size];
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i][j] = matrixCosts.matrix[i][j];
		}
	}
}

int MatrixCosts::getSize() {
	return size;
}

MatrixCosts& MatrixCosts::operator=(MatrixCosts &m) {
	swap(this->matrix, m.matrix);
	swap(this->size, m.size);
	return *this;
}

int MatrixCosts::getCost(pair<int, int> edge) {
	return matrix[edge.first][edge.second];
}

int MatrixCosts::getCost(int r, int c) {
	return matrix[r][c];
}

std::pair<int, int> MatrixCosts::findLast() {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] == 0) {
				return pair<int, int>(i, j);
			}
		}
	}

	//jesli nie znjadzie nic odpowiedniego to zwraca
	//nieprawidlowa pare (-1,-1)
	return pair<int, int>(-1, -1);
}

std::vector<std::pair<int, int> > MatrixCosts::findLasts() {
	std::vector<std::pair<int, int> > toRet;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] == 0) {
				toRet.push_back(std::pair<int, int>(i, j));
			}
		}
	}

	return toRet;
}

