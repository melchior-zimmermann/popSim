#include <memory>
#include <vector>
#include "Change.hpp"


using std::vector;
using std::unique_ptr;


/**
 * @file StdChange.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the standard implementation of the virtual Change class.
 *
 */



class Species;

#ifndef STDCHANGE_HPP
#define STDCHANGE_HPP

 /**
 *\brief Class containing just one function, used to calculate change in species density.
 *
 * The StdChange class implements the inherited virtual method getChange(), used to calculate change
 * in species density. The implementation of StdChange calculates change in species density as:
 *
 *\f$\dot{x_i} = \delta * x_i * (\alpha + (1 - (cc/x_i)^2) + \beta + \sum_{j \neq i}^numTotal I_{ij} * x_j)\f$
 *
 *where all parameters except \f$\delta\f$ are those of the species (\f$x_i, x_j\f$ refer to density of species i and j, respectively).
 \f$\delta\f$ is owned by the environment.
 *
 */

class StdChange: public Change{
  public:
  	double getChange(Species* spec, double delta, vector<unique_ptr <Species>>* speciesList);
};

#endif /*STDCHANGE_HPP*/