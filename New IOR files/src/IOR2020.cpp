#pragma comment(linker, "/STACK:10000000000")

//============================================================================
// Name        : 2020.cpp
// Author      : ArinaK
// Version     :
// Copyright   : It is working !!!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <bitset>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <string.h>
#include <utility>


using namespace std;

struct finishTop {
	int matrix [3][3] = {0, 0, 0};
};

int main() {

//=====================/ Создание всех стартовых расстановок /=====================

	fstream myfile;
	myfile.open("1.txt");
	vector<unsigned long long> start_pos_level_1 (0);
		for (int i = 1; i <= 5; ++i) {
			for (int j = 1; j <= 5; ++j) {
				if (i != j) { for (int k = 1; k <= 5; ++k) {
					if ((i != k) && (j != k)) { for (int l = 1; l <= 5; ++l) {
						if ((i != l)&&(j != l)&&(k != l)) { for (int m = 1; m <= 5; ++m) {
							if ((i != m)&&(j != m)&&(k != m)&&(l != m)){
								start_pos_level_1.push_back(i * 10000 + j * 1000 + k * 100 + l * 10 + m);
								//cout << "00000" << endl << start_pos_level_1[start_pos_level_1.size() - 1] << endl << endl;
							}
						}}
					}}
				}}
			}
		}
		unsigned long long counter = 0;
		for (int i = 0; i < start_pos_level_1.size(); ++i) {
			for (int j = 0; j < start_pos_level_1.size(); ++j) {
				myfile << "level 2 : " << start_pos_level_1[j] << endl;
				myfile << "level 1 : " << start_pos_level_1[i] << endl;
				myfile << "================" << endl;
				counter++;
			}
		}
	cout << "Всего стартовых расстановок : " << counter << endl;
	cout << "working...." << endl; // prints working....
	myfile.close();

//=====================/ Создание всех финешных расстановок /=====================

	vector<unsigned long long> finish_tops(0);

/*
 * 	1-5 - Э
 *	6-10 - Ш
 */

	finishTop tmp_finish_top;
	short int buinding = 0, level =0;

	// TODO: создание и вывод в файл вариантов схем построек


	return 0;
}
