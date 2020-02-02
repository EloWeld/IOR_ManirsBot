#pragma comment(linker, "/STACK:10000000000")
//============================================================================
// Name		: SortBot.cpp
// Author	  : ArinaK
// Version	 :3.3
// Copyright   : Never use it!!!
// Description : Hello World in C++, ANSI-style
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

#define VER_EMP_SPD 100
#define HOR_EMP_SPD 80
#define VER_SPD 100
#define HOR_SPD 80
#define HOR_ACS 15
#define VER_ACS 20
#define HOR_EMP_ACS 15
#define VER_EMP_ACS 20
#define CUBE_MOVE 10

using namespace std;

struct conn {
	unsigned int to;
	unsigned long long weight;
};

struct element {
	unsigned long long dist;
	unsigned int ind;
	element() {}
	element(unsigned long long _dist, unsigned int _ind) {
		dist = _dist;
		ind = _ind;
	}
};

struct Heap {
	vector <element> elem;

	element getMin() {
		return elem[0];
	}
	bool emp() {
		return (elem.size() == 0);
	}
	void push(unsigned long long new_dist, unsigned int new_ind) {
		elem.push_back(element(new_dist, new_ind));
		int i = elem.size() - 1;
		while ((i > 0) && (elem[(i - 1) / 2].dist > elem[i].dist)) {
			swap(elem[i], elem[(i - 1) / 2]);
		}
	}
	void pop() {
		swap(elem[0], elem[elem.size() - 1]);
		elem.pop_back();
		int i = 0;
		int m = i;
		if ((2 * i + 1 < elem.size()) && (elem[m].dist > elem[2 * i + 1].dist)) m = 2 * i + 1;
		if ((2 * i + 2 < elem.size()) && (elem[m].dist > elem[2 * i + 2].dist)) m = 2 * i + 2;
		while (m != i) {
			swap(elem[i], elem[m]);
			i = m;
			if ((2 * i + 1 < elem.size()) && (elem[m].dist > elem[2 * i + 1].dist)) m = 2 * i + 1;
			if ((2 * i + 2 < elem.size()) && (elem[m].dist > elem[2 * i + 2].dist)) m = 2 * i + 2;

		}
	}
};
Heap heap;

string topToStr(unsigned long long top) {
	string str_top = "";
	for (int i = 11; i >= 0; i--) {
		int v = (top >> (3 * i)) & 7;
		str_top = str_top + (char)(v + '0');
	}
	return str_top;
}

string onlyClawToStr(unsigned long long top) {
	string str_claw_coordinates = "";
	int pos = (top >> 36) & 7;
	int level = (top >> 39) & 1;
	str_claw_coordinates = str_claw_coordinates + (char)(level + '0');
	str_claw_coordinates = str_claw_coordinates + (char)(pos + '0');

	return str_claw_coordinates;
}

string clawToStr(unsigned long long top) {
	string str_top = "";
	for (int i = 11; i >= 0; i--) {
		int v = (top >> (3 * i)) & 7;
		str_top = str_top + (char)(v + '0');
	}
	int pos = (top >> 36) & 7;
	int level = (top >> 39) & 1;
	str_top = str_top + " " + (char)(level + '0') + " " + (char)(pos + '0');
	return str_top;
}

unsigned long long createTopBase(int v1, int v2, int v3, int v4, int v5, int v6) {
	unsigned long long top = ((unsigned long long)v1 << 33) | ((unsigned long long)v2 << 30)
		| (v3 << 27) | (v4 << 24) | (v5 << 21) | (v6 << 18);
	return top;
}

unsigned long long createTopMedium(unsigned long long top, int from, int to) {
	unsigned long long cube = top & (7ULL << ((from * 3) + 18));
	top = top ^ cube;
	cube = cube >> ((from + 6 - to) * 3);
	top = top | cube;
	return top;
}

unsigned long long createTopClaw(unsigned long long top, int level, int pos) {
	top = (((unsigned long long) level) << 39) | (((unsigned long long) pos) << 36) | top;
	return top;
}

