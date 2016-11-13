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
#include <iomanip>
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
	if (args[1][0]=='d')
	{
		//P(Fij = f | class) = (# of times pixel (i,j) has value f in training examples from this class) / (Total # of training examples from this class)
		//training phase
	  	string line;
	  	int vec[28];
	  	int matrix[28*28*10];
	  	float confusion_matrix[100];
	  	for(int i =0;i<28*28*10;i++)
	  	{
	  		matrix[i]=0;
	  	}
		ifstream trainingimages ("./digitdata/trainingimages");
		ifstream traininglabels ("./digitdata/traininglabels");
		int j=0;
		// double count[10];
		double pclass[10];

		while(traininglabels.is_open()&&trainingimages.is_open())
		{

			getline (traininglabels,line);
			if(line=="")
				break;
			int number=stoi(line);
			pclass[number]++;
			// int a;
			// cout<<"reading the label: "<<number<<endl;
			// cin>>a;
			//j is the next line
			// cout<<"the image is :"<<endl;
		    while ( getline (trainingimages,line))
		    {
			    for(int i =0;i<28;i++)
			    {

			    	if(line[i]=='+'||line[i]=='#'){
						matrix[number*28*28+j*28+i]++;
			    	// cout<<matrix[number*28*28+j*28+i];	
			    	}
			    	// else
			    		// cout<<' ';
			   	}
			   	// cout<<endl;
		    
		    if(j==27)
		    {
		    	j=0;
		    	break;
		    }
		    j++;

		   }
		}
	
//testing phase
	cout<<"finished training"<<endl;
	ifstream testimages ("./digitdata/testimages");
	ifstream testlabels ("./digitdata/testlabels");
	cout<<"start testing"<<endl;
	j=0;
	int correctones;
	// for(int i=0;i<100;i++){
	// 	confusion_matrix[i]=1;
	// }
	int appearance[10];
	double AP[10];
	float smoothing=0.1;

	while(testimages.is_open()&&testlabels.is_open())
	{
		getline (testlabels,line);
		for(int i=0;i<10;i++){
				AP[i]=log(double(pclass[i]+2*smoothing)/double(5000+2*10*smoothing));
				// cout<<"the possibloity of number "<< i<<" is "<< AP[i]<<endl;
				// cout<<"the numer of this class is "<<pclass[i]<<endl;
		}
		// cout<<"the line is :"<<line<<endl;
		if(line=="")
			break;
		int number=stoi(line);
		int a;
		// cout<<"reading the test label: "<<number<<endl;
		// cin>>a;
		//j is the next line
		// cout<<"the image is :"<<endl;
	    while ( getline (testimages,line))
	    {

		    for(int i =0;i<28;i++)
		    {

		    	if(line[i]=='+'||line[i]=='#'){
		    		for(int m=0;m<10;m++){
					AP[m]=AP[m]+log(double(matrix[m*28*28+j*28+i]+smoothing))-log(double(pclass[m]+2*smoothing));		
		    		}			   
		    	}
		    	else
		    		{
		    		for(int m=0;m<10;m++){
					AP[m]=AP[m]+log(double(pclass[m]+smoothing-matrix[m*28*28+j*28+i]))-log(double(pclass[m]+2*smoothing));			    			
		    		}
		    	}
		   	}
		   	// cout<<endl;
		    	// cout<<"AP 1 is "<<(double)AP[2]<<endl;
		    	// cout<<"AP 1 is "<<(double)(matrix[1*28*28+1*28+1]+smoothing)/double(pclass[1]+2*smoothing)<<endl;
	    
	    if(j==27)
	    {
	    	j=0;
	    	double max=-INFINITY;
	    	int index=0;
	    	for(int i=0;i<10;i++)
	    	{
	    		if(AP[i]>=max)
	    		{
	    			max=AP[i];
	    			index=i;
	    			// cout<<"the most likely value is"<<AP[i]<<endl;
	    		}
	    	}
	    	// cout<<"the number should be: "<<index;
	    	if(index==number){
	    		correctones++;
	    	}
	    	else
	    	{
	    		// row is the testing label, column is the estimation
	    		confusion_matrix[number*10+index]=confusion_matrix[number*10+index]+1;
	    		// cout<<"wrong :"<<"should be  "<<number<<"but estimation is "<<index<<endl;
	    	}
    		appearance[number]++;

	    	// cin>>index;
	    	break;
	    }
	    j++;

	   }
	}
		double accuracy=correctones;
		cout<<"accuracy is: "<<(double)accuracy/1000<<endl;
		// cout<<"label count"<<count<<endl;
		for(int i=0;i<100;i++)
		{
			confusion_matrix[i]=confusion_matrix[i]/(appearance[i/10]);
		}
		//Odds ratio testing
		float one,two,three,four;
		int onea,oneb,twoa,twob,threea,threeb,foura,fourb;
			for(int a=0;a<10;a++){
				for(int b=a+1;b<10;b++)
				{
					if(confusion_matrix[a*10+b]>one)
					{
						four=three;
						foura=threea;
						fourb=threeb;
						three=two;
						threea=twoa;
						threeb=twob;
						twoa=onea;
						twob=oneb;
						two=one;
						onea=a;
						oneb=b;
						one=confusion_matrix[10*a+b];
					}
					else if(confusion_matrix[a*10+b]>two)
					{
						four=three;
						foura=threea;
						fourb=threeb;
						three=two;
						threea=twoa;
						threeb=twob;
						twoa=a;
						twob=b;
						two=confusion_matrix[10*a+b];
					}
					else if(confusion_matrix[a*10+b]>three)
					{
						four=three;
						foura=threea;
						fourb=threeb;
						threea=a;
						threeb=b;
						three=confusion_matrix[10*a+b];
					}
					else if(confusion_matrix[a*10+b]>four)
					{
						foura=a;
						fourb=b;
						four=confusion_matrix[10*a+b];
					}	

				}
			}

		cout<<"top confusing pair is: " <<onea<<" "<<oneb<<endl;
		cout<<"second confusing pair is"<<twoa<<" "<<twob<<endl;
		cout<<"third confusing pair is"<<threea<<" "<<threeb<<endl;
		cout<<"fourth confusing pair is"<<foura<<" "<<fourb<<endl;
		std::cout << std::setprecision(5) << std::fixed;
		for(int i=0;i<10;i++)
		{
			for(int k=0;k<10;k++)
			{
			cout<<"\t"<<confusion_matrix[i*10+k];				
			}
			cout<<endl;
		}

	}		


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