#include <vector>
#include <cstdlib>
#include <iostream>
#include <memory>


using namespace std;

/**
 * @file Individual.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Individual calss used to calculate evolutionary change in species attributes (see Evo class).
 *
 */

#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

class Species;

 /**
 *\brief Class used to discretize continous species density, so as to select from during evolution events.
 *
 * The Individual class is used to represent discrete individuals of a species (which has continous density).
 * It has a similar (but much more restricted) set of attributes as Species, although what is calculated
 * by individuals is a probability of survival/reproduction, as opposed to a change in density (Species).
 * This class (and its inheritor) are used during evolution events to discretize species density (see Evo class).
 */

class Individual{
  public:
    int specID;/**<ID of species the individual belongs to.*/
    double optimum;/**<optimum of the individual (only used in simulations with evolution)*/
    int numTotal;/**<Total number of species at start of simulation*/
    vector<double> interactions; /**<interaction values for individuals*/
    double survival = 0; /**<Probability that a given individual survives and reproduces*/
    /**
    *\brief Method to calculate survival/reproduction probability of individual
    *
    * This method calculates survival/reproduction porbability of the individual
    * with equations similar to those used by the species:
    *
    * \f$P(S_i) = \delta \cdot \sum_{j \neg i} II_{ij}*x_j +0.5\f$
    *
    * where \f$II_{ij}\f$ is the interaction of individual i with species j, and
    * \f$x_j\f$ is density of species j.
    *
    *\param speciesList List containing all (alive) species in the environment
    *\param delta Size of timeStep.
    */
    virtual double getSurvival(vector<unique_ptr<Species>>* allSpecs, double delta);
    
};

#endif /*INDIVIDUAL_HPP*/