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

//==================/ ����������� ������ �������� ������ ����� /===================

											// ASCII
#define empty 'x'							//   120
#define first_color_of_big_blocks   0		//    48
#define last_color_of_big_blocks    4		//	  52
#define	first_color_of_small_blocks 5		//	  53
#define	last_color_of_small_blocks  9		//    57
#define max_num_of_used_blocks		9

//=============================/ ������� ���������� /==============================

struct FinishTop {		// 00   01   02   10   11   12   20   21   22
	char buildings [max_num_of_used_blocks] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
};

//======================/ ������������� "�������" ������� /========================
// TODO:

FinishTop creatFirsLevelForFinishTops(int a, int b, int c);
int typeCheckForFinishTops(FinishTop top, int level, int pos);
bool blockIsNotUsed(FinishTop top, int new_block);
string finishTopToStr(FinishTop top);



//==================================/  MAIN  /=====================================

int main() {

//=====================/ �������� ���� ��������� ����������� /=====================

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
		for (unsigned int i = 0; i < start_pos_level_1.size(); ++i) {
			for (unsigned int j = 0; j < start_pos_level_1.size(); ++j) {
				start_pos_level_1[j] > 9999 ? myfile << "level 2 : " << start_pos_level_1[j] << endl : myfile << "level 2 : 0" << start_pos_level_1[j] << endl;
				start_pos_level_1[i] > 9999 ? myfile << "level 1 : " << start_pos_level_1[i] << endl : myfile << "level 1 : 0" << start_pos_level_1[i] << endl;
				myfile << "================" << endl;
			}
		}
	cout << "����� ��������� ����������� : " << start_pos_level_1.size()*start_pos_level_1.size() << endl;
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
	for (short int i = first_color_of_big_blocks; i <= last_color_of_small_blocks; i++) {							// 1-� ����	1-� ����
		for (short int j = first_color_of_big_blocks; j <= last_color_of_small_blocks; j++) {						// 1-� ����	2-� ����
			if (i != j) { for (short int k = first_color_of_big_blocks; k <= last_color_of_small_blocks; k++) {		// 1-� ����	3-� ����
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

	myfile.open("3.txt");

//=====================/ ��������� 1-� ������� 2-��� ����� /======================

	vector<FinishTop> pos1_on_level2_finish_tops(0);

	for (unsigned int i = 0; i < first_level_finish_tops.size(); i++) {									// ��� 1-� ����� ��������
		if ((typeCheckForFinishTops(first_level_finish_tops[i], 0, 0) != 0)) {							// ���� �� ����, �� ��� ��� �
			pos1_on_level2_finish_tops.push_back(first_level_finish_tops[i]);							// ���� �������� � .. , ����� ��������� 1-� ���� ����������� �� ������ ..
		} else {																						// ���� ����� ���-�� ��������� �� ������ ����
			for (int k = 0; k <= last_color_of_small_blocks; k ++) {									// ����������
				if (blockIsNotUsed(first_level_finish_tops[i], k)) {
					first_level_finish_tops[i].buildings[3] = k + 48;
					//myfile << finishTopToStr(first_level_finish_tops[i]) << endl;
					pos1_on_level2_finish_tops.push_back(first_level_finish_tops[i]);
				}
			}
		}
	}

	first_level_finish_tops.resize(0);


//=====================/ ��������� 2-� ������� 2-��� ����� /======================

	vector<FinishTop> pos1_and_pos2_on_level2_finish_tops(0);

	for (unsigned int i = 0; i < pos1_on_level2_finish_tops.size(); i++) {								// ��� 1-� ����� ��������, � ������� �� 1-� ������� 2-��� ����� ���-�� ����
		if ((typeCheckForFinishTops(pos1_on_level2_finish_tops[i], 0, 1) != 0)) {						// ���� �� ����, �� ��� ��� �
			pos1_and_pos2_on_level2_finish_tops.push_back(pos1_on_level2_finish_tops[i]);
		} else {																						// ���� ����� ���-�� ��������� �� ������ ����
			for (int k = 0; k <= last_color_of_small_blocks; k ++) {									// ����������
				if (blockIsNotUsed(pos1_on_level2_finish_tops[i], k)) {
					pos1_on_level2_finish_tops[i].buildings[4] = k + 48;
					//myfile << finishTopToStr(pos1_on_level2_finish_tops[i]) << endl;
					pos1_and_pos2_on_level2_finish_tops.push_back(pos1_on_level2_finish_tops[i]);
				}
			}
		}
	}

	pos1_on_level2_finish_tops.resize(0);


//=====================/ ��������� 3-� ������� 2-��� ����� /======================

	vector<FinishTop> all_2_levels_finish_tops(0);

	for (unsigned int i = 0; i < pos1_and_pos2_on_level2_finish_tops.size(); i++) {						// ��� 1-� ����� ��������, � ������� �� 1-� ������� 2-��� ����� ���-�� ����
		if ((typeCheckForFinishTops(pos1_and_pos2_on_level2_finish_tops[i], 0, 2) != 0)) {				// ���� �� ����, �� ��� ��� �
			all_2_levels_finish_tops.push_back(pos1_and_pos2_on_level2_finish_tops[i]);
			myfile << finishTopToStr(pos1_and_pos2_on_level2_finish_tops[i]) << endl;
		} else {																						// ���� ����� ���-�� ��������� �� ������ ����
			for (int k = 0; k <= last_color_of_small_blocks; k ++) {									// ����������
				if (blockIsNotUsed(pos1_and_pos2_on_level2_finish_tops[i], k)) {
					pos1_and_pos2_on_level2_finish_tops[i].buildings[5] = k + 48;
					all_2_levels_finish_tops.push_back(pos1_and_pos2_on_level2_finish_tops[i]);
					myfile << finishTopToStr(pos1_and_pos2_on_level2_finish_tops[i]) << endl;
				}
			}
		}
	}

	myfile.close();
	pos1_and_pos2_on_level2_finish_tops.resize(0);

//======================/ �������� 3-� ������ ���� ��������/======================

	myfile.open("4.txt");

//=====================/ ��������� 1-� ������� 3-��� ����� /======================

	vector<FinishTop> pos1_level3_finish_tops(0);

	for (unsigned int i = 0; i < all_2_levels_finish_tops.size(); i++) {								// ��� 2-� ����� ��������
		if ((typeCheckForFinishTops(all_2_levels_finish_tops[i], 1, 0) != 0)) {							// ���� �� ����, �� ��� ��� �
			pos1_level3_finish_tops.push_back(all_2_levels_finish_tops[i]);
		} else {																						// ���� ����� ���-�� ��������� �� ������ ����
			for (int k = first_color_of_small_blocks; k <= last_color_of_small_blocks; k ++) {			// ����������
				if (blockIsNotUsed(all_2_levels_finish_tops[i], k)) {
					all_2_levels_finish_tops[i].buildings[6] = k + 48;
					//myfile << finishTopToStr(all_2_levels_finish_tops[i]) << endl;
					pos1_level3_finish_tops.push_back(all_2_levels_finish_tops[i]);
				}
			}
		}
	}

	all_2_levels_finish_tops.resize(0);


//=====================/ ��������� 2-� ������� 3-��� ����� /======================

	vector<FinishTop> pos1_and_pos2_level3_finish_tops(0);

	for (unsigned int i = 0; i < pos1_level3_finish_tops.size(); i++) {									// ��� 2-� ����� ��������
		if ((typeCheckForFinishTops(pos1_level3_finish_tops[i], 1, 1) != 0)) {							// ���� �� ����, �� ��� ��� �
			pos1_and_pos2_level3_finish_tops.push_back(pos1_level3_finish_tops[i]);
		} else {																						// ���� ����� ���-�� ��������� �� ������ ����
			for (int k = first_color_of_small_blocks; k <= last_color_of_small_blocks; k ++) {			// ����������
				if (blockIsNotUsed(pos1_level3_finish_tops[i], k)) {
					pos1_level3_finish_tops[i].buildings[7] = k + 48;
					//myfile << finishTopToStr(pos1_level3_finish_tops[i]) << endl;
					pos1_and_pos2_level3_finish_tops.push_back(pos1_level3_finish_tops[i]);
				}
			}
		}
	}

	pos1_level3_finish_tops.resize(0);

//=====================/ ��������� 3-� ������� 3-��� ����� /======================

	vector<FinishTop> all_finish_tops(0);

	for (unsigned int i = 0; i < pos1_and_pos2_level3_finish_tops.size(); i++) {						// ��� 2-� ����� ��������
		if ((typeCheckForFinishTops(pos1_and_pos2_level3_finish_tops[i], 1, 2) != 0)) {					// ���� �� ����, �� ��� ��� �
			all_finish_tops.push_back(pos1_and_pos2_level3_finish_tops[i]);
			myfile << finishTopToStr(pos1_and_pos2_level3_finish_tops[i]) << endl;
		} else {																						// ���� ����� ���-�� ��������� �� ������ ����
			for (int k = first_color_of_small_blocks; k <= last_color_of_small_blocks; k ++) {			// ����������
				if (blockIsNotUsed(pos1_and_pos2_level3_finish_tops[i], k)) {
					pos1_and_pos2_level3_finish_tops[i].buildings[8] = k + 48;
					myfile << finishTopToStr(pos1_and_pos2_level3_finish_tops[i]) << endl;
					all_finish_tops.push_back(pos1_and_pos2_level3_finish_tops[i]);
				}
			}
		}
	}

	pos1_and_pos2_level3_finish_tops.resize(0);

	myfile.close();

	cout << "working...." << endl; // prints working....

//========================/ ����� ���������� ����� /=========================
//TODO: ��� �� �������� ��� ���-��������� � ����� � ������ ������


	cout << "ready" << endl; // prints ready
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
	return -2;
}

bool blockIsNotUsed(FinishTop top, int new_block) {
	char char_block = new_block + 48;
	for (short int i = 0; i < max_num_of_used_blocks; i++) {
		if (top.buildings[i] == char_block) return 0;
	}
	return 1;
}

string finishTopToStr(FinishTop top) {
	string str_top = "";
	for (short int i = 0; i < max_num_of_used_blocks; i++) {
		str_top = (string) str_top + top.buildings[i];
	}
	return str_top;
}
