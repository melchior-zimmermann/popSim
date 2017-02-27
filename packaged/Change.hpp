#include <memory>
#include <vector>

using namespace std;

/**
 * @file Change.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Change class used to calculted variation in density over time.
 *
 */



class Species;

#ifndef CHANGE_HPP
#define CHANGE_HPP

 /**
 *\brief Class containing just one function, used to calculate change in species density (virtual).
 *
 * The Change class has no attributes and only one method, used to calculate change in species density.
 * For now, there are only two implemented versions of that method, with and without taking the
 * environmental constant and species optimum into account (EcoChange and StdChange).
 *
 */

class Change{
  public:
  	/**
  	*\brief Method to calculate change in species density.
  	*
  	* This method uses the explicit Euler scheme to resolve a Lotka-Volterra-like
  	* differential equation we use to calculate the evolution of the system.
	*\param spec Species for which we will calculate change in density.
	*\param delta size of step used in explicit Euler.
	*\param speciesList List containing all (alive) species present in the environment.
	*/
  	virtual double getChange(Species* spec, double delta, vector<unique_ptr<Species>>* speciesList)=0;
};

#endif /*CHANGE_HPP*/