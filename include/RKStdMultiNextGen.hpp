#include <vector>
#include <memory>
#include "NextGen.hpp"

class Environment;
class Species;

#ifndef RKSTDMULTINEXTGEN_HPP
#define RKSTDMULTINEXTGEN_HPP

/**
 * @file RKStdMultiNextGen.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the standard implementation of the NextGen class using rk4 for multi-environment simulations
 *
 */


 /**
 *\brief Class containing a single method, used to control transition from one step to the next, and update Species/Environemnt attributes
 *
 */


class RKStdMultiNextGen: public NextGen{ 
  public: 
  	/**
  	*\brief Method used to update Species density at each timestep
  	*
  	* This method works exactly like the one in StdNextGen, except that species with
  	* <tt>density == 0</tt> are not moved to env.deadSpecies, to avoid having to recreate
  	* them if a population from said species migrates into environment from somewhere.
  	* If migration probability is very low for all species, it might be worth to delete/introduce
  	* them each time, since this might take less time than the increased loop time caused by a higher
  	* number of species (I'd say it depends on the simulation parameters, but I haven't done any performance
  	* tests to compare the two methods).
  	*
  	*\param stepNum Values of the current step
  	*\param speciesList List containing all Species of the environment
  	*\param env Pointer to current environment
  	*/ 
  	void getNextGen(int stepNum, std::vector<std::unique_ptr<Species>>* speciesList, Environment* env);
};

#endif /*RKSTDMULTINEXTGEN_HPP*/