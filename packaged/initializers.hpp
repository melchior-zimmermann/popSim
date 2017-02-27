#include "Environment.hpp"
#include "Species.hpp"
#include "interface.hpp"
#include <vector>
#include <memory>


using namespace std;

/**
 * @file initializers.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing methods to initialize Environment/Species with values from simParams
 *
 * The two methods of this file are used to generate Environment/Species with attributes specified in
 * simParams. getEnv() calls initSpecs() to populate the Environment's speciesList.
 */

#ifndef INITIALIZERS_HPP
#define INITIALIZERS_HPP

/**
*\brief Populates speciesList with Species having attributes generated from the ranges contained in simParams
*
*\params speciesList List to be populated
*\params params Instance of simParams containing ranges from which to generate values for species attributes
*\params envConst pointer to attribute envConst of the Environment for which we are generating species
*/

int initSpecs(vector<unique_ptr<Species>>* speciesList, simParams params, double* envConst);
/**
*\brief Populates Environment with all parameters, based on informaiton in simParams (see simParams in interface.hpp)
*
*\params savePath Path to folder in which to save simulation results
*\params params Instance of simParams containing ranges from which to generate values for species attributes
*\params numSelf ID of environment being initiated
*\params allEnvs List of all environments in the simulation
*/
Environment getEnv(string savePath, simParams params, int numSelf, vector<Environment>* allEnvs);


#endif /*INITIALIZERS_HPP*/