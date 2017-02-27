#include <cstdlib>
#include <iostream>
#include <vector>
#include "Simulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef ECOSIMULATION_HPP
#define ECOSIMULATION_HPP

/**
 * @file EcoSimulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the EcoSimulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * The EcoSimulaiton class is very similar to the simulation class, except that it also saves all perturbations at
 * the end of a simulation.
 *
 */

class EcoSimulation: public Simulation{
  public:
  	EcoSimulation(vector<Environment> _env) :Simulation(move(_env)){}
    vector<Environment>* getEnvs() {return &envs;}
    void addEnv(Environment newVal) {envs.push_back(move(newVal));}
    int runSim(int runNumber);

};

#endif /*ECOSIMULATION_HPP*/