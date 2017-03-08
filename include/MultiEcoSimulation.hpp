#include <cstdlib>
#include <iostream>
#include <vector>
#include "MultiSimulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef MULTIECOSIMULATION_HPP
#define MULTIECOSIMULATION_HPP

/**
 * @file MultiEcoSimulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the MultiEcoSimulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * Same as EcoSimulation, but for simulations with multiple environments (see also MultiSimulation)
 */

class MultiEcoSimulation: public MultiSimulation{
  protected:
    vector<Environment> envs;
  public:
    MultiEcoSimulation();
  	MultiEcoSimulation(vector<Environment> _envs):MultiSimulation(move(_envs)){}
    vector<Environment>* getEnvs() {return &envs;}
    void setEnvs(vector<Environment> newVal) {envs = move(newVal);}
    void addEnv(Environment env) {envs.push_back(move(env));}


    int runSim(int runNumber);
};


#endif /*MULTIECOSIMULATION_HPP*/