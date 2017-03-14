
#include <memory>
#include <vector>

#include "Change.hpp"


using std::vector;
using std::unique_ptr;


/**
 * @file RKChange.hpp
 * @author Melchior Zimmermann
 * @date 14 Mar 2017
 * @brief File containing the definition of a wrapper class for Change-implementations to solve them using RK4.
 *
 */



class Species;

#ifndef RKCHANGE_HPP
#define RKCHANGE_HPP

 /**
 *\brief Class containing just one function, used to calculate change in species density.
 *
 * The RKChange class wraps around other implementations of the virtual Change class and uses them to estimate the derivative
 * to solve the differential equation governing change in species density using RK4.
 *
 */

class RKChange: public Change{
  private:
  	unique_ptr<Change> change;
  public:
  	RKChange(unique_ptr<Change> _change); 
  	double getChange(Species* spec, double delta, vector<unique_ptr <Species>>* speciesList);
};

#endif /*RKCHANGE_HPP*/