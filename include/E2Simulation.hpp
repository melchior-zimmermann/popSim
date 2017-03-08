#include <cstdlib>
#include <iostream>
#include <vector>
#include "EvoSimulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef E2SIMULATION_HPP
#define E2SIMULATION_HPP

/**
 * @file E2Simulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the E2Simulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * The E2Simulaiton class is very similar to the simulation class, except that it has the extra functionality
 * of both EcoSimulation and EvoSimulation.
 *
 */

class E2Simulation: public EvoSimulation{
  protected:
    vector<Environment> envs;
  public:
  	E2Simulation(vector<Environment> _env) :EvoSimulation(move(_env)){}
    int getInterSaveDiv() {return saveDiv;}
    void setInterSaveDiv(int newVal) {interSaveDiv = newVal;}
    vector<Environment>* getEnvs() {return &envs;}
    void addEnv(Environment newVal) {envs.push_back(move(newVal));}
    int runSim(int runNumber);

};


#endif /*E2SIMULATION_HPP*/