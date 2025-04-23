//
// Created by ryder on 2021/12/9.
//

#ifndef PRACTICE_ALGO_H
#define PRACTICE_ALGO_H

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

template<typename Container>
void printC(const Container& c, ostream& out = cout) {
	if (c.empty()) {
		return;
	}
	else {
		typename Container::const_iterator itr = c.begin();
		out << " [ " << *itr++;
		while (itr != c.end()) {
			out << ", " << *itr++;
		}
		out << " ]" << endl;
	}
}

void strToUpper(string& s);

void strToLower(string& s);

#endif //PRACTICE_ALGO_H
