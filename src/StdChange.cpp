
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

using std::vector;
using std::unique_ptr;
using std::cout;
using std::endl;

#include "StdChange.hpp"
#include "Species.hpp"

double StdChange::getChange(Species* spec, double delta, vector<unique_ptr<Species>>* speciesList){
	//gets the change in density after a given timestep, uses the list of all species (for their densities)
	//as well as delta (the stepSize)

    double sumInteractions = 0;
    int tot = speciesList->size();

    for(int i = 0; i<tot; i++){
        sumInteractions += (*speciesList)[i]->getDensity() * (*spec->getInteractions())[(*speciesList)[i]->getNumSelf()]; 
    }

    double change = delta*spec->getDensity()*(spec->getAlpha()*(1 - (spec->getDensity()/spec->getCc())*(spec->getDensity()/spec->getCc())) + spec->getBeta() + sumInteractions);

    if (std::isnan(change)) {
    	cout<<"Got nan for species "<<spec->getNumSelf()<<endl;
    	exit(EXIT_FAILURE);
    }

    return change;

}