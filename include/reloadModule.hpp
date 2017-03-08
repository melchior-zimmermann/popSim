#include "Environment.hpp"
#include "interface.hpp"
#include <string>
#include <vector>


using namespace std;

/**
 * @file reloadModule.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing methods to reload past simulations and reRun them, evtl. adding species.
 *
 * The two methods of this file are used to reload past simulations form a complete save, and
 * reRun that simulation again. Parameters of the simulation (and the newly added species)
 * are decided by the values in the simParam file found in the same folder as the complete
 * save used to reload the simulation (for a more complete explanation of how to reRun simulations,
 * and what kind of modifications can be done, please consult the user manual).
 */


#ifndef RELOADMODULE_HPP
#define RELOADMODULE_HPP

/**
*\brief Structure containing all the values of the simulation we loaded
*
* This structure will be populated with the values from the complete save file used to 
* reload the simulation. There will be one of these structures filled for each environment
* of the old simulation.
*/

struct simValues{
	int oldSpec;/**<Number of Species that were alive at the end of the old simulation*/
	int addSpec;/**<Number of Species to add in the new simulation*/
	int totSpec;/**<Total number of Species in the new simulation*/
	int numEnvs;/**<Numnber of environment in the simulaiton (same for old and new)*/
	double envConst = 0;/**<Value of the environmental constant*/
	vector<int> genTimes;/**<Rates of evolution for each Species*/
	vector<int> densToInds;/**<Number of individuals per units of density for each Species*/
	vector<int> IDs;/**<IDs of live Species when the complete save was made*/
	vector<double> alphas;/**<Alphas of (live) Species in old simulation*/
	vector<double> betas;/**<Betas of (live) Species in old simulation*/
	vector<double> ccs;/**<Carrying capacity (cc) of (live) Species in old simulation*/
	vector<double> densities;/**<Densities of (live) Species in old simulation*/
	vector<double> optimums;/**<Optimumss of (live) Species in old simulation*/
	vector<double> optRange;/**<Range of optimum evolution of (live) Species in old simulation*/
	vector<vector <double> > interactions;/**<Interaction values of (live) Species in old simulation*/
	vector<vector <double> > evoRanges;/**<Range of evolution of interaction values of (live) Species in old simulation*/
	vector<vector<vector<vector<double>>>> routes;/**<Migration routes of (live) Species in old simulation*/
	
	
};

/**
*\brief Parser method used to populate a simValues instance with values form a previous simulation.
*
*\param params Simulation parameters of the simulation we are loading
*\param value simValues object to populate
*\param Path to the complete save used to load simulation
*/

int loadSimValues(simParams* params, simValues* values, string savePath );
/**
*\brief Method used to recreate an Environment with the same attribute values than that of the previous simulation.
*
*\param newSavePath Folder where results of the new simulation will be saved
*\param allEnvs Pointer to vector of Environment's owned by the Simulation object
*\param params Pointer to the simParams object from previous simulation
*\param values Pointer to the simValues object containing values of the previous simulation
*\param numSelf ID of the Environment to create
*\param savePath Path to the folder where results/parameters of the new simulation will be saved
*/
Environment loadEnv(string newSavePath, vector<Environment>* allEnvs, simParams* params, simValues* values, int numSelf, string savePath);
/**
*\brief Method used to recreate Species based on the values of the previous simulation (will also add the specified numbers of Species
* with parameter values generated from the ranges in simParams)
*
*\param speciesList Pointer to the vector of species owned by Environment
*\param params Pointer to the simParams object
*\param values Pointer to the simValues object containing values of the previous simulation
*\param envConst Pointer to envConst of the Environment
*/
void loadSpecs(vector<unique_ptr<Species>>* speciesList, simParams* params, simValues* values, double* envConst);
/**
*\brief Controller method that takes care of loading a simulation, creating the right Environment/Species, and launching the simulation
*
*\param oldSavePath The path to the folder containing the complete save/simParam file of the simulation we want to load
*\param newSavePath The path to the folder where results of the new simulation will be saved
*\param addSpec The number of newly generated Species that will be added to the simulation
*\param numRuns The number of runs we will execute on the new simulation
*/
int reRunSim(string oldSavePath, string newSavePath, int addSpec, int numRuns);


#endif /*RELOADMODULE_HPP*/