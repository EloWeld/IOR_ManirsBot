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

//==================/ ����������� ������ �������� ������ ����� /===================

											// ASCII
#define empty 'x'							//   120
#define first_color_of_big_blocks   0		//    48
#define last_color_of_big_blocks    4		//	  52
#define	first_color_of_small_blocks 5		//	  53
#define	last_color_of_small_blocks  9		//    57

//=============================/ ������� ���������� /==============================

struct FinishTop {
	char buildings [9] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
};

//======================/ ������������� "�������" ������� /========================
// TODO:

FinishTop creatFirsLevelForFinishTops(int a, int b, int c);
int typeCheckForFinishTops(FinishTop top, int level, int pos);



//==================================/  MAIN  /=====================================

int main() {

//=====================/ �������� ���� ��������� ����������� /=====================

	fstream myfile;
	myfile.open("1.txt");
	vector<unsigned long long> start_pos_level_1 (0);
		for (int i = first_color_of_big_blocks; i <= 4; ++i) {
			for (int j = first_color_of_big_blocks; j <= 4; ++j) {
				if (i != j) { for (int k = first_color_of_big_blocks; k <= 4; ++k) {
					if ((i != k) && (j != k)) { for (int l = first_color_of_big_blocks; l <= 4; ++l) {
						if ((i != l)&&(j != l)&&(k != l)) { for (int m = first_color_of_big_blocks; m <= 4; ++m) {
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
	cout << "����� ��������� ����������� : " << counter << endl;
	myfile.close();

//=====================/ �������� ���� �������� ����������� /=====================

/*	x - �������
 * 	0-4 - ����
 *	5-9 - �����
 *
 *  0     1		2
 *  1�    2�    3�
 * 012 | 345 | 678
 */

//======================/ �������� 1-� ������ ���� ��������/======================

	vector<FinishTop> first_level_finish_tops(0);
	myfile.open("2.txt");

	//�� 0 �� 9 �.�. � ������ ���� ���� ������� 1 ����
	for (int i = first_color_of_big_blocks; i <= last_color_of_small_blocks; i++) {							// 1-� ����	1-� ����
		for (int j = first_color_of_big_blocks; j <= last_color_of_small_blocks; j++) {						// 1-� ����	2-� ����
			if (i != j) { for (int k = first_color_of_big_blocks; k <= last_color_of_small_blocks; k++) {	// 1-� ����	3-� ����
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

//======================/ �������� 2-� ������ ���� ��������/======================
// TODO:

//	...
	if (typeCheckForFinishTops(first_level_finish_tops[/*...*/], 0, 0) != 0){}	// ���� �� ����, �� ��� ��� �




			/*	if ((i != k) && (j != k)) { for (int l = 1; l <= 10; l++) {								// 2-� ����	1-� ����
					if ((i != l)&&(j != l)&&(k != l)) { for (int m = ; m <= 10; m++) {					// 2-� ����	2-� ����
						if ((i != m)&&(j != m)&&(k != m)&&(l != m)) { for (int n = ; n <= 10; ++n) {	// 2-� ����	3-� ����
							if ((i != n)&&(j != n)&&(k != n)&&(l != n)&&(m != n)) {

							}
						}}
					}}
				}}*/


	cout << "working...." << endl; // prints working....
	return 0;
}

/*
 * ������ �����, � ���!
 */

//========================/ ������ ���� ������� ������� /=========================
// TODO:

FinishTop creatFirsLevelForFinishTops(int a, int b, int c) {
	FinishTop tmp_finish_top = {(a + 48), (b + 48), (c + 48), 'x', 'x', 'x', 'x', 'x', 'x'};
	return tmp_finish_top;
}

int typeCheckForFinishTops(FinishTop top, int level, int pos) {
	int cheking = top.buildings[level * 3 + pos];
	if (cheking == 120)return -1; 					// �������
	if ((cheking >= 48)&&(cheking <= 52))return 0;  // ����
	if ((cheking >= 53)&&(cheking <= 57))return 1;  // �����
}
