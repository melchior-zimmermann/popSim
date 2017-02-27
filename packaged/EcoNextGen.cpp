#include <iostream>
#include <vector> 
#include <memory>
#include "EcoNextGen.hpp"
#include "Species.hpp"
#include "Environment.hpp"
#include "helpers.hpp"

using namespace std;

void EcoNextGen::getNextGen(int stepNum, vector<unique_ptr<Species>>* speciesList, Environment* env){
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

	for(int i = 0; i<numSpecs; i++){
		//cout<<"base gng first loop with i = "<<i<<endl;
		(*change)[i] = (*speciesList)[i]->getChange(env->getDelta(), speciesList);
	}

	//cout<<"base gng After first loop\n";

	for(int i = numSpecs-1; i>-1; i--){
		double dens = (*speciesList)[i]->getDensity();
		dens += (*change)[i];
		(*speciesList)[i]->setDensity(dens);
		if(dens < env->getDeathThreshold()){
			(*speciesList)[i]->setDensity(0);
			env->getDeadSpecies()->push_back(move((*speciesList)[i]));
			speciesList->erase(speciesList->begin()+i);
		}
	}
}
