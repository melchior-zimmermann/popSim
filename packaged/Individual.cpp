#include "Species.hpp"
#include "Individual.hpp"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <memory>


using namespace std;

double Individual::getSurvival(vector<unique_ptr<Species>>* allSpecs, double delta){
	//This function gets the change an individual would experience
	//in the environment, which is then used to calculate survival
	//chances of the individual (intra-species selection)

	survival= 0;

	int tot = allSpecs->size();

    for(int i = 0; i<tot; i++){
        survival += (*allSpecs)[i]->getDensity() * interactions[(*allSpecs)[i]->getNumSelf()]; 
    }
    survial *= delta;

    return survival+0.5;
}