#include "Individual.hpp"
#include "I2.hpp"
#include "Species.hpp"
#include <math.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <memory>

using namespace std;

double I2::getSurvival(vector<unique_ptr<Species>>* allSpecs, double delta){
	//This function gets the change an individual would experience
	//in the environment, which is then used to calculate survival
	//chances of the individual (intra-species selection)

	double survival = Individual::getSurvival(allSpecs, delta);
	//cout<<"Internal survival is: "<<survival<<endl;

    if(survival > 0.5 ){
		survival = 0.5 + exp(-((*envConst-optimum)*(*envConst-optimum)))*(survival-0.5);
		//survival = 0.5 + exp(-((optimum)*(optimum)))*(survival-0.5);
	}

	return survival;
}