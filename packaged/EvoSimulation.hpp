#include <cstdlib>
#include <iostream>
#include <vector>
#include "Simulation.hpp"
#include "Environment.hpp"

using namespace std;


#ifndef EVOSIMULATION_HPP
#define EVOSIMULATION_HPP

/**
 * @file EvoSimulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the EvoSimulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * The EvoSimulaiton class is very similar to the simulation class, except that it can also save interaction values of species
 * during simulaiton execution (to track changes due to evolution).
 *
 */

class EvoSimulation: public Simulation{
  protected:
    vector<Environment> envs;
    int interSaveDiv;/**<Each how many timesteps interaction values are saved for species (in rawSave file)*/
  public:
    EvoSimulation();
  	EvoSimulation(vector<Environment> _env) :Simulation(move(_env)){}
    int getInterSaveDiv() {return saveDiv;}
    void setInterSaveDiv(int newVal) {interSaveDiv = newVal;}
    vector<Environment>* getEnvs() {return &envs;}
    void addEnv(Environment newVal) {envs.push_back(move(newVal));}
    void setEnv(Environment newVal) {envs.push_back(move(newVal));}
    
    virtual int runSim(int runNumber);

};

#endif /*EVOSIMULATION_HPP*/