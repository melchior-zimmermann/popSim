#include <vector>
#include <memory>
#include "Individual.hpp"
#include "Evo.hpp"

class Species;

using namespace std;

/**
 * @file NoEvo.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the EcoEvo class used to calculate evolutionary change in species interaction and optimum values.
 *
 */

#ifndef NOEVO_HPP
#define NOEVO_HPP

 /**
 *\brief Dummy implementation of the Evo class.
 *
 *Dummy implementaiton of the Evo class, given to species in simulations where no evolution occurs.
 *(could also be used to disable evolution in some species)
 *
 */

class NoEvo: public Evo{
  public:
  	void getEvo(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution){};
	vector<unique_ptr<Individual>> getInds(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution){vector<unique_ptr<Individual>> v; return move(v);}
	vector<int> runSelection(vector<unique_ptr<Individual>>* inds, vector<unique_ptr<Species>>* speciesList){vector<int> ints; return ints;}
	
};

#endif /*NOEVO_HPP*/