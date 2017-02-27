#include <vector>
#include <memory>
#include "Individual.hpp"
#include "Evo.hpp"

class Species;

using namespace std;

/**
 * @file StdEvo.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the StdEvo class used to calculate evolutionary change in species interaction.
 *
 */

#ifndef STDEVO_HPP
#define STDEVO_HPP
 
 /**
 *\brief Implementation of the Evo class that updates species interactions according to selection. Species optimum is not affected.
 *
 *
 */

class StdEvo: public Evo{
  public:
  	/**
  	*\brief Controller method for updating species interactions/optimum.
  	*
  	* Calls getInds() to get a list of individuals on which to call runSelection().
  	* Then calculates the mean of all interaction values from selected individuals, and 
  	* sets this as the new interaction values for the species.
  	* 
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
  	*\param spec Species for which we will calculate evolutionary change (parameters are automatically updated during call).
  	*\param resolution How many discrete individuals are contained in one unit of density.
  	*/
  	void getEvo(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution);
  	/**
  	*\brief Method for creating individuals from species.
  	*
  	* This method divides a species with continous density into a discrete number of individuals 
  	* <tt>numInds = round(species.density) * resolution<\tt>.
  	* Interaction values for each individual are generated from a gaussian distribution \f$\mathcal{N}(\mu , \sigma^2)\f$
  	* where \f$\mu\f$ is species interaction value, and \f$\sigma^2\f$ is evolution range for that interaction.
  	* 
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
  	*\param spec Species from which we will generate individuals.
  	*\param resolution How many discrete individuals are contained in one unit of density.
  	*/
	vector<unique_ptr<Individual>> getInds(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution);
  	/**
  	*\brief Method that selects surviving/reproducing individuals.
  	*
  	* This method calculates survival/reproduction probability for each individual
  	* (detailed in the Individual class), and returns a list indicating how many copies of
  	* each individual will be in the next generation (either 2 or 0).
  	* 
  	*\param inds List containing all individuals that were generated from evolving species.
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
	*
	*/
	vector<int> runSelection(vector<unique_ptr<Individual>>* inds, vector<unique_ptr<Species>>* speciesList);
	
};

#endif /*STDEVO_HPP*/