int getCube(unsigned long long top, int level, int pos) {
	int cube = (top >> (((1 - level) * 6) + pos) * 3) & 7;
	return cube;
}

bool isInRules(unsigned long long start, unsigned long long finish) {
	for (int i = 0; i < 6; i++) {
		int cube_start = (start >> ((i * 3) + 18)) & 7;
		int cube_finish = (finish >> ((i * 3) + 18)) & 7;
		if (cube_start == cube_finish) {
			return false;
		}
	}
	return true;
}

bool isStand(unsigned long long top, int pos) {
	int cube = getCube(top, 0, pos);
	return (cube != 0) & (cube != 5) & (cube != 6);
}

bool isClaw(unsigned long long top, int level, int pos) {
	if (level == 0) {
		int cube0 = getCube(top, 0, pos);
		int cube1 = getCube(top, 1, pos);
		if ((cube0 != 0) && (cube1 == 0)) {
			return true;
		}
	}
	else {
		int cube1 = getCube(top, 1, pos);
		if (cube1 != 0) {
			return true;
		}
	}
	return false;
}

int getClawPos(unsigned long long claw_top) {
	unsigned long long claw_YX = (claw_top >> 36) & 15;
	return claw_YX;
}

bool onLevel2(unsigned long long top, unsigned long long from_pos, unsigned long long to_pos) {
	if ((from_pos % 6) > (to_pos % 6)) {
		int tmp = from_pos;
		from_pos = to_pos;
		to_pos = tmp;
	}
	for (unsigned int pos = from_pos + 1; pos < to_pos; ++pos) {
		int obj = getCube(top, 2, pos);
		if (obj != 0) {
			return true;
		}
	}
	return false;
}

int cubeOnLevel2(unsigned long long top) {
	int pos = 666;
	for (unsigned int i = 0; i < 6; ++i) {
		if (getCube(top, 1, i) != 0) pos = i;
	}
	return pos;
}
int noCubeOnLevel1(unsigned long long top) {
	int pos = 666;
	for (unsigned int i = 0; i < 6; ++i) {
		if (getCube(top, 0, i) == 0) pos = i;
	}
	return pos;
}

int getClawCoordinates(int pos) {
	return ((1 - (pos / 6)) << 3) | (pos % 6);
}

int getClawPosition(int coordinates) {
	return ((1 - ((coordinates >> 3) & 1)) * 6) + (coordinates & 7);
}

int differents(unsigned long long now, unsigned long long was) {
	int itr = 666;
	for (int i = 0; i < 12; ++i) {
		if ((((now >> (i * 3)) & 7) == 0) && (((was >> (i * 3)) & 7) != 0)) {
			itr = i;
		}
	}
	int coordinates = getClawCoordinates(itr);
	return coordinates;
}

