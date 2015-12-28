//============================================================================
// Name        : TSP_BaB.cpp
// Author      : Kinga & Krzysztof
// Version     :
// Copyright   : Copyright 2015: Kinga Wilczek 210063 & Krzysztof Cabala 210047
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "SalesMan.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

void test_poprawnosci() {
	SalesMan seller;

	int rozwiazanie;
	Solution s, s1;
	int i = 0;
	do {
		seller.generate(8);

		cout << seller.toString();

		list<int>::iterator it;

		s = seller.complete();

//		it = s.trace.begin();
//		cout << endl << *it++;
//		while (it != s.trace.end()) {
//			cout << " -> " << *it++;
//		}
//		cout << endl << "cost: " << s.cost << endl;
		rozwiazanie = s.cost;

		s1 = seller.branchAndBound();
		cout << "\nZ przegaldu: ";
//		it = s.trace.begin();
//		cout << endl << *it++;
//		it = s.trace.begin();
//		cout << endl << *it++;
//		while (it != s.trace.end()) {
//			cout << " -> " << *it++;
//		}
		cout << endl << "cost: " << s.cost << endl;

		cout << "\nZ BB: ";

//		it = s1.trace.begin();
//		cout << endl << *it++;
//		it = s1.trace.begin();
//		cout << endl << *it++;
//		while (it != s1.trace.end()) {
//			cout << " -> " << *it++;
//		}
		cout << endl << "cost: " << s1.cost << endl;
		cout
				<< "===================================================================Poprawnych bylo: "
				<< i++ << "\n\n";
	} while (rozwiazanie == s.cost);

}

void test_wydajnosci() {
	const int sizes_exhausted = 7;
	int size_exhausted[sizes_exhausted] = { 4, 5, 6, 7, 8, 9, 10 };

	const int sizes_bb = 14;
	int size_bb[sizes_bb] = { 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,
			30 };
	int repeats = 100;
	unsigned begin, end;

	unsigned exhausted, bb;
	SalesMan seller;

	fstream out;
	out.open("output.txt", ios::out);
	out << "rozmiar;zupelny" << endl;
	Solution s;

	cout << "\nSTART bb\n";
	for (int i = 0; i < sizes_bb; i++) {
		cout << size_bb[i] << "......" << endl;
		bb = 0;
		out << size_bb[i] << ";";
		for (int j = 0; j < repeats; j++) {
			cout << j << "\n";
			seller.generate(size_bb[i]);
			begin = clock();
			seller.branchAndBound();
			end = clock();
			bb += (end - begin);

		}
		out << (double) bb / (double) repeats / (double) CLOCKS_PER_SEC << endl;
		cout << " done\n";
	}

	out << "rozmiar;zupelny" << endl;
	cout << "\nSTART Przeglad\n";
	for (int i = 0; i < sizes_exhausted; i++) {
		cout << size_exhausted[i] << "......" << endl;

		exhausted = 0;
		out << size_exhausted[i] << ";";
		for (int j = 0; j < repeats; j++) {
			cout << j << endl;
			seller.generate(size_exhausted[i]);
			cout << j << endl;
			begin = clock();
			seller.complete();
			end = clock();
			exhausted += (end - begin);
		}
		cout << endl;
		out << (double) exhausted / (double) repeats / (double) CLOCKS_PER_SEC;
		out << endl;
	}

	out << "=============================\nrozmiar;bb" << endl;

	out.close();
}

void show_solution(Solution &s) {
	list<int>::iterator it = s.trace.begin();
	cout << endl << *it++;
	while (it != s.trace.end()) {
		cout << " -> " << *it++;
	}
	cout << endl << "cost: " << s.cost << endl;
}

void menu() {
	char wybor = '0';
	SalesMan seller;
	Solution s;

	while (1) {
		printf("\033c");
		cout << "==========================================================\n"
				<< "=   PROJEKTOWANIE EFEKTYWNYCH ALGORYTMOW LABORATORIUM 1  =\n"
				<< "=   Kinga Wilczek        210063                          =\n"
				<< "=   Krzysztof Cabala     210047                          =\n"
				<< "==========================================================\n\n";
		switch (wybor) {
		case '0': {
			cout << "MENU:\n" << "1. Wyswietl graf\n" << "2. Generuj graf\n"
					<< "3. Wczytaj plik\n" << "4. Przeglad zupelny\n"
					<< "5. Branch & Bound\n" << "6. Koniec\n" << ">";

			system("stty raw isig");
			wybor = getchar();
			system("stty cooked");

			break;
		}

		case '1': {
			cout << "\nMacierz grafu: \n" << seller.toString()
					<< "\n\nWcisnij dowolny przycisk...";

			system("stty raw isig");
			getchar();
			system("stty cooked");
			wybor = '0';

			break;
		}

		case '2': {
			cout << "\nPodaj ilosc wierzcholkow: \n";
			int n;
			cin >> n;

			seller.generate(n);

			wybor = '0';

			break;
		}

		case '3': {
			cout << "\nPodaj sciezke do pliku: \n";
			string path;
			cin >> path;

			seller.readFile(path);
			cout << "\n\nWcisnij dowolny przycisk...";
			system("stty raw isig");
						getchar();
						system("stty cooked");
			wybor = '0';

			break;
		}

		case '4': {
			unsigned begin, end;
			Solution s;

			cout << "\nCzas dzialania: \n";

			begin = clock();
			s = seller.complete();
			end = clock();

			cout << (double) (end - begin) / CLOCKS_PER_SEC * 1000;

			cout << " ms\nRozwiazanie:\n";
			show_solution(s);
			cout << "\n\nWcisnij dowolny przycisk...";

			system("stty raw isig");
			getchar();
			system("stty cooked");
			wybor = '0';

			break;
		}

		case '5': {
			unsigned begin, end;
			Solution s;
			SalesMan tmp = seller;
			cout << "\nCzas dzialania: \n";

			begin = clock();
			s = tmp.branchAndBound();
			end = clock();

			cout << (double) (end - begin) / CLOCKS_PER_SEC * 1000;

			cout << " ms\nRozwiazanie:\n";
			show_solution(s);
			cout << "\n\nWcisnij dowolny przycisk...";

			system("stty raw isig");
			getchar();
			system("stty cooked");
			wybor = '0';

			break;
		}

		case '6': {
			return;
		}

		case '9': {
			seller.readFile("tsp10.txt");
			wybor = '0';
			break;
		}

		case '8': {
			seller.readFile("tsp4.txt");
			wybor = '0';
			break;
		}

		default: {
			wybor = '0';
			break;
		}

		}
	}

}

int main() {
	srand(time(0));

//	menu();

	SalesMan sales;
	sales.readFile("tsp4.txt");

	cout<<"\nDynamic:\n";
	Solution s2 = sales.dynamic();
	show_solution(s2);

	cout<<"\nB&B:\n";
	Solution s = sales.branchAndBound();
	show_solution(s);






	return 0;
}
