#include <cstdlib>
#include <iostream>
#include <vector>
#include "MultiEvoSimulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef E2MSIMULATION_HPP
#define E2MSIMULATION_HPP

/**
 * @file E2MSimulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the E2MSimulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * Same as E2Simulation, but for simulations with multiple environments (see also MultiSimulation)
 */

class E2MSimulation: public MultiEvoSimulation{
  protected:
    vector<Environment> envs;

  public:
    E2MSimulation(vector<Environment> _envs): MultiEvoSimulation(move(_envs)){}
    vector<Environment>* getEnvs() {return &envs;}
    void setEnvs(vector<Environment> newVal) {envs = move(newVal);}
    void addEnv(Environment env) {envs.push_back(move(env));}
    void setNumEnvs(int newVal) {numEnvs = newVal;}
    int getNumEnvs() {return numEnvs;}
    void setNumSpecs(int newVal) {numSpecs = newVal;}
    int getNumSpecs() {return numSpecs;}


    int runSim(int runNumber);


};

#endif /*E2MSIMULATION_HPP*/