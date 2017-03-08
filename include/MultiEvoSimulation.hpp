#include <cstdlib>
#include <iostream>
#include <vector>
#include "MultiSimulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef MULTIEVOSIMULATION_HPP
#define MULTIEVOSIMULATION_HPP

/**
 * @file MultiEvoSimulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the MultiEvoSimulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * Same as EvoSimulation, but for simulations with multiple environments (see also MultiSimulation)
 */




class MultiEvoSimulation: public MultiSimulation{
  protected:
    int interSaveDiv;/**<Each how many timestep Species interaction values are saved (in rawSave file)*/ 
    vector<Environment> envs;
  public:
    MultiEvoSimulation();
    MultiEvoSimulation(vector<Environment> _envs): MultiSimulation(move(_envs)){}
    void setEnvs(vector<Environment> newVal) {envs = move(newVal);}
    vector<Environment>* getEnvs() {return &envs;}
    void addEnv(Environment env) {envs.push_back(move(env));}
    int getInterSaveDiv() {return saveDiv;}
    void setInterSaveDiv(int newVal) {interSaveDiv = newVal;}

    virtual int runSim(int runNumber);
};

#endif /*MULTIEVOSIMULATION_HPP*/