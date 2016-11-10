#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono> // C++11
#include <unordered_map>
using namespace std;

/** 
* Main function
* args[1] = part 1 or 2
* for Sudoku:
* 	args[2] = grid file path
* 	args[3] = word bank
* for game of breakthrough
*/
int main(int argc, char** args) {

	//part 2
	if (args[1][0] == 't') {
		if (argc != 5) {
			cout << "Wrong input format! Expected: ./mp3 [t] [model] [trainfile] [testfile]"<<endl;
			return 1;
		}

		//multinomial model
		if (args[2][0] == 'm') { 
			ifstream trainfile(args[3]);
			ifstream testfile(args[4]);
			int k = 0; //number of class labels
			int n = 0; //the length of a single document
			int m = 0; //the size of the vocabulary
			string line;
			vector<int> labels;
			unordered_map<string, float> unique_strings;
			while (getline(trainfile, line)) {
				string curWord;
				string curCount;
				int curCountInt;
				int i;
				if (line[0] == '-') { //if label is -1
					labels.push_back(-1);
					i = 2;
				}
				else { //label is 1
					labels.push_back(1);
					i = 1;					
				}
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {
						curCount.push_back(line[i]);
					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					} else if (line[i] == ' ')  {
						if (curWord.size() != 0) {
							// cout << "curWord is " << curWord << endl;
							curCountInt = std::stoi(curCount, nullptr, 10);
							if (unique_strings.find(curWord) == unique_strings.end()) {
								unique_strings.insert(std::make_pair(curWord, (float)curCountInt));
							} else {
								unique_strings[curWord] += (float)curCountInt;
							}
							curWord.clear();
							curCount.clear();	
						}
					}
					i++;
				}
			}
		}
	} 
	return 0;
}