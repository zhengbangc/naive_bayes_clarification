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
#include <cmath> //C++11
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
			unordered_map<string, float> positive_dict; //positive dictionary of unique words and count
			unordered_map<string, float> negative_dict; //negative dictionary of unique words and count
			float total_positive_count = 0;
			float total_negative_count = 0;

			while (getline(trainfile, line)) {
				if (line.size() == 0)
					continue;
				string curWord;
				string curCount;
				int curCountInt;
				int i;
				bool positive;
				if (line[0] == '-') { //if label is -1
					i = 2;
					positive = false;
				}
				else { //label is 1
					i = 1;			
					positive = true;		
				}
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {
						curCount.push_back(line[i]);
					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					} else if (line[i] == ' ')  {
						if (curWord.size() != 0) {
							curCountInt = std::stoi(curCount, nullptr, 10);
							if (positive) { //if the label is 1
								if (positive_dict.find(curWord) == positive_dict.end()) {
									positive_dict.insert(std::make_pair(curWord, (float)curCountInt));
								} else {
									positive_dict[curWord] += (float)curCountInt;
								}
								total_positive_count += (float)curCountInt;
							} else { //if the label is -1
								if (negative_dict.find(curWord) == negative_dict.end()) {
									negative_dict.insert(std::make_pair(curWord, (float)curCountInt));
								} else {
									negative_dict[curWord] += (float)curCountInt;
								}
								total_negative_count += (float)curCountInt;
							}
							curWord.clear();
							curCount.clear();	
						}
					}
					i++;
				}
			}
			cout << positive_dict.size() << endl;
			cout << negative_dict.size() << endl;
			cout << "total_positive_count = " << total_positive_count << endl;
			cout << "total_negative_count = " << total_negative_count << endl;
			cout << "P(positive) = " << total_positive_count / (total_positive_count + total_negative_count) << endl;
			cout << "P(negative) = " << total_negative_count / (total_positive_count + total_negative_count) << endl;

			//now that we have all the train data ready, let's deal with the test data
			vector<int> correct_classification;
			vector<int> estimate_classification;
			string curWord;
			string curCount;
			int curCountInt;
			int i;
			while (getline(testfile, line)) {
				if (line.size() == 0)
					continue;
				//first prepare for the correct answer to calculate the accuracy
				if (line[0] == '-') {
					correct_classification.push_back(-1);
					i = 2;
				}
				else {
					correct_classification.push_back(1);
					i = 1;
				}

				vector<string> curWords;
				vector<int> curCounts;
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {
						curCount.push_back(line[i]);
					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					} else if (line[i] == ' ') {
						if (curWord.size() != 0) {
							curWords.push_back(curWord);
							curCountInt = std::stoi(curCount, nullptr, 10);
							curCounts.push_back(curCountInt);
							curWord.clear();
							curCount.clear();	
						}
					}
					i++;
				}
				float p_positive = log10(total_positive_count / (total_positive_count + total_negative_count));
				float p_negative = log10(total_negative_count / (total_positive_count + total_negative_count));
				for (int j = 0; j < curWords.size(); j++) {
					if (positive_dict.find(curWords[j]) != positive_dict.end())
						p_positive += log10((positive_dict[curWords[j]] + 1) / (total_positive_count + positive_dict.size()));
					if (negative_dict.find(curWords[j]) != negative_dict.end())
						p_negative += log10((negative_dict[curWords[j]] + 1) / (total_negative_count + negative_dict.size()));
				}
				if (p_positive >= p_negative) {
					estimate_classification.push_back(1);
				} else {
					estimate_classification.push_back(-1);
				}
				
			}

			int number_correct = 0;
			cout <<"correct size is " <<  correct_classification.size() << endl;
			// cout <<"estimate size is " << estimate_classification.size() << endl;
			for (int i = 0; i < correct_classification.size(); i++) {
				if (estimate_classification[i] == correct_classification[i]) {
					number_correct++;
				}
			}
			cout << "the accuracy is " << (double)number_correct/(double)correct_classification.size() << endl;

		}
	} 
	return 0;
}