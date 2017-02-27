#include "helpers.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <errno.h>
#include <string.h>
#include <vector>


using namespace std;


double getRandom(){
	//gets a random doubleing point number between 0 and 1
	double r = double(rand())/double(RAND_MAX);
	return r;
}

int getInt(){
	//gets an int from the user (and checks that it is valid)
	int a;
	cin>>a;
	if(cin.fail()){
		cout<<"WARNING!\n Wrong input (we need an int). Try again...\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		a=getInt();
	}
	return a;
}

double getDouble(){
	//gets a double from user input (and checks that it is valid)
	double a;
	cin>>a;
	if(cin.fail()){
		cout<<"WARNING!\n Wrong input (we need a double). Try again...\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		a=getDouble();
	}
	return a;
}

char getChar(){
	//gets ONE char from user input and checks if it is valid
	char a;
	cin>>a;
	if(cin.fail()){
		cout<<"WARNING!\n Wrong input (we need ONE char). Try again...\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		a=getChar();
	}
	return a;
}

void getRange(double range[2]){
	//gets two doubles that define a range(rangeSize must be positive)

	bool check = true;

	while(check){

		cout<<"Enter lower range limit:\n";
		range[0] = getDouble();
		cout<<"Enter upper range limit:\n";
		range[1] = getDouble();

		if(range[0] > range[1]){
			cout<<"Negative range size is not allowed.\nPlease enter a valid range.\n";
		}else{
			check = false;
		}
	}
}

void getIntRange(int range[2]){
	//gets two ints that define a range(rangeSize must be positive)

	bool check = true;

	while(check){

		cout<<"Enter lower range limit:\n";
		range[0] = getInt();
		cout<<"Enter upper range limit:\n";
		range[1] = getInt();

		if(range[0] > range[1]){
			cout<<"Negative range size is not allowed.\nPlease enter a valid range.\n";
		}else{
			check = false;
		}
	}
}

double getUniform(double range[2]){
	//gets a random double from a uniform distribution between range[0] and range[1]

	double size = range[1] - range[0];

	double res = (getRandom()*size) + range[0];

	return res;
}

double getUniformV(vector<double> range){
	//gets a random double from a uniform distribution between range[0] and range[1]

	double size = range[1] - range[0];

	double res = (getRandom()*size) + range[0];

	return res;
}

int getUniformInt(int range[2]){
	//gets a random int from a uniform distribution between range[0] and range[1]
	if(range[0] == range[1]){
		return 0;
	}

	return (rand()%(range[1] - range[0])) + range[0];
}

int initSave(string fileName){
	//initiates a saveFile (erases content if file alreadu exists)
	ofstream saveFile(fileName, ofstream::trunc);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in helpers::initSave:\n";
		cout<<strerror(errno);
		return -1;
	}
	saveFile<<""<<endl;

	saveFile.close();
	//cout<<"Filename is "<<fileName<<endl;

	return 0;
}


