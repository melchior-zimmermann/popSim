#include <cstdlib>
#include <iostream>
#include <vector>
#include "Environment.hpp"

using namespace std;
/**
 * @file Simulation.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Simulation class, which contains the main loop of our simulation, as well as some control-flow attribtutes.
 */

 /**
 *\brief Class containing all Environment objects, as well control-flow attributes for the simulation.
 *
 * The Simulation class contains all Environment objects. It updates them each timeStep, and is also in charge of
 * deciding when to save what (for all save types).
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H



class Simulation {

  protected:
	  int numSteps;/**<Number of steps for which the simulation should be run*/
    int saveDiv;/**<Each how many steps densities are saved (in rawSave file)*/
    bool completeStart = false;/**<Determines whether or not a complete save is made at the beginning of the simulation.*/
    bool completeEnd = false;/**<Determines whether or not a complete save is made at the end of the simulation.*/           
	  vector<Environment> envs;/**<List containing all environments of the simulation*/

  public:
    Simulation();
    Simulation(vector<Environment> _env) :envs(move(_env)){}
    bool getCompleteStart(){return completeStart;}
    void setCompleteStart(bool newVal) {completeStart = newVal;}
    bool getCompleteEnd(){return completeEnd;}
    void setCompleteEnd(bool newVal) {completeEnd = newVal;}
  	int getNumSteps() {return numSteps;}
  	void setNumSteps(int newVal) {numSteps = newVal;}
  	int getSaveDiv() {return saveDiv;}
  	void setSaveDiv(int newVal) {saveDiv = newVal;}
  	vector<Environment>* getEnvs() {return &envs;}
  	void addEnv(Environment newVal) {envs.push_back(move(newVal));}

    /**
    * \brief Method that runs the main loop of the simulation.
    *
    *\param runNumber ID of the current run of the simulation.
    */
  	virtual int runSim(int runNumber);


};




#endif /*SIMULATION_H*/