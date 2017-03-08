#include <vector>
#include <memory>
#include "Individual.hpp"

class Species;

using namespace std;


/**
 * @file Evo.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Evo class used to calculate evolutionary change in species interaction/optimum values (virtual).
 *
 */

#ifndef EVO_HPP
#define EVO_HPP

 /**
 *\brief Class containing three methods used to calculate evolutionary change in species (virtual).
 *
 * The Evo class has three methods that are used to calculate change in species attributes
 * (interactions/optimum). getEvo() is the controller method, getInds() divides the species
 * into individuals, and runSelection() selects surviving/reproducing individuals.
 *
 */

class Evo{
  protected:
    double delta;/**<Delta used to calculate survival probability (same value as simulation delta)*/
  public:

    void setDelta(double newVal){delta = newVal;}

  	/**
  	*\brief Controller method for updating species interactions/optimum.
  	*
  	* This method controls the evolutionary process, dividing the species into
  	* the right amount of individuals, selecting among them, and updating species
  	* interactions and optimum accordingly (for a more detailed description, see
  	* implementations).
  	* 
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
	*\param spec Species for which we will calculate evolutionary change (parameters are automatically updated during call).
	*\param resolution How many discrete individuals are contained in one unit of density.
	*/
  	virtual void getEvo(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution)=0;
  	/**
  	*\brief Method for creating individuals from species.
  	*
  	* This method divides a species with continous density into a discrete number of individuals 
  	* <tt>numInds = round(species.density) * resolution + 1<\tt> (+1 to avoid divide-by-zero errors. For a more detailed description, see
  	* implementations).
  	* 
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
	*\param spec Species from which we will generate individuals.
	*\param resolution How many discrete individuals are contained in one unit of density.
	*/
	virtual vector<unique_ptr<Individual>> getInds(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution)=0;
  	/**
  	*\brief Method that selects surviving/reproducing individuals.
  	*
  	* This method calculates survival/reproduction probability for each individual, and 
  	* puts the number of existing copies of them in the vector it returns (for a more detailed description, see
  	* implementations).
  	* 
  	*\param inds List containing all individuals that were generated from evolving species.
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
	*
	*/
	virtual vector<int> runSelection(vector<unique_ptr<Individual>>* inds, vector<unique_ptr<Species>>* speciesList)=0;
	
};

#endif /*EVO_HPP*/