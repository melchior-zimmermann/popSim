
#include <vector>
#include <memory>
#include <iostream>
#include "StdChange.hpp"
#include "Species.hpp"

using std::vector;
using std::unique_ptr;


double StdChange::getChange(Species* spec, double delta, vector<unique_ptr<Species>>* speciesList){
	//gets the change in density after a given timestep, uses the list of all species (for their densities)
	//as well as delta (the stepSize)

    double sumInteractions = 0;
    int tot = speciesList->size();

    for(int i = 0; i<tot; i++){
        sumInteractions += (*speciesList)[i]->getDensity() * (*spec->getInteractions())[(*speciesList)[i]->getNumSelf()]; 
    }

    double change = delta*spec->getDensity()*(spec->getAlpha()*(1-(spec->getDensity()/spec->getCc()) * (spec->getDensity()/spec->getCc())) + spec->getBeta() + sumInteractions);

    return change;

}