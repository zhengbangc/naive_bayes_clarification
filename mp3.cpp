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

bool sort_dict (const pair<string, int>  lhs, const pair<string, int>  rhs) {
	return lhs.second > rhs.second;
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
			cout << yellow << "Using multinomial model" << def << endl;
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

			int estimated_positive_correct_negative = 0;
			int estimated_positive_correct_positive = 0;
			int estimated_negative_correct_positive = 0;
			int estimated_negative_correct_negative = 0;
			int number_correct = 0;
			for (int i = 0; i < correct_classification.size(); i++) {
				if (estimate_classification[i] == correct_classification[i]) {
					number_correct++;
				}
				if (estimate_classification[i] == 1 && correct_classification[i] == -1)
					estimated_positive_correct_negative++;
				else if (estimate_classification[i] == 1 && correct_classification[i] == 1)
					estimated_positive_correct_positive++;
				else if (estimate_classification[i] == -1 && correct_classification[i] == 1)
					estimated_negative_correct_positive++;
				else if (estimate_classification[i] == -1 && correct_classification[i] == -1)
					estimated_negative_correct_negative++;
			}
			cout <<"The confusion matrix:" << endl;
			cout << blue << "		estimated positive | estimated negative" << endl;
			cout << "correct positive 	" << estimated_positive_correct_positive << "  	   |	    " << estimated_negative_correct_positive << endl;
			cout << "correct negative 	" << estimated_positive_correct_negative << "	   |	    " << estimated_negative_correct_negative << def << endl;
			cout << "the accuracy is " << red << (double)number_correct/(double)correct_classification.size() << def << endl << endl;


			vector<std::pair<string, int> >sorted_positive_dict (positive_dict.begin(), positive_dict.end());
			std::sort(sorted_positive_dict.begin(), sorted_positive_dict.end(), sort_dict);
			cout << "The top 10 words with the highest likelihood for " << green << "-1: " <<def << endl;
			for (int i = 0; i < 10; i++) {
				cout << green << sorted_positive_dict[i].first << def << " occurs " << green << sorted_positive_dict[i].second<< def << " times "  << endl;
			}
			vector<std::pair<string, int> >sorted_negative_dict (negative_dict.begin(), negative_dict.end());
			std::sort(sorted_negative_dict.begin(), sorted_negative_dict.end(), sort_dict);
			cout << "The top 10 words with the highest likelihood for " << green << "-1: " <<def << endl;
			for (int i = 0; i < 10; i++) {
				cout << green << sorted_negative_dict[i].first << def << " occurs " << green << sorted_negative_dict[i].second<< def << " times "  << endl;
			}

			cout << "The top 10 words with the highest odds ratio " << green << "log((P(w|positive)/P(w|negative)))" << def<< endl;
			std::vector<pair<string, double> > sorted_positive_odds_ratio;
			for (int i = 0; i < sorted_positive_dict.size() ; i++) {
				// cout << green << sorted_positive_dict[i].first << def << " occurs " << green << sorted_negative_dict[i].second<< def << " times "  << endl;
				string curWord = sorted_positive_dict[i].first;
				double positive_possibility = log10((double)(positive_dict[curWord] + 1) / (double)(total_positive_count + positive_dict.size()));
				double negative_possibility = log10((double)(negative_dict[curWord] + 1) / (double)(total_negative_count + negative_dict.size()));
				sorted_positive_odds_ratio.push_back(make_pair(curWord, positive_possibility - negative_possibility) );
			}
			std::sort(sorted_positive_odds_ratio.begin(), sorted_positive_odds_ratio.end(), sort_dict);
			for (int i = 0; i < 10; i++) {
				cout << green << sorted_positive_odds_ratio[i].first << def << " with log((P(w|positive)/P(w|negative))) " << green << sorted_positive_odds_ratio[i].second<< def  << endl;
			}

		}
		if (args[2][0] == 'b') { 
			cout << yellow << "Using Bernoulli model" << def << endl;
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
				//first prepare for thate correct answer to calculate the accuracy
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

			int estimated_positive_correct_negative = 0;
			int estimated_positive_correct_positive = 0;
			int estimated_negative_correct_positive = 0;
			int estimated_negative_correct_negative = 0;
			int number_correct = 0;
			for (int i = 0; i < correct_classification.size(); i++) {
				if (estimate_classification[i] == correct_classification[i]) {
					number_correct++;
				}
				if (estimate_classification[i] == 1 && correct_classification[i] == -1)
					estimated_positive_correct_negative++;
				else if (estimate_classification[i] == 1 && correct_classification[i] == 1)
					estimated_positive_correct_positive++;
				else if (estimate_classification[i] == -1 && correct_classification[i] == 1)
					estimated_negative_correct_positive++;
				else if (estimate_classification[i] == -1 && correct_classification[i] == -1)
					estimated_negative_correct_negative++;
			}
			cout <<"The confusion matrix:" << endl;
			cout << blue << "		estimated positive | estimated negative" << endl;
			cout << "correct positive 	" << estimated_positive_correct_positive << "  	   |	    " << estimated_negative_correct_positive << endl;
			cout << "correct negative 	" << estimated_positive_correct_negative << "	   |	    " << estimated_negative_correct_negative << def << endl;
			cout << "the accuracy is " << red << (double)number_correct/(double)correct_classification.size() << def << endl << endl	;

			vector<std::pair<string, int> >sorted_positive_dict (positive_dict.begin(), positive_dict.end());
			std::sort(sorted_positive_dict.begin(), sorted_positive_dict.end(), sort_dict);
			cout << "The top 10 words with the highest likelihood for " << green << "-1: " <<def << endl;
			for (int i = 0; i < 10; i++) {
				cout << green << sorted_positive_dict[i].first << def << " occurs " << green << sorted_positive_dict[i].second<< def << " times "  << endl;
			}
			vector<std::pair<string, int> >sorted_negative_dict (negative_dict.begin(), negative_dict.end());
			std::sort(sorted_negative_dict.begin(), sorted_negative_dict.end(), sort_dict);
			cout << "The top 10 words with the highest likelihood for " << green << "-1: " <<def << endl;
			for (int i = 0; i < 10; i++) {
				cout << green << sorted_negative_dict[i].first << def << " occurs " << green << sorted_negative_dict[i].second<< def << " times "  << endl;
			}

			cout << "The top 10 words with the highest odds ratio " << green << "log((P(w|positive)/P(w|negative)))" << def<< endl;
			std::vector<pair<string, double> > sorted_positive_odds_ratio;
			for (int i = 0; i < sorted_positive_dict.size() ; i++) {
				string curWord = sorted_positive_dict[i].first;
				double positive_possibility = log10((double)(positive_dict[curWord] + 1) / (double)(total_positive_count + positive_dict.size()));
				double negative_possibility = log10((double)(negative_dict[curWord] + 1) / (double)(total_negative_count + negative_dict.size()));
				sorted_positive_odds_ratio.push_back(make_pair(curWord, positive_possibility - negative_possibility) );
			}
			std::sort(sorted_positive_odds_ratio.begin(), sorted_positive_odds_ratio.end(), sort_dict);
			for (int i = 0; i < 10; i++) {
				cout << green << sorted_positive_odds_ratio[i].first << def << " with log((P(w|positive)/P(w|negative)))= " << green << sorted_positive_odds_ratio[i].second<< def  << endl;
			}
		}
	} 
	return 0;
}