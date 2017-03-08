#include <iostream>
#include <vector> 
#include <memory>
#include <cmath>
#include "E2MultiNextGen.hpp"
#include "Species.hpp"
#include "Environment.hpp"
#include "helpers.hpp"

using namespace std;


void E2MultiNextGen::getNextGen(int stepNum, vector<unique_ptr<Species>>* speciesList, Environment* env){
	//gets the change in density from all species,
	//then updates all species' density
	if(getRandom() < env->getPertProb()){

		double envConst = env->getEnvConst();
		envConst += getUniformV(env->getPertRange());
		env->getPerts()->push_back(vector<double> {(double)stepNum, envConst});
		env->setEnvConst(envConst);
	}

	int numSpecs = speciesList->size();
	vector<double>* change = env->getChange();
	int i;

	for(i = 0; i<numSpecs; i++){
		//cout<<"base gng first loop with i = "<<i<<endl;
		(*change)[i] = (*speciesList)[i]->getChange(env->getDelta(), speciesList);
	}

	//cout<<"base gng After first loop\n";

	for(i = numSpecs-1; i>-1; i--){
		double dens = (*speciesList)[i]->getDensity();
		dens += (*change)[i];
		(*speciesList)[i]->setDensity(dens);
		if(dens < env->getDeathThreshold()){
			(*speciesList)[i]->setDensity(0);
		}
	}
	numSpecs = speciesList->size();

	for( i = 0; i<numSpecs; i++){
		//cout<<"evo gng third loop with i = "<<i<<endl;
		if(stepNum%(*speciesList)[i]->getGenTime() == 0){
			//cout<<"Going Evo\n";
			int resolution = (int)round((*speciesList)[i]->getDensity()*(*speciesList)[i]->getIndToDens()) + 1;
			(*speciesList)[i]->getEvo(speciesList, resolution);

		}
	}
}