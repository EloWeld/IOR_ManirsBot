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
#include <cstring>
#include <stdint.h>
#include <bitset>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <string.h>
#include <utility>


using namespace std;

//==================/ Обозначения цветов объектов разных типов /===================

											// ASCII
#define empty 'x'							//   120
#define first_color_of_big_blocks   0		//    48
#define last_color_of_big_blocks    4		//	  52
#define	first_color_of_small_blocks 5		//	  53
#define	last_color_of_small_blocks  9		//    57
#define max_num_of_used_blocks		9

//=============================/ Рабочие структурки /==============================

struct FinishTop {		// 00   01   02   10   11   12   20   21   22
	char buildings [max_num_of_used_blocks] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
};

//======================/ Инициализация "рабочих" функций /========================
// TODO:

FinishTop creatFirsLevelForFinishTops(int a, int b, int c);
int typeCheckForFinishTops(FinishTop top, int level, int pos);
bool blockIsNotUsed(FinishTop top, int new_block);
string FinishTopToStr(FinishTop top);



//==================================/  MAIN  /=====================================

int main() {

//=====================/ Создание всех стартовых расстановок /=====================

	fstream myfile;
	myfile.open("1.txt");
	vector<unsigned long long> start_pos_level_1 (0);
		for (short int i = first_color_of_big_blocks; i <= 4; ++i) {
			for (short int j = first_color_of_big_blocks; j <= 4; ++j) {
				if (i != j) { for (short int k = first_color_of_big_blocks; k <= 4; ++k) {
					if ((i != k) && (j != k)) { for (short int l = first_color_of_big_blocks; l <= 4; ++l) {
						if ((i != l)&&(j != l)&&(k != l)) { for (short int m = first_color_of_big_blocks; m <= 4; ++m) {
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
				start_pos_level_1[j] > 9999 ? myfile << "level 2 : " << start_pos_level_1[j] << endl : myfile << "level 2 : 0" << start_pos_level_1[j] << endl;
				start_pos_level_1[i] > 9999 ? myfile << "level 1 : " << start_pos_level_1[i] << endl : myfile << "level 1 : 0" << start_pos_level_1[i] << endl;
				myfile << "================" << endl;
				counter++;
			}
		}
	cout << "Всего стартовых расстановок : " << counter << endl;
	myfile.close();

//=====================/ Создание всех финешных расстановок /=====================

/*	x - пустота
 * 	0-4 - этаж
 *	5-9 - шпиль
 *
 *  0     1		2
 *  1э    2э    3э
 * 012 | 345 | 678
 */

//======================/ Создание 1-х этажей схем построек/======================

	vector<FinishTop> first_level_finish_tops(0);
	vector<FinishTop> level_1_and_2_finish_tops(0);
	vector<FinishTop> all_finish_tops(0);
	myfile.open("2.txt");

	//От 0 до 9 т.к. в каждой зоне есть минимум 1 блок
	for (short int i = first_color_of_big_blocks; i <= last_color_of_small_blocks; i++) {							// 1-й этаж	1-я зона
		for (short int j = first_color_of_big_blocks; j <= last_color_of_small_blocks; j++) {						// 1-й этаж	2-я зона
			if (i != j) { for (short int k = first_color_of_big_blocks; k <= last_color_of_small_blocks; k++) {	// 1-й этаж	3-я зона
				if ((i != k) && (j != k)) {
					first_level_finish_tops.push_back(creatFirsLevelForFinishTops(i, j, k));
					myfile << first_level_finish_tops[first_level_finish_tops.size() - 1].buildings[0];
					myfile << first_level_finish_tops[first_level_finish_tops.size() - 1].buildings[1];
					myfile << first_level_finish_tops[first_level_finish_tops.size() - 1].buildings[2] << endl;
				}
			}}
		}
	}
	myfile.close();

//======================/ Создание 2-х этажей схем построек/======================
// TODO:

//	...
	myfile.open("3.txt");

	for (int i = 0; i < first_level_finish_tops.size(); i++) {														// все 1-е этажи
		short int count_of_spiers = 0;
				for (short int j = 0; j < 3; j++) {																	// все постройки (0, 1, 2)
					if ((typeCheckForFinishTops(first_level_finish_tops[i], 0, j) != 0)) {							// если не этаж, то над ним х
						count_of_spiers++;
						if (count_of_spiers == 3) all_finish_tops.push_back(first_level_finish_tops[i]);
						// else { cout << "Жизнь - боль, смотрим дальше..." << endl; }
					} else {
						// надо добавить в .. все возможные надстройки над этим этажем, причем так чтобы следующий 1е этаж
	//					for (int k = 0; k < last_color_of_small_blocks; k ++) {					// заполнение
	//						if (blockIsNotUsed(first_level_finish_tops[i], k)) {
	//							first_level_finish_tops[i].buildings[j + 3] = k + 48;
	//							myfile << "@ " << j << " "  <<  FinishTopToStr(first_level_finish_tops[i]) << endl;
	//						}
	//					}
					}
				}
			}

/*
	for (int i = 0; i < first_level_finish_tops.size(); i++) {														// все 1е этажи
			bool top_is_not_checked = 1;
			for (int j = 0; j < 3; j++) {																			// все постройки (0, 1, 2)
				if ((typeCheckForFinishTops(first_level_finish_tops[i], 0, j) != 0)&&(top_is_not_checked)) {		// если не этаж, то над ним х
					top_is_not_checked = 0;
					myfile << "$ " << j << " " << FinishTopToStr(first_level_finish_tops[i]) << endl;
					//level_1_and_2_finish_tops.push_back(first_level_finish_tops[i]);
				} else {
//					for (int k = 0; k < last_color_of_small_blocks; k ++) {					// заполнение
//						if (blockIsNotUsed(first_level_finish_tops[i], k)) {
//							first_level_finish_tops[i].buildings[j + 3] = k + 48;
//							myfile << "@ " << j << " "  <<  FinishTopToStr(first_level_finish_tops[i]) << endl;
//						}
//					}
				}
			}
		}


/*
	for (int i = 0; i < first_level_finish_tops.size(); i++) {							// все 1е этажи
		for (int j = 0; j < 3; j++) {													// все постройки (0, 1, 2)
			if (typeCheckForFinishTops(first_level_finish_tops[i], 0, j) != 0) {		// если не этаж, то над ним х
				myfile << FinishTopToStr(first_level_finish_tops[i]) << endl;
				break;
				//level_1_and_2_finish_tops.push_back(first_level_finish_tops[i]);
			} else {
//				for (int k = 0; k < last_color_of_small_blocks; k ++) {					// заполнение
//					if (blockIsNotUsed(first_level_finish_tops[i], k)) {
//						first_level_finish_tops[i].buildings[j + 3] = k + 48;
//
//					}
//				}
			}
		}
	}
*/
	myfile.close();



			/*	if ((i != k) && (j != k)) { for (int l = 1; l <= 10; l++) {								// 2-й этаж	1-я зона
					if ((i != l)&&(j != l)&&(k != l)) { for (int m = ; m <= 10; m++) {					// 2-й этаж	2-я зона
						if ((i != m)&&(j != m)&&(k != m)&&(l != m)) { for (int n = ; n <= 10; ++n) {	// 2-й этаж	3-я зона
							if ((i != n)&&(j != n)&&(k != n)&&(l != n)&&(m != n)) {

							}
						}}
					}}
				}}*/


	cout << "working...." << endl; // prints working....
	return 0;
}

/*
 * Думали конец, а нет!
 */

//========================/ Свалка всех имеемых фуекций /=========================
// TODO:

FinishTop creatFirsLevelForFinishTops(int a, int b, int c) {
	FinishTop tmp_finish_top = {(a + 48), (b + 48), (c + 48), 'x', 'x', 'x', 'x', 'x', 'x'};
	return tmp_finish_top;
}

int typeCheckForFinishTops(FinishTop top, int level, int pos) {
	int cheking = top.buildings[level * 3 + pos];
	if (cheking == 120)return -1; 					// пустота
	if ((cheking >= 48)&&(cheking <= 52))return 0;  // этаж
	if ((cheking >= 53)&&(cheking <= 57))return 1;  // шпиль
}

bool blockIsNotUsed(FinishTop top, short int new_block) {
	char char_block = new_block + 48;
	for (short int i = 0; i < max_num_of_used_blocks; i++) {
		if (top.buildings[i] == char_block) return 0;
	}
	return 1;
}

string FinishTopToStr(FinishTop top) {
	string str_top = "";
	for (short int i = 0; i < max_num_of_used_blocks; i++) {
		str_top = (string) str_top + top.buildings[i];
	}
	return str_top;
}