int getWeight(unsigned long long top, unsigned long long from_pos, unsigned long long to_pos, bool end) {
	unsigned long long weight = 0;
	unsigned long long claw_pos = getClawPos(top);

	unsigned long long NUMBER_OF_VER_PERMUTATION = 0;
	unsigned long long NUMBER_OF_EMP_HOR_PERMUTATION = 0;
	unsigned long long NUMBER_OF_EMP_VER_PERMUTATION = 0;

	unsigned long long HOR_LONG = abs(((getClawCoordinates(from_pos)) & 7) - ((getClawCoordinates(to_pos)) & 7));
	unsigned long long VER_LONG_UP = 0;
	unsigned long long VER_LONG_DOWN = 0;
	unsigned long long HOR_EMP_LONG = abs(int(claw_pos & 7) - int(getClawCoordinates(from_pos) & 7));
	unsigned long long VER_EMP_LONG_UP = 0;
	unsigned long long VER_EMP_LONG_DOWN = 0;

	if (HOR_EMP_LONG != 0) {
		NUMBER_OF_EMP_HOR_PERMUTATION += 1;
	}

	unsigned long long EXTRA_HOR_EMP_LONG = 0;
	unsigned long long EXTRA_VER_EMP_LONG = 0;
	//c кубиком
	//c 1 на 1
	if ((from_pos > 5) && (to_pos > 5)) {
		//не в соседнюю клетку
		if (HOR_LONG > 1) {
			VER_LONG_UP = 1;
			VER_LONG_DOWN = 1;
			NUMBER_OF_VER_PERMUTATION += 2;
			// через 2 этаж
			if (onLevel2(top, from_pos, to_pos)) {
				VER_LONG_UP = 2;
				VER_LONG_DOWN = 2;
			}
		}
	}
	//c 1 на 2
	if ((from_pos > 5) ^ (to_pos > 5)) {
		VER_LONG_UP = 1;
		VER_LONG_DOWN = 0;
		NUMBER_OF_VER_PERMUTATION += 1;
	}
	//c 2 на 2
	if ((from_pos < 6) && (to_pos < 6)) {
		VER_LONG_UP = 0;
		VER_LONG_DOWN = 0;
		NUMBER_OF_VER_PERMUTATION += 0;
	}


	// без кубика
	//c 1 на 1
	if ((getClawPosition(claw_pos) < 6) && (from_pos < 6)) {
		// через 1 этаж
		if (onLevel2(top, claw_pos, from_pos)) {
			VER_LONG_UP = 1;
			VER_LONG_DOWN = 1;
			NUMBER_OF_EMP_VER_PERMUTATION += 2;
		}
	}
	//c 1 на 2
	if ((getClawPosition(claw_pos) > 5) ^ (from_pos > 5)) {
		VER_EMP_LONG_UP = 1;
		VER_EMP_LONG_DOWN = 0;
		NUMBER_OF_EMP_VER_PERMUTATION += 1;
	}
	//c 2 на 2
	if ((getClawPosition(claw_pos) > 5) && (from_pos > 5)) {
		VER_EMP_LONG_UP = 0;
		VER_EMP_LONG_DOWN = 0;
		NUMBER_OF_EMP_VER_PERMUTATION += 0;
	}

	//конечна€ перестановка
	if (end) {
		EXTRA_VER_EMP_LONG = 1 - (getClawCoordinates(to_pos) >> 3);
		NUMBER_OF_EMP_VER_PERMUTATION += 1 * EXTRA_VER_EMP_LONG;
		EXTRA_HOR_EMP_LONG = getClawCoordinates(to_pos) & 7;
		if (EXTRA_HOR_EMP_LONG > 0) {
			NUMBER_OF_EMP_HOR_PERMUTATION += 1;
		}
	}

	weight = HOR_ACS + (VER_ACS * NUMBER_OF_VER_PERMUTATION) +
		+(HOR_EMP_ACS * NUMBER_OF_EMP_HOR_PERMUTATION) + (VER_EMP_ACS * NUMBER_OF_EMP_VER_PERMUTATION) +
		+((VER_LONG_UP + VER_LONG_DOWN) * VER_SPD) + (HOR_LONG * HOR_SPD) +
		+((VER_EMP_LONG_UP + VER_EMP_LONG_DOWN) * VER_EMP_SPD) + (HOR_EMP_LONG * HOR_EMP_SPD) + CUBE_MOVE +
		+(EXTRA_VER_EMP_LONG * VER_EMP_SPD) + (EXTRA_HOR_EMP_LONG * HOR_EMP_SPD);

	return weight;
}

