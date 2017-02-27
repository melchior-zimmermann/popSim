
#include <vector>
#include <memory>

#ifndef NEXTGEN_HPP
#define NEXTGEN_HPP

using namespace std;

class Environment;
class Species;


/**
 * @file NextGen.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the NextGen class (virtual), used to coordinate updates to Species/Environment attributes at each step
 *
 */


 /**
 *\brief Class containing a single method, used to control transition from one step to the next, and update Species/Environemnt attributes (virtual)
 *
 * An instance of an implementation of this class is owned by each Environment object, and used to coordinate update in Species density, interactions and environmental parameters
 * (when required).
 */

class NextGen{
  public:
  	/**
  	*\brief Method used to update Species/Environment attributes at each timestep (for details, see documentation of implantations)
  	*
  	*\param stepNum Values of the current step
  	*\param speciesList List containing all Species of the environment
  	*\param env Pointer to current environment
  	*/ 
  	virtual void getNextGen(int stepNum, std::vector<std::unique_ptr<Species>>* speciesList, Environment* env)=0;
};

#endif /*NEXTGEN_HPP*/