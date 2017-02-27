#include <vector>
#include <memory>
#include "Individual.hpp"
#include "Evo.hpp"


class Species;

using namespace std;

/**
 * @file EcoEvo.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the EcoEvo class used to calculate evolutionary change in species interaction and optimum values.
 *
 */

#ifndef ECOEVO_HPP
#define ECOEVO_HPP

  /**
 *\brief Implementation of the Evo class that acts on species optimum as well as interactions.
 *
 * The EcoEvo class implements the getInds() method differently from StdEvo, since it creates
 * I2 objects to which it returns a list of pointers. The runSelection() method remains the same,
 * but getEvo() also updates species optimum.
 *
 */

class EcoEvo: public Evo{
  public:
  	/**
  	*\brief Controller method for updating species interactions/optimum.
  	*
  	* Calls getInds() to get a list of individuals on which to call runSelection().
  	* Then calculates the mean of all interaction values, as well as optimum, from selected individuals, and 
  	* sets this as the new interaction values, and optimum, for the species.
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
  	* <tt>numInds = round(species.density) * resolution + 1<\tt> (+1 to avoid divide-by-zero errors).
  	* Interaction values for each individual are generated from a gaussian distribution \f$\mathcal{N}(\mu , \sigma^2)\f$
  	* where \f$\mu\f$ is species interaction value, and \f$\sigma^2\f$ is evolution range for that interaction.
  	* Individual optimum is calculated in the same manner, with \f$\mu\f$ being species optimum, and
  	* \f$\sigma^2\f$ optRange.
  	* Objects pointed to are of class I2 instead of Individual (take environmental constant into account).
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
  	* (detailed in the I2 class), and returns a list indicating how many copies of
  	* each individual will be in the next generation (either 2 or 0).
  	* 
  	*\param inds List containing all individuals that were generated from evolving species.
  	*\param speciesList List containing all (alive) species present in the environment (used to calculate survival/reproduction probability of indivduals).
	*
	*/
	vector<int> runSelection(vector<unique_ptr<Individual>>* inds, vector<unique_ptr<Species>>* speciesList);
	
};

#endif /*ECOEVO_HPP*/