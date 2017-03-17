#include <vector>
#include <memory>
#include "NextGen.hpp"

class Environment;
class Species;

#ifndef RKSTDNEXTGEN_HPP
#define RKSTDNEXTGEN_HPP

/**
 * @file RKStdNextGen.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the standard implementation of the NextGen class using rk4
 *
 */


 /**
 *\brief Class containing a single method, used to control transition from one step to the next, and update Species/Environemnt attributes
 *
 */

class RKStdNextGen: public NextGen{
  public:
  	/**
  	*\brief Method used to update Species density at each timestep
  	*
  	* This method first calculates the change in species density for each species, then
  	* updates densities for each species. Species whose density is <Envionrment.deathThreshold
  	* will have their density set to 0, and be moved from SpeciesList to deadSpecies. Used
  	* for standard simulation with and without evolution.
  	*
  	*\param stepNum Values of the current step
  	*\param speciesList List containing all Species of the environment
  	*\param env Pointer to current environment
  	*/ 
  	void getNextGen(int stepNum, std::vector<std::unique_ptr<Species>>* speciesList, Environment* env);
};

#endif /*RKSTDNEXTGEN_HPP*/