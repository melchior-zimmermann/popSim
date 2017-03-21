#include <vector>
#include <memory>
#include "NextGen.hpp"


class Environment;
class Species;

/**
 * @file E2NextGen.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the implementation of the NextGen class using rk4 when evolution is on and environmental constant is taken into account
 *
 */


 /**
 *\brief Class containing a single method, used to control transition from one step to the next, and update Species/Environemnt attributes
 *
 */

#ifndef RKE2NEXTGEN_HPP
#define RKE2NEXTGEN_HPP

class RKE2NextGen: public NextGen{
  public:
  	/**
  	*\brief Method used to update Species density and interactions, as wel as environmental constant at each timestep
  	*
  	* This method works much in the same way as that of StdNextGen, but with the added functionality of both
  	* EcoNextGen and EvoNextGen.
  	*
  	*\param stepNum Values of the current step
  	*\param speciesList List containing all Species of the environment
  	*\param env Pointer to current environment
  	*/ 
  	void getNextGen(int stepNum, std::vector<std::unique_ptr<Species>>* speciesList, Environment* env);
};

#endif /*RKE2NEXTGEN_HPP*/