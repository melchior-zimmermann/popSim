#include <iostream>
#include <vector> 
#include <memory>
#include <cmath>

#include "RKEvoMultiNextGen.hpp"
#include "Species.hpp"
#include "Environment.hpp"


using std::vector;
using std::unique_ptr;

void RKEvoMultiNextGen::getNextGen(int stepNum, vector<unique_ptr<Species>>* speciesList, Environment* env){
	//gets the change in density from all species,
	//then updates all species' density
	//cout<<"Got in@\n";

	int numSpecs = speciesList->size();
	double currentDensities[numSpecs];
	int i;
	for (i = 0; i<numSpecs; i++) {
		currentDensities[i] = (*speciesList)[i]->getDensity();
	}

	double derivatives1[numSpecs];
	double derivatives2[numSpecs];
	double derivatives3[numSpecs];
	double derivatives4[numSpecs];

	double delta = env->getDelta();

	//calculating first derivative

	for (i = 0; i<numSpecs; i++) {
		derivatives1[i] = (*speciesList)[i]->getChange(1, speciesList);//setting delta to '1' so that getChange returns derivative instead of increment
	}

	//applying first derivative, calculating second

	for (i = 0; i<numSpecs; i++) {
		(*speciesList)[i]->setDensity(currentDensities[i] + (delta*derivatives1[i]/2.0));
	}

	for (i = 0; i<numSpecs; i++) {
		derivatives2[i] = (*speciesList)[i]->getChange(1, speciesList);//setting delta to '1' so that getChange returns derivative instead of increment
	}

	//applying second derivative, calculating third

	for (i = 0; i<numSpecs; i++) {
		(*speciesList)[i]->setDensity(currentDensities[i] + (delta*derivatives2[i]/2.0));
	}

	for (i = 0; i<numSpecs; i++) {
		derivatives3[i] = (*speciesList)[i]->getChange(1, speciesList);//setting delta to '1' so that getChange returns derivative instead of increment
	}

	//applying third derivative, calculating fourth

	for (i = 0; i<numSpecs; i++) {
		(*speciesList)[i]->setDensity(currentDensities[i] + (delta*derivatives3[i]));
	}

	for (i = 0; i<numSpecs; i++) {
		derivatives4[i] = (*speciesList)[i]->getChange(1, speciesList);//setting delta to '1' so that getChange returns derivative instead of increment
	}

	//calculating estimated value at next step

	vector<double>* change = env->getChange();

	for (i = 0; i<numSpecs; i++) {
		(*speciesList)[i]->setDensity(currentDensities[i] + delta*(derivatives1[i] + 2*(derivatives2[i] + derivatives3[i]) + derivatives4[i])/6);
		(*change)[i] = currentDensities[i] - (*speciesList)[i]->getDensity();
	}

	for(i = numSpecs-1; i>-1; i--) {
		if((*speciesList)[i]->getDensity() < env->getDeathThreshold()){
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