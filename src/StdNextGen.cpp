#include <iostream>
#include <vector> 
#include <memory>
#include "StdNextGen.hpp"
#include "Species.hpp"
#include "Environment.hpp"


using namespace std;

void StdNextGen::getNextGen(int stepNum, vector<unique_ptr<Species>>* speciesList, Environment* env){
	//gets the change in density from all species,
	//then updates all species' density
	//cout<<"Got in@\n";

	int numSpecs = speciesList->size();
	vector<double>* change = env->getChange();

	//cout<<change->size()<<endl;

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