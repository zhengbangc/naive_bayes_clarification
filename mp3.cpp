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
	

	if (args[1][0]=='c')
	{

	}

	if (argc != 4 && args[1][0] == 's') {
			cout << "Wrong input format. Expected ./mp2 [s] [gridfile] [wordbank]"<<endl;
			return 1;
	}

	if (args[1][0] == 's') {

	} 
	return 0;
}