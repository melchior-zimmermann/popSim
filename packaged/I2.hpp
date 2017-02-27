#include <vector>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "Individual.hpp"

using namespace std;

/**
 * @file I2.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the I2 calss used to calculate evolutionary change in species attributes while taking the environmental constant into account (see Evo class).
 *
 */

#ifndef I2_HPP
#define I2_HPP


class Species;

 /**
 *\brief Class used to discretize continous species density, so as to select from during evolution events.
 *
 * The I2 class is used much like its parent class, but getSurvival() takes the
 * environmental constant into account when calculating survival/reproduction probability.
 */

class I2 : public Individual {
  public:
  	double *const envConst;/**<Pointer to the environmental constant (owned by Environment)*/
  	double optimum;/**<Individual optimum (value for envConst at which it thrives best)*/
    I2(double *const _envConst): envConst(_envConst){}
    /**
    *\brief Method to calculate survival/reproduction probability of individual
    *
    * Much the same as in Individual, except that, if survival/reproduction probability
    * is higher than 0.5, then:
    *
    *\f$survivalProb = 0.5 + (survivalProb - 0.5)*e^{-(optimum-(*envConst))^2}\f$
    *
    *\param speciesList List containing all (alive) species in the environment
    *\param delta Size of timestep of simulation
    */ 
    double getSurvival(vector<unique_ptr<Species>>* allSpecs, double delta);
};

#endif /*I2_HPP*/