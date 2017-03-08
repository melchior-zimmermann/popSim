#include <memory>
#include <vector>
#include "Change.hpp"

class Species;

using namespace std;

#ifndef ECOCHANGE_HPP
#define ECOCHANGE_HPP

/**
 * @file EcoChange.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the implementation of the virtual Change class taking the environmental constant into account.
 *
 */


 /**
 *\brief Class containing just one function, used to calculate change in species density.
 *
 * The EcoChange class calculates change in species density taking the environmental constant
 * and species optimum into account. The same formula as in StdChange is used (not inherited since it
 * is very little code and we want to keep the calling stack to a minimum for performance). However, if change is
 * positive (species density is increasing), it is modified by:
 *
 *\f$ \dot{x_i} = \dot{x_i} * e^{-(envConst - opt)^2}\f$
 *
 */
class EcoChange: public Change{
  public:
  	double getChange(Species* spec, double delta, vector<unique_ptr<Species>>* speciesList);
};

#endif /*ECOCHANGE_HPP*/