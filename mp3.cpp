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

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW 	= 33,
        FG_BLUE     = 36,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

/** 
* Main function
* args[1] = part 1 or 2
* for Sudoku:
* 	args[2] = grid file path
* 	args[3] = word bank
* for game of breakthrough
*/
int main(int argc, char** args) {
	Color::Modifier red(Color::FG_RED);
	Color::Modifier def(Color::FG_DEFAULT);
	Color::Modifier blue(Color::FG_BLUE);
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier yellow(Color::FG_YELLOW);
	
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
			string line;
			unordered_map<string, int> positive_dict; //positive dictionary of unique words and count
			unordered_map<string, int> negative_dict; //negative dictionary of unique words and count
			int total_positive_count = 0;
			int total_negative_count = 0;

			while (getline(trainfile, line)) {
				if (line.size() == 0)
					continue;
				string curWord;
				string curCount;
				int curCountInt;
				int i;
				bool positive;
				if (line[0] == '-') { //if label is -1
					i = 3;
					positive = false;
				}
				else { //label is 1
					i = 2;			
					positive = true;		
				}
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {
						curCount.push_back(line[i]);
					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					}
					if (line[i] == ' ' || i == line.length()-1)  {
						if (curWord.size() != 0) {
							curCountInt = std::stoi(curCount, nullptr, 10);
							if (positive) { //if the label is 1
								if (positive_dict.find(curWord) == positive_dict.end()) {
									positive_dict.insert(std::make_pair(curWord, curCountInt));
								} else {
									positive_dict[curWord] += curCountInt;
								}
								total_positive_count += curCountInt;
							} else { //if the label is -1
								if (negative_dict.find(curWord) == negative_dict.end()) {
									negative_dict.insert(std::make_pair(curWord, curCountInt));
								} else {
									negative_dict[curWord] += curCountInt;
								}
								total_negative_count += curCountInt;
							}
							curWord.clear();
							curCount.clear();	
						}
					}
					i++;
				}
			}
			cout << "There are " << green << positive_dict.size() << def<< " unique words in positive posts"  << endl;
			cout << "There are " << green << negative_dict.size() << def << " unique words in negative posts" <<endl;
			cout << "total_positive_count = " << green << total_positive_count << def << endl;
			cout << "total_negative_count = " << green << total_negative_count << def << endl;
			double prior_positive = (double)total_positive_count / (double)(total_positive_count + total_negative_count);
			double prior_negative = (double)total_negative_count / (double)(total_positive_count + total_negative_count);
			// double p_positive = log10((double)positive_dict.size() / (double)(positive_dict.size() + negative_dict.size()));
			// double p_negative = log10((double)negative_dict.size() / (double)(positive_dict.size() + negative_dict.size()));
			cout << "prior_positive = " << green << prior_positive << def << endl;
			cout << "prior_negative = " << green << prior_negative << def << endl;
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
					i = 3;
				}
				else {
					correct_classification.push_back(1);
					i = 2;
				}

				vector<string> curWords;
				vector<int> curCounts;
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {
						curCount.push_back(line[i]);
					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					}
					if (line[i] == ' ' || i == line.length() - 1) {
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
				double likelihoods_positive = 0;
				double likelihoods_negative = 0;
				for (int j = 0; j < curWords.size(); j++) {
					if (positive_dict.find(curWords[j]) != positive_dict.end()) {
						likelihoods_positive += log10((double)(positive_dict[curWords[j]] + 1) / (double)(total_positive_count + positive_dict.size()));						
					} else {
						likelihoods_positive += log10((double)1/(double)(total_positive_count + positive_dict.size()));
					}

					if (negative_dict.find(curWords[j]) != negative_dict.end()) {
						likelihoods_negative += log10((double)(negative_dict[curWords[j]] + 1) / (double)(total_negative_count + negative_dict.size()));						
					} else {
						likelihoods_negative += log10((double)1/(double)(total_negative_count + negative_dict.size()));
					}
				}
				likelihoods_positive += log10(prior_positive);
				likelihoods_negative += log10(prior_negative);
				if (likelihoods_positive > likelihoods_negative) {
					estimate_classification.push_back(1);
				} else {
					estimate_classification.push_back(-1);
				}
			}

			int number_correct = 0;
			for (int i = 0; i < correct_classification.size(); i++) {
				if (estimate_classification[i] == correct_classification[i]) {
					number_correct++;
				}
			}
			cout << "the accuracy is " << red << (double)number_correct/(double)correct_classification.size() << def << endl;

		}
		if (args[2][0] == 'b') { 
			ifstream trainfile(args[3]);
			ifstream testfile(args[4]);
			string line;
			unordered_map<string, int> positive_dict; //positive dictionary of unique words and count
			unordered_map<string, int> negative_dict; //negative dictionary of unique words and count
			int total_positive_count = 0; //total number of words existence in positive posts, i.e. if the word appear in one post, we increment the varibale
			int total_negative_count = 0; //total number of words existence in negative posts, i.e. if the word appear in one post, we increment the varibale
			while (getline(trainfile, line)) {
				if (line.size() == 0)
					continue;
				string curWord;
				string curCount;
				// int curCountInt;
				int i;
				bool positive;
				if (line[0] == '-') { //if label is -1
					i = 3;
					positive = false;
				}
				else { //label is 1
					i = 2;			
					positive = true;		
				}
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {

					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					}
					if (line[i] == ' ' || i == line.length()-1)  {
						if (curWord.size() != 0) {
							if (positive) { //if the label is 1
								if (positive_dict.find(curWord) == positive_dict.end()) {
									positive_dict.insert(std::make_pair(curWord, 1));
								} else {
									positive_dict[curWord] += 1;
								}
								total_positive_count++;
							} else { //if the label is -1
								if (negative_dict.find(curWord) == negative_dict.end()) {
									negative_dict.insert(std::make_pair(curWord, 1));
								} else {
									negative_dict[curWord] += 1;
								}
								total_negative_count++;
							}
							curWord.clear();
						}
					}
					i++;
				}
			}

			cout << "There are " << green << positive_dict.size() << def<< " unique words in positive posts"  << endl;
			cout << "There are " << green << negative_dict.size() << def << " unique words in negative posts" <<endl;
			cout << "total_positive_count = " << green << total_positive_count << def << endl;
			cout << "total_negative_count = " << green << total_negative_count << def << endl;
			double prior_positive = (double)total_positive_count / (double)(total_positive_count + total_negative_count);
			double prior_negative = (double)total_negative_count / (double)(total_positive_count + total_negative_count);

			vector<int> correct_classification;
			vector<int> estimate_classification;
			string curWord;
			int i;
			while (getline(testfile, line)) {
				if (line.size() == 0)
					continue;
				//first prepare for the correct answer to calculate the accuracy
				if (line[0] == '-') {
					correct_classification.push_back(-1);
					i = 3;
				}
				else {
					correct_classification.push_back(1);
					i = 2;
				}

				vector<string> curWords;
				vector<int> curCounts;
				while (i < line.length()) {
					if (line[i] >= '0' && line[i] <= '9') {

					} else if (line[i] != ':' && line[i] != ' ') {
						curWord.push_back(line[i]);
					}
					if (line[i] == ' ' || i == line.length() - 1) {
						if (curWord.size() != 0) {
							curWords.push_back(curWord);
							curCounts.push_back(1);
							curWord.clear();
						}
					}
					i++;
				}
				double likelihoods_positive = 0;
				double likelihoods_negative = 0;
				for (int j = 0; j < curWords.size(); j++) {
					if (positive_dict.find(curWords[j]) != positive_dict.end()) {
						likelihoods_positive += log10((double)(positive_dict[curWords[j]] + 1) / (double)(total_positive_count + positive_dict.size()));						
					} else {
						likelihoods_positive += log10((double)1/(double)(total_positive_count + positive_dict.size()));
					}

					if (negative_dict.find(curWords[j]) != negative_dict.end()) {
						likelihoods_negative += log10((double)(negative_dict[curWords[j]] + 1) / (double)(total_negative_count + negative_dict.size()));						
					} else {
						likelihoods_negative += log10((double)1/(double)(total_negative_count + negative_dict.size()));
					}
				}
				likelihoods_positive += log10(prior_positive);
				likelihoods_negative += log10(prior_negative);
				if (likelihoods_positive > likelihoods_negative) {
					estimate_classification.push_back(1);
				} else {
					estimate_classification.push_back(-1);
				}
			}

			int number_correct = 0;
			for (int i = 0; i < correct_classification.size(); i++) {
				if (estimate_classification[i] == correct_classification[i]) {
					number_correct++;
				}
			}
			cout << "the accuracy is " << red << (double)number_correct/(double)correct_classification.size() << def << endl;


		}
	} 
	return 0;
}