int getSwap(unsigned long long from, unsigned long long to) {
	unsigned long long top_from = from & 68719476735;
	unsigned long long top_to = to & 68719476735;
	//	unsigned long long end_claw_pos = 0;

	if (top_from != top_to) {
		int cube1_pos = 0;
		int cube1_from = top_from & 7;
		int cube1_to = top_to & 7;
		while (cube1_from == cube1_to) {
			cube1_pos++;
			top_from = top_from >> 3;
			top_to = top_to >> 3;
			cube1_from = top_from & 7;
			cube1_to = top_to & 7;
		}

		int cube2_pos = cube1_pos + 1;
		top_from = top_from >> 3;
		top_to = top_to >> 3;
		int cube2_from = top_from & 7;
		int cube2_to = top_to & 7;
		while (cube2_from == cube2_to) {
			cube2_pos++;
			top_from = top_from >> 3;
			top_to = top_to >> 3;
			cube2_from = top_from & 7;
			cube2_to = top_to & 7;
		}

		top_from = top_from >> 3;
		top_to = top_to >> 3;

		if (top_from == top_to) {
			if (cube1_to == 0) {
				int claw_pos = getClawPos(to);
				int cube_pos = getClawCoordinates(cube2_pos);
				if (claw_pos == cube_pos) {
					return getWeight(from, cube1_pos, cube2_pos, false);
				}
				else {
					// ’итрое условие дл€ конечной позции
					if ((claw_pos == 8) && (cube1_pos < 6)) { // 1000
						return getWeight(from, cube1_pos, cube2_pos, true);
					}
				}
			}
			if (cube2_to == 0) {
				int claw_pos = getClawPos(to);
				int cube_pos = getClawCoordinates(cube1_pos);
				if (claw_pos == cube_pos) {
					return getWeight(from, cube2_pos, cube1_pos, false);
				}
				else {
					// ’итрое условие дл€ конечной позции
					if ((claw_pos == 8) && (cube2_pos < 6)) { // 1000
						return getWeight(from, cube2_pos, cube1_pos, true);
					}
				}
			}
		}
	}
	return 0;
}
bool isSecondLevel(unsigned long long top) {
	if ((top & 262143) == 0) {
		return false;
	}
	return true;
}

