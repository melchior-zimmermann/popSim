#include <cstdlib>
#include <iostream>
#include <vector>
#include "Simulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef MULTISIMULATION_HPP
#define MULTISIMULATION_HPP

/**
 * @file MultiSimulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the MultiSimulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * The MultiSimulation class acts the same way as the Simulation class, but does so on multiple environments.
 * It also gets migrations from each Environment, and redistributes them to the other Environments.
 */



class MultiSimulation: public Simulation{
  protected:
    vector<Environment> envs;
    int numEnvs;/**<Total number of environments in simulation*/
    int numSpecs;/**<Total number of Species in simulation*/

  public:
    MultiSimulation();
    MultiSimulation(vector<Environment> _envs): Simulation(move(_envs)){}
    void setEnvs(vector<Environment> newVal) {envs = move(newVal);}
    vector<Environment>* getEnvs() {return &envs;}
    void addEnv(Environment env) {envs.push_back(move(env));}
    void setNumEnvs(int newVal) {numEnvs = newVal;}
    int getNumEnvs() {return numEnvs;}
    void setNumSpecs(int newVal) {numSpecs = newVal;}
    int getNumSpecs() {return numSpecs;}

    virtual int runSim(int runNumber);


};


#endif /*MULTISIMULATION_HPP*/