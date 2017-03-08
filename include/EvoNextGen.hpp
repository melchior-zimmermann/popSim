#include <vector>
#include <memory>

#include "NextGen.hpp"

class Environment;
class Species;


/**
 * @file EvoNextGen.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the implementation of the NextGen class when evolution is on
 *
 */


 /**
 *\brief Class containing a single method, used to control transition from one step to the next, and update Species/Environemnt attributes
 *
 */

#ifndef EVONEXTGEN_HPP
#define EVONEXTGEN_HPP

class EvoNextGen: public NextGen{
  public:
  	/**
  	*\brief Method used to update Species density and interactions at each timestep
  	*
  	* This method works much in the same way as that of StdNextGen, except that after calculating
  	* cahnge in Species density for all species (and moving dead species), it calls spec.getEvo()
  	* on each Species where <tt>stepNum%spec.evoRate == 0</tt>
  	*
  	*\param stepNum Values of the current step
  	*\param speciesList List containing all Species of the environment
  	*\param env Pointer to current environment
  	*/ 
  	void getNextGen(int stepNum, std::vector<std::unique_ptr<Species>>* speciesList, Environment* env);
};

#endif /*EVONEXTGEN_HPP*/