int main() {
	cout << " _____ " << endl;
	cout << "|start|" << endl;
	cout << "|_____|" << endl;
	// —оздание стартовых вершин
	vector<unsigned long long> start_tops(0);
	for (int i = 1; i <= 6; ++i) {
		for (int j = 1; j <= 6; ++j) {
			for (int k = 1; k <= 6; ++k) {
				for (int l = 1; l <= 6; ++l) {
					for (int m = 1; m <= 6; ++m) {
						for (int n = 1; n <= 6; ++n) {
							if ((i != j) && (i != k) && (i != l) && (i != m)
								&& (i != n) && (j != k) && (j != l) && (j != m)
								&& (j != n) && (k != l) && (k != m) && (k != n)
								&& (l != m) && (l != n) && (m != n)) {
								start_tops.insert(start_tops.end(), createTopBase(i, j, k, l, m, n));
							}
						}
					}
				}
			}
		}
	}

	// —оздание промежуточных вершин
	vector<unsigned long long> medium_tops(0);
	for (unsigned int i = 0; i < start_tops.size(); ++i) {
		for (unsigned int from = 0; from < 6; ++from) {
			for (unsigned int to = 0; to < 6; ++to) {
				if ((isStand(start_tops[i], to)) && (from != to)) {
					medium_tops.insert(medium_tops.end(), createTopMedium(start_tops[i], from, to));
				}
			}
		}
	}

	// ƒобавление стртовых вершин к промежуточным вершинам
	for (unsigned int i = 0; i < start_tops.size(); ++i) {
		medium_tops.insert(medium_tops.end(), start_tops[i]);
	}


	// —оздание вершин с клешней
	vector<unsigned long long> all_tops(0);
	for (unsigned int i = 0; i < medium_tops.size(); ++i) {
		for (int level = 0; level <= 1; ++level) {
			for (int pos = 0; pos <= 5; ++pos) {
				if (isClaw(medium_tops[i], level, pos)) {
					all_tops.insert(all_tops.end(), createTopClaw(medium_tops[i], level, pos));
				}
			}
		}
	}

	// ƒобавление к вершинам с клешней стратовых позиций клешни
	for (unsigned int i = 0; i < start_tops.size(); ++i) {
		all_tops.insert(all_tops.end(), createTopClaw(start_tops[i], 1, 0));
	}
	medium_tops.clear();
	start_tops.clear();
	cout << all_tops.size() << endl;
	vector<conn> connections[80000];
	for (unsigned int i = 0; i < all_tops.size(); ++i) {
		for (unsigned int j = 0; j < all_tops.size(); ++j) {
			unsigned long long top_swap = getSwap(all_tops[i], all_tops[j]);
			if (top_swap != 0) {
				connections[i].insert(connections[i].end(), { j, top_swap });
			}
		}
		if (i % 770 == 0) cout << i / 770 << "%" << endl;
	}
	cout << "ѕерва€ часть успешно завершена" << endl;
	ofstream myfile;
	//cout<<"-------------------------";
	//	myfile.close();
	unsigned long long INF = 524280;
	//cout << "=======================" << endl;
	//определение стартовой вершины
	for (unsigned int start = 76320; start < all_tops.size(); ++start) {
		//Dijkstra's algorithm
		unsigned long long dist[77100];
		bool visited[77100];
		unsigned int pred[77100];
		for (unsigned int i = 0; i < all_tops.size(); ++i) {
			dist[i] = INF;
			visited[i] = false;
			pred[i] = 77777;
		}
		dist[start] = 0;

		//cout << "$$$$$$$$$" << endl;
		heap.push(0, start);
		while (!heap.emp()){
			//			cout<<itr<<endl;
			element minimum = heap.getMin();
			heap.pop();
			if ((dist[minimum.ind] == minimum.dist)&&(!visited[minimum.dist])){
				//cout << itr << "^^^" <<sel<<endl;
				unsigned int sel = minimum.ind;
				for (unsigned long long i = 0; i < connections[sel].size(); ++i) {
					unsigned int wgh = dist[sel] + connections[sel][i].weight;
					if ((!visited[connections[sel][i].to]) && (wgh < dist[connections[sel][i].to])) {
						dist[connections[sel][i].to] = wgh;
						pred[connections[sel][i].to] = sel;
						heap.push(wgh, connections[sel][i].to);
					}
				}
				visited[sel] = true;
			}
			//cout<<"::::::::::::"<<endl;
		}
		string name = "case" + topToStr(all_tops[start]) + ".txt";
		myfile.open(name);
		//		int chose1 = 0;
		//		int chose1 = 0;
		//определение финишной клешни
		for (unsigned long finish = (76320); finish < all_tops.size(); ++finish) {
			if (isInRules(all_tops[start], all_tops[finish])) {
				myfile << topToStr(all_tops[finish]) << " ";
				unsigned int to_start = finish;
				myfile << onlyClawToStr(all_tops[to_start]);
				myfile << " 0" << noCubeOnLevel1(all_tops[pred[to_start]]);
				myfile << " 1" << cubeOnLevel2(all_tops[pred[to_start]]);

				while (pred[to_start] != start) {
					to_start = pred[to_start];
					myfile << " " << onlyClawToStr(all_tops[to_start]);
					int tmp = differents(all_tops[to_start], all_tops[pred[to_start]]);
					myfile << " " << (tmp >> 3) << (tmp & 7);
				}
				myfile /*< " " << onlyClawToStr(all_tops[start]) */ << endl;

			}
		}
		//		myfile << all_tops[start] << endl;
		myfile.close();
		if (((start - 76320) % 7) == 0)cout << (start - 76320) / 7 << "%" << endl;
		//cout << endl << start << endl;
	}
	cout << "								   dd" << endl;
	cout << "								   dd" << endl;
	cout << "	   eeee      nn nnn            dd" << endl;
	cout << "	 ee	   ee	 nnn   nn	   dddddd" << endl;
	cout << "	 eeeeeeee    nn 	nn   dd    dd" << endl;
	cout << "	 ee          nn     nn   dd    dd" << endl;
	cout << "	  eeeeee     nn     nn     dddddd" << endl;
	return 0;
}

