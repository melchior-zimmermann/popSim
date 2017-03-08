#include <iostream>
#include <vector> 
#include <memory>
#include <cmath>
#include "EvoNextGen.hpp"
#include "Species.hpp"
#include "Environment.hpp"

using namespace std;

void EvoNextGen::getNextGen(int stepNum, vector<unique_ptr<Species>>* speciesList, Environment* env){
	//gets the change in density from all species,
	//then updates all species' density

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
			env->getDeadSpecies()->push_back(move((*speciesList)[i]));
			speciesList->erase(speciesList->begin()+i);
		}
	}
	numSpecs = speciesList->size();

	for( i = 0; i<numSpecs; i++){
		//cout<<"evo gng third loop with i = "<<i<<endl;
		if(stepNum%(*speciesList)[i]->getGenTime() == 0){
			//cout<<"Going Evo\n";
			int resolution = (int)round((*speciesList)[i]->getDensity()*(*speciesList)[i]->getIndToDens());
			(*speciesList)[i]->getEvo(speciesList, resolution);

		}
	}
}