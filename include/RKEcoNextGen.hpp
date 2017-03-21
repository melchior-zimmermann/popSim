#include <vector>
#include <memory>
#include "NextGen.hpp"

class Environment;
class Species;

#ifndef RKECONEXTGEN_HPP
#define RKECONEXTGEN_HPP

/**
 * @file RKEcoNextGen.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the implementation of the NextGen class using rk4 when environmental constant is taken into account
 *
 */


 /**
 *\brief Class containing a single method, used to control transition from one step to the next, and update Species/Environemnt attributes
 *
 */


class RKEcoNextGen: public NextGen{
  public:
  	/**
  	*\brief Method used to update Species density and environmental constant at each timestep
  	*
  	* This method works much in the same way as that of StdNextGen, except that if
  	* a call to getRandom() (heleprs.hpp) returns a value smaller than env.pertProb, the environmental constant
  	* of env will be changed by a random amount drawn form the uniform distribution (env.pertRange[0], env.pertRange[1])
  	* (this happens before species change in densities are calculated). This perturbation is saved in env.perturbations.
  	*
  	*\param stepNum Values of the current step
  	*\param speciesList List containing all Species of the environment
  	*\param env Pointer to current environment
  	*/ 
  	void getNextGen(int stepNum, std::vector<std::unique_ptr<Species>>* speciesList, Environment* env);
};

#endif /*RKECONEXTGEN_HPP*/